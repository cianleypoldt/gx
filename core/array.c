#include "utils.h"

#include <stdlib.h>
#include <string.h>

Array array_create(size_t element_size) {
        Array array = { .element_size = element_size,
                        .count        = 0,
                        .capacity     = GX_ARRAY_BASE_COUNT,
                        .data = malloc(element_size * GX_ARRAY_BASE_COUNT) };
        return array;
}

void array_resize(Array* array, size_t capacity) {
        void* new_ptr = malloc(capacity * array->element_size);
        if (array->count > capacity) {
                array->count = capacity;
        }
        memcpy(new_ptr, array->data, array->count * array->element_size);
        free(array->data);
        array->data     = new_ptr;
        array->capacity = capacity;
}

void array_append(Array* array, const void* data) {
        if (array->count >= array->capacity) {
                array_resize(array, array->capacity * GX_ARRAY_RESIZE_FACTOR);
        }
        memcpy(array_at(array, array->count), data, array->element_size);
        array->count++;
}

void* array_at(const Array* array, size_t index) {
        return (unsigned char*) array->data + index * array->element_size;
}

void array_delete(Array* array) {
        free(array->data);
        array->data = NULL;
}

void array_remove_at(Array* array, size_t index) {
        if (index >= array->count) {  // against overflow of size_t bytes
                return;
        }
        void*  src   = array_at(array, index + 1);
        void*  dst   = array_at(array, index);
        size_t bytes = (array->count - index - 1) * array->element_size;
        if (bytes > 0) {
                memmove(dst, src, bytes);
        }
        array->count--;
        if (array->count < array->capacity / (GX_ARRAY_RESIZE_FACTOR *
                                              GX_ARRAY_RESIZE_FACTOR) &&
            array->capacity > GX_ARRAY_BASE_COUNT) {
                array_resize(array, array->capacity / GX_ARRAY_RESIZE_FACTOR);
        }
}
