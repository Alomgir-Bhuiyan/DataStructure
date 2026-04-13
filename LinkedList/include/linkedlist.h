#ifndef _LINKED_LIST_H
#define _LINKED_LIST_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * ============================================================
 * LinkedList Library (Generic Type Container)
 * ============================================================
 *
 * This library implements a dynamically growing list structure
 * capable of storing heterogeneous data types using void* and
 * runtime type tagging.
 *
 * Features:
 *  - Generic value storage (int, float, char, pointers, etc.)
 *  - Heap-managed ownership tracking
 *  - Safe indexed access (array-like)
 *  - Value-based insertion and pointer-based insertion
 *
 * ============================================================
 */

/**
 * Type descriptor for stored values.
 * Each enum represents size/category of stored primitive type.
 *
 * NOTE:
 * PTR assumes pointer size (platform dependent in real systems).
 */
typedef enum {
    INT8   = 1,
    INT16  = 2,
    INT    = 4,
    INT64  = 8,
    FLOAT  = 4,
    DOUBLE = 8,
    CHAR   = 1,
    PTR    = 4,
} DataType;

/**
 * Node structure representing a single element in the list.
 *
 * Members:
 *  - data: pointer to stored value (heap or external)
 *  - data_type: runtime type tag for debugging/safety
 *  - owns_data: indicates if node owns allocated memory
 *  - self_id: self-reference pointer (debug/identity use)
 *  - next_node_id: linked-list style next pointer
 *
 * NOTE:
 * Structure is packed to avoid compiler-added padding.
 */
typedef struct LinkedList {
    void *data;
    DataType data_type;
    int owns_data;
    struct LinkedList *self_id;
    struct LinkedList *next_node_id;
} __attribute__((packed)) LinkedList;

/**
 * List container structure.
 *
 * Members:
 *  - nodes: dynamic array of pointers to LinkedList nodes
 *  - count: number of elements in the list
 *
 * This allows O(1) index access while still maintaining
 * linked structure internally.
 */
typedef struct {
    LinkedList **nodes;
    size_t count;
} ListArray;

/* ============================================================
 * MACROS
 * ============================================================
 */

/**
 * Convert C type to internal DataType enum at compile time.
 *
 * Uses _Generic to deduce type and map it to DataType.
 */
#define TO_DATATYPE(type) _Generic((type){0}, \
    int8_t:  INT8,                             \
    int16_t: INT16,                            \
    int:     INT,                              \
    int64_t: INT64,                            \
    float:   FLOAT,                            \
    double:  DOUBLE,                           \
    char:    CHAR,                             \
    char*:   PTR,                              \
    void*:   PTR                               \
)

/**
 * Insert a value directly into the list.
 *
 * Automatically:
 *  - Creates temporary value
 *  - Copies it into heap memory
 *  - Stores ownership in node
 *
 * Parameters:
 *  arr  - target list
 *  val  - value to insert
 *  type - C type of value (int, float, etc.)
 */
#define list_append_val(arr, val, type) \
    list_append_copy((arr), &(type){(val)}, TO_DATATYPE(type), sizeof(type))

/**
 * Retrieve element from list by index with type cast.
 *
 * NOTE:
 * No runtime type safety guarantee; caller must ensure correctness.
 */
#define list_get(arr, index, type) \
    ((type *)list_get_safe((arr), (index)))

/* ============================================================
 * FUNCTION DECLARATIONS
 * ============================================================
 */

/**
 * Create and initialize a new empty list.
 *
 * Returns:
 *  Pointer to allocated ListArray structure.
 */
ListArray *define_list(void);

/**
 * Append a raw pointer into the list.
 *
 * WARNING:
 * Does NOT copy data. Caller must ensure pointer lifetime.
 *
 * Parameters:
 *  arr        - target list
 *  data       - pointer to existing memory
 *  data_type  - runtime type descriptor
 *
 * Returns:
 *  Pointer to created node or NULL on failure.
 */
LinkedList *list_append(ListArray *arr, void *data, DataType data_type);

/**
 * Append a value by copying it into heap memory.
 *
 * This function:
 *  - Allocates memory
 *  - Copies provided data into heap
 *  - Marks node as owning the memory
 *
 * Parameters:
 *  arr        - target list
 *  data       - pointer to source data
 *  data_type  - runtime type descriptor
 *  data_size  - size of data in bytes
 *
 * Returns:
 *  Pointer to created node or NULL on failure.
 */
LinkedList *list_append_copy(ListArray *arr, void *data, DataType data_type, size_t data_size);

/**
 * Safely retrieve data pointer at given index.
 *
 * Performs bounds checking.
 *
 * Returns:
 *  Pointer to stored data or NULL if invalid index.
 */
void *list_get_safe(ListArray *arr, size_t index);

/**
 * Print internal structure of list for debugging.
 *
 * Displays:
 *  - node index
 *  - node address
 *  - type
 *  - ownership flag
 *  - next pointer
 */
void print_array(ListArray *arr);

/**
 * Free entire list and all owned memory.
 *
 * Frees:
 *  - Each node
 *  - Each owned data block
 *  - Node array
 *  - List container
 */
void free_list(ListArray *arr);

#endif
