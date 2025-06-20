#include <mew/containers/vector.h>

#include <stdint.h>

#include <mew/unit.h>

bool is_uint64_t_array_equals(MewVector *vec, uint64_t *array) {
    for (size_t i = 0; i < vec->count; i++) {
        uint64_t *item = mew_vec_get(vec, i);
        if (*item != array[i]) {
            return false;
        }
    }

    return true;
}

TEST(vector_init, {
    MewVector vector;
    mew_vec_init(&vector, new_malloc_allocator(), sizeof(uint64_t));

    mewassert("Vector should be empty", vector.count == 0);
    mewassert("Vector should be empty", vector.capacity == 0);
    mewassert("Vector should be empty", vector.data == NULL);
    mewassert("Vector should have correct element size", vector.element_size == sizeof(uint64_t));

    mew_vec_destroy(&vector);
})

TEST(vector_push, {
    MewVector vector;
    mew_vec_init(&vector, new_malloc_allocator(), sizeof(uint64_t));

    for (size_t i = 1; i <= 1024; i++) {
        mew_vec_push(&vector, (void *)&i);
    }

    mewassert("Vector should have 3 elements", vector.count == 1024);
    mewassert("Vector should have some capacity", vector.capacity != 0);
    mewassert("Vector should have valid data pointer", vector.data != NULL);

    mew_vec_destroy(&vector);
})

TEST(vector_reserve, {
    MewVector vector;
    mew_vec_init(&vector, new_malloc_allocator(), sizeof(uint64_t));
    mew_vec_reserve(&vector, 10);

    mewassert("Vector should be empty", vector.count == 0);
    mewassert("Vector should have capacity equals to 10 after first reserve", vector.capacity == 10);
    mewassert("Vector should have valid data pointer", vector.data != NULL);

    mew_vec_reserve(&vector, 20);
    mewassert("Vector should have capacity equals to 20 after second reserve", vector.capacity == 20);

    mew_vec_reserve(&vector, 15);
    mewassert("Vector reserve does nothing when invoked with new_capacity less then actual", vector.capacity == 20);

    mew_vec_destroy(&vector);
})

TEST(vector_get, {
    MewVector vector;
    mew_vec_init(&vector, new_malloc_allocator(), sizeof(uint64_t));

    for (size_t i = 1; i <= 1024; i++) {
        mew_vec_push(&vector, (void *)&i);
    }

    for (size_t i = 0; i < vector.count; i++) {
        uint64_t *item = mew_vec_get(&vector, i);
        mewassert("Item should not be NULL if index is valid", item != NULL);
        mewassert("Item should be valid", *item == i + 1);
    }

    uint64_t *invalid = mew_vec_get(&vector, 10000);
    mewassert("Item should be NULL if index is invalid", invalid == NULL);

    mew_vec_destroy(&vector);
})

TEST(vector_insert_at, {
    MewVector vector;
    mew_vec_init(&vector, new_malloc_allocator(), sizeof(uint64_t));

    for (uint64_t i = 0; i < 5; i++) {
        mew_vec_push(&vector, (void *)&i);
    }

    uint64_t item = 111;
    mew_vec_insert_at(&vector, &item, 0);
    mew_vec_insert_at(&vector, &item, 3);
    mew_vec_insert_at(&vector, &item, 7);

    mewassert(
        "Vector should be valid after insert_at operations",
        is_uint64_t_array_equals(&vector, (uint64_t[]) {111, 0, 1, 111, 2, 3, 4, 111})
    );

    mew_vec_destroy(&vector);
})

TEST(vector_insert_at_the_end, {
    MewVector vector;
    mew_vec_init(&vector, new_malloc_allocator(), sizeof(uint64_t));

    for (uint64_t i = 0; i < 1024; i++) {
        mew_vec_insert_at(&vector, (void *)&i, i);
    }

    mewassert("Vector should have 1024 elements", vector.count == 1024);

    mew_vec_destroy(&vector);
})

TEST(vector_insert_at_the_beginning, {
    MewVector vector;
    mew_vec_init(&vector, new_malloc_allocator(), sizeof(uint64_t));

    for (uint64_t i = 0; i < 1024; i++) {
        mew_vec_insert_at(&vector, (void *)&i, 0);
    }

    mewassert("Vector should have 1024 elements", vector.count == 1024);

    mew_vec_destroy(&vector);
})

TEST(vector_delete_at, {
    MewVector vector;
    mew_vec_init(&vector, new_malloc_allocator(), sizeof(uint64_t));

    for (uint64_t i = 0; i < 5; i++) {
        mew_vec_push(&vector, (void *)&i);
    }

    mew_vec_delete_at(&vector, 0);
    mewassert(
        "Vector's first element should be deleted successfully",
        is_uint64_t_array_equals(&vector, (uint64_t[]) {1, 2, 3, 4})
    );

    mew_vec_delete_at(&vector, 3);
    mewassert(
        "Vector's last element should be deleted successfully",
        is_uint64_t_array_equals(&vector, (uint64_t[]) {1, 2, 3})
    );

    mew_vec_delete_at(&vector, 1);
    mewassert(
        "Vector's middle element should be deleted successfully",
        is_uint64_t_array_equals(&vector, (uint64_t[]) {1, 3})
    );

    mew_vec_delete_at(&vector, 123);
    mewassert(
        "Vector delete_at with invalid index does nothing",
        is_uint64_t_array_equals(&vector, (uint64_t[]) {1, 3})
    );

    mew_vec_destroy(&vector);
})

TEST(vector_copy_to, {
    MewVector src;
    mew_vec_init(&src, new_malloc_allocator(), sizeof(u64));
    for (u64 i = 0; i < 10; i++) {
        mew_vec_push(&src, &i);
    }

    MewVector dst;
    mew_vec_init(&dst, new_malloc_allocator(), sizeof(u64));

    mew_vec_copy_to(&dst, &src);
    mewassert("After copy empty vec should now have the same count as source", dst.count == src.count);
    mewassert("After copy empty vec should now have the same capacity as source", dst.capacity == src.count);
    mewassert(
        "After copy all elements should be copied correctly",
        is_uint64_t_array_equals(&dst, (u64[]) {0, 1, 2, 3, 4, 5, 6, 7, 8, 9})
    );

    mew_vec_copy_to(&dst, &src);
    mewassert("After copy dst vec should now have the same count as twice the source", dst.count == src.count * 2);
    mewassert(
        "After copy dst vec should now have the same capacity as twice the source",
        dst.capacity == src.count * 2
    );
    mewassert(
        "After copy all elements should be copied correctly",
        is_uint64_t_array_equals(&dst, (u64[]) {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9})
    );
})

// TEST: mew_vec_define_for_type

mewtest_main()
