#include "utils.h"

#include <stdlib.h>
#include <string.h>

Array array_create(size_t element_size) {
        Array array;
        array.element_size = element_size;
        array.count        = 0;
        array.data         = malloc(array.element_size * GX_ARRAY_BASE_COUNT);
        array.capacity     = GX_ARRAY_BASE_COUNT;
        return array;
}

void array_resize(Array* array, size_t capacity) {
        void* new_ptr   = malloc(capacity * array->element_size);
        array->capacity = capacity;
        array->count    = array->count <= capacity ? array->count : capacity;
        memcpy(new_ptr, array->data, array->count * array->element_size);
        free(array->data);
        array->data = new_ptr;
}

void array_append(Array* array, void* data) {
        if (array->count >= array->capacity) {
                array_resize(array, array->capacity * GX_ARRAY_RESIZE_FACTOR);
        }
        void* dest = array_at(array, array->count);
        memcpy(dest, data, array->element_size);
        array->count++;
}

void* array_at(Array* array, size_t index) {
        return array->data + index * array->element_size;
}

void array_delete(Array* array) {
        free(array->data);
        array->data = NULL;
}

void array_remove_at(Array* array, size_t index) {
        void* current = array_at(array, index);
        void* next    = array_at(array, index + 1);
        for (int i = index; i < array->count - 1; i++) {
                memcpy(current, next, array->element_size);
                current = next;
                next += array->element_size;
        }
        array->count--;
}
