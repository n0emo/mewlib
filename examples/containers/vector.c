#include <mew/containers/vector.h>
#include <stdio.h>

mew_vec_define_for_type(int);

static void print_vec(MewVector_int vec);

int main(void) {
    MewVector_int vec;
    mew_vec_int_init(&vec, new_malloc_allocator());
    printf("Created vec:\n");
    print_vec(vec);

    printf("Reserved space for 10 elements:\n");
    mew_vec_int_reserve(&vec, 10);
    print_vec(vec);

    for (int i = 0; i < 10; i++) {
        mew_vec_int_push(&vec, i * 4);
    }
    print_vec(vec);

    mew_vec_int_insert_at(&vec, 111, 0);
    mew_vec_int_insert_at(&vec, 222, 6);
    mew_vec_int_insert_at(&vec, 333, 12);
    print_vec(vec);

    mew_vec_int_delete_at(&vec, 0);
    mew_vec_int_delete_at(&vec, 5);
    mew_vec_int_delete_at(&vec, 10);
    print_vec(vec);

    mew_vec_int_destroy(&vec);

    return 0;
}

static void print_vec(MewVector_int vec) {
    printf(
        "Vector:\n"
        "  count   : %zu\n"
        "  capacity: %zu\n"
        "  data_ptr: %p\n"
        "  elements: [",
        vec.inner.count,
        vec.inner.capacity,
        vec.inner.data
    );

    for (size_t i = 0; i < vec.inner.count; i++) {
        printf("%d", mew_vec_int_get(&vec, i));
        if (i != vec.inner.count - 1) {
            printf(", ");
        }
    }

    printf("]\n\n");
}
