#include "cJSON/cJSON.h"

cJSON *parse(char data[]) {
    cJSON *root = cJSON_Parse(data);
    if(root == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if(error_ptr != NULL) {
            return NULL;
        }
    }

    return root;
}