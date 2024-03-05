#include "FDSan.h"
#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 285
//<Prompt> ["cJSON_CreateBool","cJSON_InsertItemInArray","cJSON_ReplaceItemInObjectCaseSensitive","cJSON_IsTrue","cJSON_CreateStringArray"]
/*<Combination>: [cJSON *cJSON_CreateBool(cJSON_bool boolean),
    cJSON_bool cJSON_InsertItemInArray(cJSON * array, int which, cJSON * newitem),
    cJSON_bool cJSON_ReplaceItemInObjectCaseSensitive(cJSON * object, const char * string, cJSON * newitem),
    cJSON_bool cJSON_IsTrue(const cJSON *const item),
    cJSON *cJSON_CreateStringArray(const char *const * strings, int count)
*/
//<score> 20, nr_unique_branch: 1
//<Quality> {"density":20,"unique_branches":{"cJSON_CreateBool":[[2423,22,2423,29,0,0,4,1]]},"library_calls":["cJSON_CreateObject","cJSON_CreateObject","cJSON_CreateString","cJSON_CreateString","cJSON_CreateNumber","cJSON_CreateNumber","cJSON_CreateBool","cJSON_CreateNumber","cJSON_AddItemToObject","cJSON_AddItemToObject","cJSON_AddItemToObject","cJSON_AddItemToObject","cJSON_AddItemToObject","cJSON_AddItemToObject","cJSON_AddItemToObject","cJSON_Print","cJSON_PrintUnformatted","cJSON_Delete","cJSON_free","cJSON_free"],"critical_calls":["cJSON_CreateObject","cJSON_CreateObject","cJSON_CreateString","cJSON_CreateString","cJSON_CreateNumber","cJSON_CreateNumber","cJSON_CreateBool","cJSON_CreateNumber","cJSON_AddItemToObject","cJSON_AddItemToObject","cJSON_AddItemToObject","cJSON_AddItemToObject","cJSON_AddItemToObject","cJSON_AddItemToObject","cJSON_AddItemToObject","cJSON_Print","cJSON_PrintUnformatted","cJSON_Delete","cJSON_free","cJSON_free"],"visited":1}
/**/

extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t * data, size_t size) {
	if(size<0) return 0;

    // Create cJSON objects
    cJSON *root = cJSON_CreateObject();
    cJSON *format = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("Jack (\"Bee\") Nimble");
    cJSON *type = cJSON_CreateString("rect");
    cJSON *width = cJSON_CreateNumber(1920);
    cJSON *height = cJSON_CreateNumber(1080);
    cJSON *interlace = cJSON_CreateBool(false);
    cJSON *frameRate = cJSON_CreateNumber(24);

    // Add items to the format object
    cJSON_AddItemToObject(format, "type", type);
    cJSON_AddItemToObject(format, "width", width);
    cJSON_AddItemToObject(format, "height", height);
    cJSON_AddItemToObject(format, "interlace", interlace);
    cJSON_AddItemToObject(format, "frame rate", frameRate);

    // Add items to the root object
    cJSON_AddItemToObject(root, "name", name);
    cJSON_AddItemToObject(root, "format", format);

    // Print the cJSON objects
    char *rootStr = cJSON_Print(root);
    char *rootUnformattedStr = cJSON_PrintUnformatted(root);

    // Release resources
    cJSON_Delete(root);
    cJSON_free(rootStr);
    cJSON_free(rootUnformattedStr);

    return 0;
}