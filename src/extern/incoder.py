from typing import Union, List
import argparse
from transformers import AutoModelForCausalLM, AutoTokenizer, StoppingCriteriaList, StoppingCriteria
# config the logging module
import logging
import logging.config
import yaml
import os
import torch


parser = argparse.ArgumentParser()
parser.add_argument("--big_model", type=int, metavar="0", default=0, help="Wether use the big incoder-6B model.")
parser.add_argument("--port", type=int, default=7860, help="The port number that the service is on.")
parser.add_argument("--log_file", type=str, default="incoder.log", help="The file to record the log messages.")
args = parser.parse_args()


BIG_MODEL = args.big_model
PORT = args.port
LOG_FILE = args.log_file


this_file = os.path.abspath(__file__)
log_conf = os.path.join(os.path.dirname(this_file), "logging.yaml")
with open(log_conf, 'r') as f_conf:
    dict_conf = yaml.load(f_conf, Loader=yaml.FullLoader)
    dict_conf["handlers"]["file"]["filename"] = LOG_FILE
logging.config.dictConfig(dict_conf)

logger = logging.getLogger('mylogger')


# config the request and response structure
MAX_LENGTH: int = 2048

from pydantic import BaseModel
class Request(BaseModel):
    prefix: str
    suffix: Union[str, None]=None
    max_tokens: int = MAX_LENGTH
    temperature: Union[float, None]=None
    top_p: Union[float, None]=0.95
    n_samples: int=1

class Response(BaseModel):
    choices: List[str]
    done_status: List[bool]

# config the Incoder model
CUDA = torch.cuda.is_available()
PAD = "<pad>"
# signals the start of a document
BOS = "<|endoftext|>"
# signals the end of a generated infill
EOM = "<|endofmask|>"
STOP_WORDS= ["</code>", EOM]


def make_sentinel(i) -> str:
    return f"<|mask:{i}|>"

SPECIAL_TOKENS = [make_sentinel(i) for i in range(256)] + [EOM]

# stop generation when meet the criteria
class StopWordsStoppingCriteria(StoppingCriteria):
    def __init__(self, init_lengths: List[int], stop_words_encoded: List[List[int]]):
        super().__init__()
        self.init_lengths = init_lengths
        if stop_words_encoded is None:
            stop_words_encoded = []
        else:
            assert isinstance(stop_words_encoded[0], list)
        assert isinstance(stop_words_encoded, list)
        self.stop_words_encoded = stop_words_encoded

    def _contains_stop_words(self, tokens: List[int]):
        if not bool(self.stop_words_encoded):
            return False
        for start_ix in range(len(tokens)):
            for swe in self.stop_words_encoded:
                if tokens[start_ix:start_ix+len(swe)] == swe:
                    return True
        return False

    def __call__(self, input_ids: torch.LongTensor, scores: torch.FloatTensor, **kwargs) -> bool:
        for init_length, i_tokens in zip(self.init_lengths, input_ids):
            if not self._contains_stop_words(i_tokens[init_length:].tolist()):
                return False
        return True

class IncoderModel:
    def __init__(self, big_model=False, cuda=True, device=None, tokenizer=None, half=True, model=None):
        if big_model:
            self.model_name = "facebook/incoder-6B"
        else:
            self.model_name = "facebook/incoder-1B"

        if cuda:
            assert device is None or device.startswith("cuda")
            if device is None:
                device = "cuda"
        else:
            assert device is None or device == "cpu"
            if device is None:
                device = "cpu"
        
        self.device = device

        if big_model:
            if cuda:
                kwargs = dict(
                    revision="float16",
                    torch_dtype=torch.float16,
                    low_cpu_mem_usage=True,
                )
            else:
                kwargs = dict(
                    low_cpu_mem_usage=True,
                )
        else:
            kwargs = {}

        if model is None:
            logger.info("loading model: %s" % (self.model_name))
            model = AutoModelForCausalLM.from_pretrained(self.model_name, **kwargs)
        if tokenizer is None:
            logger.info("loading tokenizer")
            tokenizer = AutoTokenizer.from_pretrained(self.model_name)
        self.tokenizer = tokenizer
        self.tokenizer.padding_side = "left"
        self.tokenizer.pad_token = PAD
        assert self.tokenizer.pad_token_id == 1
        logger.info("loading complete")
        
        if cuda and half:
            self.half = True
            model = model.half()
        else:
            self.half = False
        model = model.to(device)
        self.model = model
        self.cuda = cuda
    
    # generate an output with a signle input.
    def single_generate(self, prompt: str, max_tokens: int, temperature: float, top_p: float):
        input_ids = self.tokenizer(prompt, return_tensors="pt").input_ids
        if CUDA:
            input_ids = input_ids.cuda()
        stopping_criteria = StoppingCriteriaList()
        stop_words = STOP_WORDS
        if stop_words is not None:
            stop_words_encoded = [self.tokenizer.encode(word, add_special_tokens=False) for word in stop_words]
            stopping_criteria.append(StopWordsStoppingCriteria([max_tokens for l in [prompt]], stop_words_encoded))

        output = self.model.generate(input_ids=input_ids, do_sample=True, top_p=top_p,
                                    temperature=temperature, max_length=max_tokens, stopping_criteria=stopping_criteria)
        detok_hypo_str = self.tokenizer.decode(output.flatten())
        if detok_hypo_str.startswith(BOS):
            detok_hypo_str = detok_hypo_str[len(BOS):]
        return detok_hypo_str
    
    # generate a single infill
    def single_infill(self, prefix: str, suffix: str, max_tokens: int=MAX_LENGTH,
                    temperature: Union[float, None]=None, top_p: Union[float, None]=0.95) -> Response:
        choices: List[str] = []
        done_status: List[bool] = []
        # combine the prefix and suffix to a prompt
        prompt: str = prefix + make_sentinel(0) + suffix + make_sentinel(1)
        max_length: int = min(max_tokens, MAX_LENGTH)

        # invoke incoder's generation
        output = self.single_generate(prompt, max_length, temperature, top_p)
        completion = output[len(prompt):] # the new inserted text.
        if EOM not in completion:
            completion += EOM
            done = False
        completion = completion[:completion.index(EOM) + len(EOM)]
        # The new generated infills.
        infilled = completion[:-len(EOM)]
        choices.append(infilled)
        done_status.append(done)
        # format the response
        response = Response(choices=choices, done_status=done_status)
        return response

    # generate a single extend
    def single_extend(self, prefix: str, max_tokens: int=MAX_LENGTH,
                    temperature: Union[float, None]=None, top_p: Union[float, None]=0.95) -> Response:
        max_length: int = min(max_tokens, MAX_LENGTH)
        # invoke incoder's generation
        generation = self.single_generate(prefix, max_length, temperature, top_p)
        # format the response
        choices: List[str] = [generation]
        done_status: List[bool] = [True]
        response = Response(choices=choices, done_status=done_status)
        return response

    # generate a batch of output with the batched input
    def batched_generate(self, inputs: List[str], max_to_generate: int=128, temperature: Union[float, None]=0.2,
                         top_p: Union[float, None]=0.95,trim_prefix: bool=True, stop_words=None):
        assert self.tokenizer.padding_side == 'left'
        assert isinstance(inputs, list)
        batch = self.tokenizer(inputs, padding="longest", truncation=True, return_tensors="pt")
        batch = batch.to(self.device)
        max_input_length = batch.input_ids.size(1)
        max_length = max_input_length + max_to_generate
        stopping_criteria = StoppingCriteriaList()
        if stop_words is not None:
            stop_words_encoded = [self.tokenizer.encode(word, add_special_tokens=False) for word in stop_words]
            stopping_criteria.append(StopWordsStoppingCriteria([max_input_length for l in inputs], stop_words_encoded))
        if max_length > 2048:
            print("warning: max_length {} is greater than the context window {}".format(max_length, 2048))
        with torch.no_grad():
            outputs = self.model.generate(input_ids=batch.input_ids, attention_mask=batch.attention_mask,
                                        do_sample=True, top_p=top_p, temperature=temperature, max_length=max_length, stopping_criteria=stopping_criteria)
        
        hypo_strs = []
        for input, output in zip(inputs, outputs):
            detok_hypo_str = self.tokenizer.decode(output.flatten(), clean_up_tokenization_spaces=False)
            while detok_hypo_str.startswith(PAD):
                detok_hypo_str = detok_hypo_str[len(PAD):]
            if detok_hypo_str.startswith(BOS):
                detok_hypo_str = detok_hypo_str[len(BOS):]

            if trim_prefix:
                detok_hypo_str = detok_hypo_str[len(input):]
                #detok_hypo_str = remove_extra_code(detok_hypo_str)
            hypo_strs.append(detok_hypo_str)
        return hypo_strs

    def generate(self, prefix: str, max_tokens: int=MAX_LENGTH, temperature: Union[float, None]=None,
                top_p: Union[float, None]=0.95, n_samples: int=1) -> Response:
        if n_samples == 1:
            res = self.single_extend(prefix, max_tokens, temperature, top_p)
            return res
        else:
            inputs = [prefix for i in range(n_samples)]
            outputs = self.batched_generate(inputs, max_to_generate=max_tokens, temperature=temperature,
                                            trim_prefix = False, stop_words=STOP_WORDS)
            res = Response(choices=outputs, done_status=[])
            return res

    def infill(self, prefix: str, suffix: str, max_tokens: int=MAX_LENGTH, temperature: Union[float, None]=None,
                top_p: Union[float, None]=0.95, n_samples: int=1) -> Response:
        if n_samples == 1:
            res = self.single_infill(prefix, suffix, max_tokens, temperature, top_p)
        else:
            choices: List[str] = []
            done_status: List[bool] = []
            input: str = prefix + make_sentinel(0) + suffix + make_sentinel(1)
            inputs = [input for i in range(n_samples)]
            outputs = self.batched_generate(inputs, max_to_generate=max_tokens, temperature=temperature,
                                            trim_prefix = True, stop_words=STOP_WORDS)
            for output in outputs:
                completion = output
                if EOM not in completion:
                    completion += EOM
                    done = False
                completion = completion[:completion.index(EOM) + len(EOM)]
                # The new generated infills.
                infilled = completion[:-len(EOM)]
                choices.append(infilled)
                done_status.append(done)
            res = Response(choices=choices, done_status=done_status)
            return res

# instantiate incoder model
if __name__ != "__main__":
    incoder = IncoderModel(big_model=BIG_MODEL, cuda=CUDA)

# test functions
def test_generation():
    prefix = """
/* 
Create a C language program of c-ares library by following the instructions below:
1. include all header files of c-ares;
2. write a main function calls the following APIs: 
ares_cancel;
*/"""
    suffix = None
    max_tokens = 1024
    temperature = 0.9
    top_p = 0.95
    n_samples = 1
    res = incoder.generate(prefix, max_tokens, temperature, top_p, n_samples=n_samples)
    print(res)

def test_batched_generate():
    prefix = """
/* 
Create a C language program of c-ares library by following the instructions below:
1. include all header files of c-ares;
2. write a main function calls the following APIs: 
ares_cancel;
*/"""
    max_tokens = 1024
    temperature = 0.2
    top_p = 0.95
    n_samples = 5
    res = incoder.generate(prefix, max_tokens, temperature, top_p, n_samples=n_samples)
    for choice in res.choices:
        print("\n\n-------")
        print(choice)

# evaluate test time cost per request: near 35s per signle request and 69s per 5 batched request on 8GB GPU.
#import time
##start = time.time()
#test_batched_generate()
#end = time.time()
#print("%s seconds per request" % (end - start))



# API module
from fastapi import FastAPI
app = FastAPI(docs_url=None, redoc_url=None)

@app.post("/infill/")
async def create_infill(req: Request) -> Response:
    res = incoder.infill(req.prefix, req.suffix, req.max_tokens, req.temperature, req.top_p, req.n_samples)
    return res

@app.post("/generate/")
async def create_extend(req: Request) -> Response:
    res = incoder.generate(req.prefix, req.max_tokens, req.temperature, req.top_p, req.n_samples)
    return res

@app.get("/syn/")
async def send_ack(id: str) -> int:
    syn_id = int(id)
    return syn_id + 1

import uvicorn
if __name__ == "__main__":
    uvicorn.run(app="incoder:app", host='0.0.0.0', port=PORT, log_config = dict_conf, log_level="trace", limit_concurrency=2)
