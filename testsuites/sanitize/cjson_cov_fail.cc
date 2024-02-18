#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 0
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
    // Parse the input data and create a cJSON object
    cJSON *root = cJSON_ParseWithLength((const char*)data, size);
    if(root == NULL) {
        return 0;
    }
    
    // Check if the root object has a "format" object
    cJSON *formatObj = cJSON_GetObjectItem(root, "format");
    if(formatObj != NULL && cJSON_IsObject(formatObj)) {
        // Get the width and height from the "format" object
        cJSON *widthObj = cJSON_GetObjectItem(formatObj, "width");
        cJSON *heightObj = cJSON_GetObjectItem(formatObj, "height");
        
        if(widthObj != NULL && heightObj != NULL && cJSON_IsNumber(widthObj) && cJSON_IsNumber(heightObj)) {
            int width = widthObj->valueint;
            int height = heightObj->valueint;
            
            // Print the width and height
            printf("Width: %d, Height: %d\n", width, height);
        }
    }
    
    // Check if the root object has an array called "name"
    cJSON *nameArray = cJSON_GetObjectItem(root, "name");
    if(nameArray != NULL && cJSON_IsArray(nameArray)) {
        // Get the size of the array
        int nameArraySize = cJSON_GetArraySize(nameArray);
        printf("Name Array Size: %d\n", nameArraySize);
    }
    
    // Check if the root object is invalid
    if(cJSON_IsInvalid(root)) {
        printf("Invalid cJSON object\n");
    }

    // Print the cJSON object
    char *printBuffer = cJSON_PrintBuffered(root, 256, true);
    printf("Printed cJSON Object: %s\n", printBuffer);
    cJSON_free(printBuffer);
    
    // Delete the cJSON object
    cJSON_Delete(root);
    
    return 0;
}
/*
Coverage Error: 
The program cannot cover the callees along the path that contains maximum callees.
LLVMFuzzerTestOneInput:
   19|    201|extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
   20|       |    // Parse the input data and create a cJSON object
   21|    201|    cJSON *root = cJSON_ParseWithLength((const char*)data, size);
   22|    201|    if(root == NULL) {
   23|    112|        return 0;
   24|    112|    }
   25|       |    
   26|       |    // Check if the root object has a "format" object
   27|     89|    cJSON *formatObj = cJSON_GetObjectItem(root, "format");
   28|     89|    if(formatObj != NULL && cJSON_IsObject(formatObj)) {
   29|       |        // Get the width and height from the "format" object
   30|      9|        cJSON *widthObj = cJSON_GetObjectItem(formatObj, "width");
   31|      9|        cJSON *heightObj = cJSON_GetObjectItem(formatObj, "height");
   32|       |        
   33|      9|        if(widthObj != NULL && heightObj != NULL && cJSON_IsNumber(widthObj) && cJSON_IsNumber(heightObj)) {
   34|      1|            int width = widthObj->valueint;
   35|      1|            int height = heightObj->valueint;
   36|       |            
   37|       |            // Print the width and height
   38|      1|            printf("Width: %d, Height: %d\n", width, height);
   39|      1|        }
   40|      9|    }
   41|       |    
   42|       |    // Check if the root object has an array called "name"
   43|     89|    cJSON *nameArray = cJSON_GetObjectItem(root, "name");
   44|     89|    if(nameArray != NULL && cJSON_IsArray(nameArray)) {
   45|       |        // Get the size of the array
   46|      0|        int nameArraySize = cJSON_GetArraySize(nameArray);
   47|      0|        printf("Name Array Size: %d\n", nameArraySize);
   48|      0|    }
   49|       |    
   50|       |    // Check if the root object is invalid
   51|     89|    if(cJSON_IsInvalid(root)) {
   52|      0|        printf("Invalid cJSON object\n");
   53|      0|    }
   54|       |
   55|       |    // Print the cJSON object
   56|     89|    char *printBuffer = cJSON_PrintBuffered(root, 256, true);
   57|     89|    printf("Printed cJSON Object: %s\n", printBuffer);
   58|     89|    cJSON_free(printBuffer);
   59|       |    
   60|       |    // Delete the cJSON object
   61|     89|    cJSON_Delete(root);
   62|       |    
   63|     89|    return 0;
   64|    201|}
*/