#include "../include/linkedlist.h"

ListArray *define_list(void) {
    ListArray *arr = malloc(sizeof(ListArray));
    if (!arr) return NULL;
    arr->nodes = NULL;
    arr->count = 0;
    return arr;
}


LinkedList *list_append(ListArray *arr, void *data, DataType data_type) {
    if (!arr) return NULL;

    LinkedList *node = malloc(sizeof(LinkedList));
    if (!node) return NULL;

    node->data = data;
    node->data_type = data_type;
    node->owns_data = 0;
    node->self_id = node;
    node->next_node_id = NULL;

    // link previous tail to this new node
    if (arr->count > 0)
        arr->nodes[arr->count - 1]->next_node_id = node;

    // grow the pointer array by one slot
    LinkedList **temp = realloc(arr->nodes, sizeof(LinkedList *) * (arr->count + 1));
    if (!temp) {
        free(node);
        return NULL;
    }

    arr->nodes = temp;
    arr->nodes[arr->count] = node;
    arr->count++;

    return node;
}


// heap copies the data so the caller doesn't have to worry about lifetime
LinkedList *list_append_copy(ListArray *arr, void *data, DataType data_type, size_t data_size) {
    void *copy = malloc(data_size);
    if (!copy) return NULL;
    memcpy(copy, data, data_size);

    LinkedList *node = list_append(arr, copy, data_type);
    if (!node) {
        free(copy);
        return NULL;
    }

    node->owns_data = 1;
    return node;
}


void *list_get_safe(ListArray *arr, size_t index) {
    if (!arr || index >= arr->count) {
        fprintf(stderr, "ERROR: index %zu out of bounds (count: %zu)\n",
                index, arr ? arr->count : 0);
        return NULL;
    }
    return arr->nodes[index]->data;
}


void print_array(ListArray *arr) {
    if (!arr) return;
    for (size_t i = 0; i < arr->count; i++) {
        printf("array[%zu] -> [%p] | data_type: %d | owns_data: %d | next: %p\n",
            i,
            (void *)arr->nodes[i],
            arr->nodes[i]->data_type,
            arr->nodes[i]->owns_data,
            (void *)arr->nodes[i]->next_node_id);
    }
}


void free_list(ListArray *arr) {
    if (!arr) return;
    for (size_t i = 0; i < arr->count; i++) {
        if (arr->nodes[i]->owns_data)
            free(arr->nodes[i]->data);
        free(arr->nodes[i]);
    }
    free(arr->nodes);
    free(arr);
}
