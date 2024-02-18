#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 3
//<Prompt> ["cJSON_IsBool","cJSON_PrintBuffered","cJSON_IsNumber","cJSON_GetArraySize","cJSON_IsInvalid"]
/*<Combination>: [cJSON_bool cJSON_IsBool(const cJSON *const item),
    char *cJSON_PrintBuffered(const cJSON * item, int prebuffer, cJSON_bool fmt),
    cJSON_bool cJSON_IsNumber(const cJSON *const item),
    int cJSON_GetArraySize(const cJSON * array),
    cJSON_bool cJSON_IsInvalid(const cJSON *const item)
*/
//<Quality> {"diversity":0,"density":0,"covered_api":[],"uncovered_api":[],"unique_branches":{},"library_calls":[]}
/**/

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Step 1: Event: Parse the input data and check if it is a valid cJSON object
    cJSON *root = cJSON_ParseWithLength((const char *)data, size);
    if (root == NULL) {
        cJSON_Delete(root);
        return 0;
    }

    // Step 2: Event: Check if the cJSON item is a boolean
    cJSON *item = cJSON_GetObjectItem(root, "interlace");
    if (cJSON_IsBool(item)) {
        cJSON_bool isInterlace = cJSON_IsTrue(item);
        printf("interlace: %s\n", isInterlace ? "true" : "false");
    }

    // Step 2: Event: Print the cJSON object with formatting
    char *printBuffer = cJSON_PrintBuffered(root, 100, true);
    printf("%s\n", printBuffer);
    cJSON_free(printBuffer);

    // Step 2: Event: Check if the cJSON item is a number
    item = cJSON_GetObjectItem(root, "width");
    if (cJSON_IsNumber(item)) {
        int width = item->valueint;
        printf("width: %d\n", width);
    }

    // Step 2: Event: Check if the cJSON item is invalid
    item = cJSON_GetObjectItem(root, "frame rate");
    if (cJSON_IsInvalid(item)) {
        printf("Invalid frame rate\n");
    }

    // Step 2: Event: Get the size of the cJSON array
    item = cJSON_GetObjectItem(root, "format");
    if (cJSON_IsObject(item)) {
        cJSON *array = cJSON_GetObjectItem(item, "type");
        int arraySize = cJSON_GetArraySize(array);
        printf("Array size: %d\n", arraySize);
    }

    // Step 3: Consume the input data by cJSON APIs

    // Step 4: Create a FILE * variable using fmemopen
    FILE *file = fmemopen((void *)data, size, "rb");

    // Step 5: Use "input_file" or "output_file" as file names

    // Step 6: Release allocated resources
    cJSON_Delete(root);
    fclose(file);

    return 0;
}
/*
   19|    214|extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
   20|       |    // Step 1: Event: Parse the input data and check if it is a valid cJSON object
   21|    214|    cJSON *root = cJSON_ParseWithLength((const char *)data, size);
   22|    214|    if (root == NULL) {
   23|    114|        cJSON_Delete(root);
   24|    114|        return 0;
   25|    114|    }
   26|       |
   27|       |    // Step 2: Event: Check if the cJSON item is a boolean
   28|    100|    cJSON *item = cJSON_GetObjectItem(root, "interlace");
   29|    100|    if (cJSON_IsBool(item)) {
   30|      2|        cJSON_bool isInterlace = cJSON_IsTrue(item);
   31|      2|        printf("interlace: %s\n", isInterlace ? "true" : "false");
   32|      2|    }
   33|       |
   34|       |    // Step 2: Event: Print the cJSON object with formatting
   35|    100|    char *printBuffer = cJSON_PrintBuffered(root, 100, true);
   36|    100|    printf("%s\n", printBuffer);
   37|    100|    cJSON_free(printBuffer);
   38|       |
   39|       |    // Step 2: Event: Check if the cJSON item is a number
   40|    100|    item = cJSON_GetObjectItem(root, "width");
   41|    100|    if (cJSON_IsNumber(item)) {
   42|      0|        int width = item->valueint;
   43|      0|        printf("width: %d\n", width);
   44|      0|    }
   45|       |
   46|       |    // Step 2: Event: Check if the cJSON item is invalid
   47|    100|    item = cJSON_GetObjectItem(root, "frame rate");
   48|    100|    if (cJSON_IsInvalid(item)) {
   49|      0|        printf("Invalid frame rate\n");
   50|      0|    }
   51|       |
   52|       |    // Step 2: Event: Get the size of the cJSON array
   53|    100|    item = cJSON_GetObjectItem(root, "format");
   54|    100|    if (cJSON_IsObject(item)) {
   55|      4|        cJSON *array = cJSON_GetObjectItem(item, "type");
   56|      4|        int arraySize = cJSON_GetArraySize(array);
   57|      4|        printf("Array size: %d\n", arraySize);
   58|      4|    }
   59|       |
   60|       |    // Step 3: Consume the input data by cJSON APIs
   61|       |
   62|       |    // Step 4: Create a FILE * variable using fmemopen
   63|    100|    FILE *file = fmemopen((void *)data, size, "rb");
   64|       |
   65|       |    // Step 5: Use "input_file" or "output_file" as file names
   66|       |
   67|       |    // Step 6: Release allocated resources
   68|    100|    cJSON_Delete(root);
   69|    100|    fclose(file);
   70|       |
   71|    100|    return 0;
   72|    214|}
*/