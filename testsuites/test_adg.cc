//<Parent> None
//<Combination> ["cJSON * cJSON_CreateFloatArray(const f32 * numbers, int count)", "cJSON * cJSON_ParseWithOpts(const char * value, const char ** return_parse_end, cJSON_bool require_null_terminated)", "cJSON_bool cJSON_HasObjectItem(const cJSON * object, const char * string)", "cJSON_bool cJSON_PrintPreallocated(cJSON * item, char * buffer, int length, cJSON_bool format)"]
//<Prompt> Prompt { kind: Generate } 
//<Mutator> Mutator { op: NoMutation, response: None }
/* 
Create a C language program of cJSON library by following the instructions below:
1. include all header files of cJSON;
2. write a main function calls the following APIs: 
cJSON * cJSON_CreateFloatArray(const f32 * numbers, int count),
cJSON * cJSON_ParseWithOpts(const char * value, const char ** return_parse_end, cJSON_bool require_null_terminated),
cJSON_bool cJSON_HasObjectItem(const cJSON * object, const char * string),
cJSON_bool cJSON_PrintPreallocated(cJSON * item, char * buffer, int length, cJSON_bool format);
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <cJSON.h>
#include <iostream>
#include <fstream>
#include <vector>

#define MAX_LEN 1024

int main(int argc, char *argv[])
{
	if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " input_file" << std::endl;
        return 1;
    }
    std::ifstream input_file(argv[1], std::ios::binary);
    if (!input_file)
    {
        std::cerr << "Failed to open input file: " << argv[1] << std::endl;
        return 1;
    }
    std::vector<uint8_t> input_data((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());
    input_file.close();

    const uint8_t *data = input_data.data();
    size_t data_sz = input_data.size();

	cJSON *root, *flt_arr, *flt_arr_item, *str_arr, *str_arr_item;
	char *out;
	int i;
	float flt_arr_data[4] = {1.1, 2.2, 3.3, 4.4};
	const char *str_arr_data[4] = {"a", "b", "c", "d"};

    root = cJSON_Parse((const char *)data);
	flt_arr = cJSON_CreateFloatArray(flt_arr_data, 4);
	cJSON_AddItemToObject(root, "flt_arr", flt_arr);
	str_arr = cJSON_CreateArray();
	for (i = 0; i < 4; i++)
	{
		str_arr_item = cJSON_CreateString(str_arr_data[i]);
		cJSON_AddItemToArray(str_arr, str_arr_item);
	}
	cJSON_AddItemToObject(root, "str_arr", str_arr);

	out = cJSON_Print(root);
	printf("%s\n", out);
	free(out);

	flt_arr_item = cJSON_GetObjectItem(root, "flt_arr");
	if (flt_arr_item != NULL)
	{
		cJSON_DeleteItemFromArray(flt_arr_item, 1);
	}
	str_arr_item = cJSON_GetObjectItem(root, "str_arr");
	if (str_arr_item != NULL)
	{
		cJSON_DeleteItemFromArray(str_arr_item, 1);
	}

	out = cJSON_Print(root);
	printf("%s\n", out);
	free(out);

	cJSON_Delete(root);
	return 0;
}