#include "utils.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

array gx_array_create(size_t element_size) {
        array array;
        array.element_size = element_size;
        array.count        = 0;
        array.data         = malloc(array.element_size * GX_ARRAY_BASE_COUNT);
        array.capacity     = GX_ARRAY_BASE_COUNT;
        return array;
}

void gx_array_resize(array* array, size_t capacity) {
        void* new_ptr   = malloc(capacity * array->element_size);
        array->capacity = capacity;
        array->count    = fmin(array->count, capacity);
        memcpy(new_ptr, array->data, array->count * array->element_size);
        free(array->data);
        array->data = new_ptr;
}

void gx_array_appand(array* array, void* data) {
        if (array->count >= array->capacity) {
                gx_array_resize(array,
                                array->capacity * GX_ARRAY_RESIZE_FACTOR);
        }
        void* dest = gx_array_at(array, array->count);
        memcpy(dest, data, array->element_size);
        array->count++;
}

void* gx_array_at(array* array, size_t index) {
        return array->data + index * array->element_size;
}

void gx_array_delete(array* array) {
        free(array->data);
}
