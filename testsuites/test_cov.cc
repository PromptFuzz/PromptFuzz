#include "cJSON.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

// only used in test pch and ast test.
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
// Create a FILE pointer to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == NULL) {
        return 0;
    }

    // Parse the input JSON data
    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    char *json_data = (char *)malloc(file_size + 1);
    if (json_data == NULL) {
        fclose(in_file);
        return 0;
    }
    fread(json_data, 1, file_size, in_file);
    fclose(in_file);
    json_data[file_size] = '\0';

    cJSON *root = cJSON_Parse(json_data);
    if (root == NULL) {
        free(json_data);
        return 0;
    }

    // Create an array
    cJSON *array = cJSON_CreateArray();
    if (array != NULL) {
        // Add the array to the root object
        cJSON_AddItemToObject(root, "array", array);

        // Add a number to the array
        cJSON_AddNumberToObject(array, NULL, 1.23);

        // Insert a new item at index 0 in the array
        cJSON *newItem = cJSON_CreateNumber(4.56);
        cJSON_InsertItemInArray(array, 0, newItem);

        // Delete the item at index 1 from the array
        cJSON_DeleteItemFromArray(array, 1);
    }

    // Print the root object
    char *output_data = cJSON_Print(root);
    printf("%s\n", output_data);
    free(output_data);

    // Free the memory and release resources
    free(json_data);
    cJSON_Delete(root);
}
// clang -fsyntax-only -Xclang -ast-dump=json -Xclang -ast-dump-filter=main -I/data/home/loydlv/vbd/llm_fuzz/data/cJSON/include test.c > test_ast.json