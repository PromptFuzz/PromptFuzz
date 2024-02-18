#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <vector>


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
    // DeclStmt
    //{
        cJSON *array, *item;
        cJSON *object = NULL;
        const char *string = "hello", *next_string = "world";
        const char *long_string = "{\"name\":\"John\", \"age\":30}";
        const char char_array[] = {'a', 'b', 'c', 'd'};
        const char char_matrix[][2] = {{'a', 'b'}, {'c', 'd'}};
        const char *strings_array[] = {"hello", "world"};
        const float flt_array[4] = {3.14f, -1.0f, 2.0f, 7.5f};
        const float flt_matrix[][2] = {{1.1, 2.2},{3.3, 4.4}};
        const float *float_star_array[2] = {(const float *)(float [2]){1.1, 2.2}, (const float *)(float [2]){3.3, 4.4}};
        const double dob_arr_data[5] = {1.1, 2.2, 3.3, 4.4, 5.5};
        const int i=0, numbers[5] = {1, 2, 3, 4, 5};
        const int *int_arr_list[2] = {(int *)(int [2]){1, 2}, (int *)(int [2]){3, 4}};
        const int matrix[3][4] = {{1,2,3,4}, {5,6,7,8}, {9,10,11,12}}; 
        cJSON *root = cJSON_CreateObject();
        char *str = cJSON_GetStringValue(root);
    //}

    // BinaryOperator
    //{
        root = cJSON_CreateNull();
        object = cJSON_Parse((const char *)data);
        cJSON_CreateString("example");
        cJSON_CreateIntArray((int[]) {1, 2, 3}, 3);
        double d_numbers[] = {1.0, 2.0, 3.0};
        cJSON *double_array = cJSON_CreateDoubleArray(d_numbers, 3);
        float floats[] = {1.0, 2.0, 3.0};
        cJSON_CreateFloatArray(floats, sizeof(floats)/sizeof(float));
        object->next = NULL;
        item = root;
    //}

}
// clang++ -fsyntax-only -Xclang -ast-dump=json -Xclang -ast-dump-filter=main -I/data/home/loydlv/vbd/llm_fuzz/output/build/cjson/include test_dfa.cc > test_dfa.json