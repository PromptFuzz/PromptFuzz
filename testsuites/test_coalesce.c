//<Parent> None
//<Combination> ["cJSON_bool cJSON_IsObject(const cJSON * item)", "cJSON_bool cJSON_IsNumber(const cJSON * item)", "void cJSON_InitHooks(cJSON_Hooks * hooks)", "cJSON_bool cJSON_IsFalse(const cJSON * item)"]
//<Prompt> Prompt { kind: Generate } 
//<Mutator> Mutator { op: NoMutation, response: None }
/* 
Create a C language program of cJSON library by following the instructions below:
1. include all header files of cJSON;
2. write a main function calls the following APIs: 
cJSON_bool cJSON_IsObject(const cJSON * item),
cJSON_bool cJSON_IsNumber(const cJSON * item),
void cJSON_InitHooks(cJSON_Hooks * hooks),
cJSON_bool cJSON_IsFalse(const cJSON * item);
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cJSON.h>

#define MAX_JSON_SIZE 1024

int main(int argc, char *argv[])
{
	char json_str[MAX_JSON_SIZE];
	cJSON *json_item = NULL;
	cJSON_Hooks hooks;

	if (argc < 2) {
		printf("Usage: %s <json_string>\n", argv[0]);
		return -1;
	}

	strncpy(json_str, argv[1], MAX_JSON_SIZE);
	json_str[MAX_JSON_SIZE - 1] = '\0';

	cJSON *json_obj = cJSON_Parse(json_str);
	if (!json_obj) {
		printf("Failed to parse json string\n");
		return -1;
	}

	/*
	 * cJSON_IsObject(const cJSON * item)
	 *
	 * Return true if the given item is a JSON object.
	 */
	if (cJSON_IsObject(json_obj)) {
		printf("json_obj is a JSON object\n");
	} else {
		printf("json_obj is not a JSON object\n");
	}

	/*
	 * cJSON_IsNumber(const cJSON * item)
	 *
	 * Return true if the given item is a JSON number.
	 */
	if (cJSON_IsNumber(json_obj)) {
		printf("json_obj is a JSON number\n");
	} else {
		printf("json_obj is not a JSON number\n");
	}

	/*
	 * cJSON_IsFalse(const cJSON * item)
	 *
	 * Return true if the given item is a JSON false.
	 */
	if (cJSON_IsFalse(json_obj)) {
		printf("json_obj is a JSON false\n");
	} else {
		printf("json_obj is not a JSON false\n");
	}

	/*
	 * cJSON_InitHooks(cJSON_Hooks * hooks)
	 *
	 * Initialize the hooks to be used by the cJSON library.
	 */
	memset(&hooks, 0, sizeof(cJSON_Hooks));
	cJSON_InitHooks(&hooks);
	if (hooks.malloc_fn == NULL) {
		printf("hooks.malloc_fn is NULL\n");
	} else {
		printf("hooks.malloc_fn is not NULL\n");
	}

	/*
	 * cJSON_Delete(cJSON * c)
	 *
	 * Delete a cJSON structure.
	 */
	cJSON_Delete(json_obj);

	return 0;
}