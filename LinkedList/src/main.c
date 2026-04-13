#include <stdio.h>
#include <stdint.h>
#include <linkedlist.h>

int main(void) {
    /* Create a new dynamic list container */
    ListArray *array = define_list();
    if (!array) {
        fprintf(stderr, "Failed to initialize list\n");
        return 1;
    }

    /* Insert values using type-safe macro (copies data internally) */
    list_append_val(array, 7,    int8_t);
    list_append_val(array, 3.14, double);
    list_append_val(array, 'A',  char);

    /* Insert raw pointer (no copy, no ownership) */
    list_append(array, "Hello", PTR);

    /* Safe typed retrieval */
    int8_t n = *list_get(array, 0, int8_t);
    double d = *list_get(array, 1, double);
    char    c = *list_get(array, 2, char);
    char   *s =  list_get(array, 3, char);

    /* Output stored values */
    printf("array[0] = %d\n", n);
    printf("array[1] = %.2f\n", d);
    printf("array[2] = %c\n", c);
    printf("array[3] = %s\n", s);

    printf("\n--- internal structure ---\n");

    /* Debug view of internal nodes (type + ownership + links) */
    print_array(array);

    /* Free entire structure (including owned memory) */
    free_list(array);

    return 0;
}
