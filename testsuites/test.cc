#include "cJSON.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// only used in test pch and ast test.
int main() {
	cJSON* root = cJSON_CreateObject();
	int i = 0;
	if (root)
		cJSON_Delete(root);
	char *buf = (char*) malloc(20);
	if (buf != NULL)
	{
		free(buf);
	}
	else {
		buf = NULL;
		goto exit;
	}

	for(int i = 1; i < 10; i++) {
		buf[i] = (char) i;
	}
	for(;;){
		break;
	}
	while(i<10) {
		i++;
	}

	do {
		i++;
	} while(i<10);

exit:
	printf("%s", buf);
	return 1;
}
// clang -fsyntax-only -Xclang -ast-dump=json -Xclang -ast-dump-filter=main -I/data/home/loydlv/vbd/llm_fuzz/data/cJSON/include test.c > test_ast.json