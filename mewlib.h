/*
Mewlib - Free header-only HTTP library for C/C++ - https://github.com/n0emo/mewlib

No warranty implied; use at your own risk.

Do this:
    #define MEWLIB_IMPLEMENTATION
before you include this file in *one* C or C++ file to create the implementation.

   // i.e. it should look like this:
   #include ...
   #include ...
   #include ...
   #define MEWLIB_IMPLEMENTATION
   #include "mewlib.h"

Licence:

    See end of file for license information.
*/

/// @file mewlib.h
/// @author Albert Shefner <usn0emo@gmail.com>
/// @brief Header-only HTTP library for C/C++
#ifndef INCLUDE_MEWLIB_H_
#define INCLUDE_MEWLIB_H_

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_WIN32)
#define MEW_WIN
#elif defined(__APPLE__)
#define MEW_OSX
#else
#define MEW_LINUX
#endif

#if defined(MEW_OSX) || defined(MEW_LINUX)
#define MEW_POSIX
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef uint8_t u8;
typedef int8_t i8;
typedef uint16_t u16;
typedef int16_t i16;
typedef uint32_t u32;
typedef int32_t i32;
typedef uint64_t u64;
typedef int64_t i64;

typedef float f32;
typedef double f64;

typedef uintptr_t usize;
typedef ptrdiff_t isize;

typedef void *mew_allocator_alloc_t(void *data, usize bytes);
typedef void mew_allocator_free_t(void *data, void *ptr);
typedef void *mew_allocator_calloc_t(void *data, usize count, usize size);
typedef void *mew_allocator_realloc_t(void *data, void *ptr, usize old_size, usize new_size);

typedef struct {
    mew_allocator_alloc_t *const alloc;
    mew_allocator_free_t *const free;
    mew_allocator_calloc_t *const calloc;
    mew_allocator_realloc_t *const realloc;
} MewAllocatorFunctionTable;

typedef struct MewAllocator {
    void *data;
    const MewAllocatorFunctionTable *ftable;
} MewAllocator;

void mew_allocator_init(MewAllocator *alloc, void *data, const MewAllocatorFunctionTable *ftable);
MewAllocator mew_allocator_create(void *data, const MewAllocatorFunctionTable *ftable);

void *mew_alloc(MewAllocator allocator, usize bytes);
void mew_free(MewAllocator allocator, void *ptr);
void *mew_calloc(MewAllocator allocator, usize count, usize size);
void *mew_realloc(MewAllocator allocator, void *ptr, usize old_size, usize new_size);
char *mew_sprintf(MewAllocator allocator, const char *format, ...) __attribute__((format(printf, 2, 3)));
void *mew_memdup(MewAllocator allocator, const void *mem, usize size);
char *mew_strdup(MewAllocator allocator, const char *s);

mew_allocator_alloc_t mew_malloc_alloc;
mew_allocator_free_t mew_malloc_free;
mew_allocator_calloc_t mew_malloc_calloc;
mew_allocator_realloc_t mew_malloc_realloc;
MewAllocator mew_malloc_allocator_create(void);

typedef struct MewArenaRegion MewArenaRegion;

struct MewArenaRegion {
    MewArenaRegion *next;
    usize count;
    usize capacity;
};

typedef struct {
    MewArenaRegion *begin;
    MewArenaRegion *end;
} MewArena;

mew_allocator_alloc_t mew_arena_alloc;
mew_allocator_free_t mew_arena_free;
mew_allocator_calloc_t mew_arena_calloc;
mew_allocator_realloc_t mew_arena_realloc;
MewAllocator mew_arena_allocator_create(MewArena *arena);

void mew_arena_free_arena(MewArena *arena);
MewArenaRegion *mew_new_region(usize capacity);
void mew_free_region(MewArenaRegion *region);

typedef struct MewVector {
    MewAllocator alloc;
    char *data;
    usize capacity;
    usize count;
    usize element_size;
} MewVector;

void mew_vec_init(MewVector *vec, MewAllocator alloc, size_t element_size);
void mew_vec_destroy(MewVector *vec);
void mew_vec_reserve(MewVector *vec, size_t new_capacity);
char *mew_vec_begin(const MewVector *vec);
char *mew_vec_end(const MewVector *vec);
usize mew_vec_size_bytes(const MewVector *vec);
void *mew_vec_get(MewVector *vec, size_t index);
void mew_vec_push(MewVector *vec, const void *element);
void mew_vec_insert_at(MewVector *vec, const void *element, size_t index);
void mew_vec_delete_at(MewVector *vec, size_t index);
void mew_vec_copy_to(MewVector *dst, const MewVector *src);

typedef uint64_t hashfunc_t(const void *value, void *user_data);
typedef bool hashmap_equals_t(const void *a, const void *b, void *user_data);
typedef bool hashmap_iter_t(const void *key, const void *value, void *user_data);

typedef struct HashMapBucket {
    usize map_index;
    bool initialized;
    char _padding[7];
} HashMapBucket;

typedef struct HashMap {
    MewAllocator alloc;
    hashfunc_t *hashfunc;
    hashmap_equals_t *equals;
    void *user_data;
    size_t key_size;
    size_t value_size;
    size_t bucket_count;
    size_t element_count;
    HashMapBucket *buckets;
} HashMap;

void hashmap_init(
    HashMap *map,
    void *user_data,
    hashfunc_t *hashfunc,
    hashmap_equals_t *equals,
    size_t key_size,
    size_t value_size
);
void hashmap_destroy(HashMap *map);
void hashmap_insert(HashMap *map, const void *key, const void *value);
bool hashmap_pop(HashMap *map, const void *key, void *found_key, void *value);
void *hashmap_get(HashMap *map, const void *key);
bool hashmap_iterate(HashMap *map, hashmap_iter_t iter);

hashfunc_t hashmap_sv_hash;
hashmap_equals_t hashmap_sv_equals;

bool mew_cstr_contains(const char *s, char c);
usize mew_cstr_len(const char *s);

typedef struct MewStringView {
    const char *items;
    usize count;
} MewStringView;

void mew_sv_init(MewStringView *sv, const char *items, usize count);
MewStringView mew_sv_create(const char *items, usize count);
MewStringView mew_sv_identity(MewStringView sv);
bool mew_sv_contains(MewStringView sv, char c);
bool mew_sv_starts_with(MewStringView sv, MewStringView prefix);
bool mew_sv_starts_with_cstr(MewStringView sv, const char *prefix);
MewStringView mew_sv_slice(MewStringView sv, size_t start, size_t count);
MewStringView mew_sv_slice_from(MewStringView sv, size_t index);
MewStringView mew_sv_trim_right_by_cstr(MewStringView sv, const char *chars);
MewStringView mew_sv_trim_left_by_cstr(MewStringView sv, const char *chars);
MewStringView mew_sv_trim_left_to_cstr(MewStringView sv, const char *chars);
MewStringView mew_sv_trim_right_to_cstr(MewStringView sv, const char *chars);
MewStringView mew_sv_trim_space(MewStringView sv);
MewStringView mew_sv_chop_by(MewStringView *sv, char c);
size_t mew_sv_count_char(MewStringView sv, char c);
ptrdiff_t mew_sv_index_char(MewStringView sv, char c);
ptrdiff_t mew_sv_last_index_char(MewStringView sv, char c);
int mew_sv_cmp(MewStringView a, MewStringView b);
bool mew_sv_eq_cstr(MewStringView sv, const char *cstr);
bool mew_sv_eq_sv(MewStringView a, MewStringView b);
MewStringView mew_sv_dup(MewAllocator alloc, MewStringView sv);

typedef struct MewStringBuilder {
    MewVector vec;
} MewStringBuilder;

void mew_sb_init(MewStringBuilder *sb, MewAllocator alloc);
MewStringBuilder mew_sb_create(MewAllocator alloc);
void mew_sb_init_default(MewStringBuilder *sb);
MewStringBuilder mew_sb_new(MewAllocator alloc);
MewStringBuilder mew_sb_new_default(void);
void mew_sb_destroy(MewStringBuilder *sb);
void mew_sb_append_char(MewStringBuilder *sb, char c);
void mew_sb_append_buf(MewStringBuilder *sb, const char *buf, size_t size);
void mew_sb_append_cstr(MewStringBuilder *sb, const char *s);
MewStringBuilder mew_sb_dup(MewAllocator alloc, MewStringBuilder sb);

#define mew_sb_count(sb) (sb)->vec.count
#define mew_sb_begin(sb) (sb)->vec.data
#define mew_sb_append_sv(sb, sv) sb_append_buf(sb, (sv).items, (sv).count)
#define mew_sb_append_sb(sb, other) sb_append_buf(sb, (other).vec.data, (other).vec.count);

#define MEWSB_FMT "%.*s"
#define MEWSB_ARG(sb) (int)(sb_count(&sb)), sb_begin(&sb)

MewStringView mew_cstr_to_sv(const char *cstr);
MewStringView mew_sb_to_sv(MewStringBuilder sb);

typedef enum { LOG_TRACE, LOG_DEBUG, LOG_INFO, LOG_WARN, LOG_ERROR } LogLevel;

void log_init(void);

const char *log_level_str(LogLevel level);
void log_simple(LogLevel level, const char *format, ...) __attribute__((format(printf, 2, 3)));

// TODO: Replace these macros with functions

#ifdef LOG_WITH_FILE
void log_with_file(LogLevel level, const char *file, int line, const char *format, ...)
    __attribute__((format(printf, 4, 5)));
#endif

#ifdef LOG_WITH_FILE
#define LOG_FILENAME (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define log(level, ...) log_with_file(level, LOG_FILENAME, __LINE__, __VA_ARGS__)
#else
#define log(level, ...) log_simple(level, __VA_ARGS__)
#endif

#define log_trace(...) log(LOG_TRACE, __VA_ARGS__)
#define log_debug(...) log(LOG_DEBUG, __VA_ARGS__)
#define log_info(...) log(LOG_INFO, __VA_ARGS__)
#define log_warn(...) log(LOG_WARN, __VA_ARGS__)
#define log_error(...) log(LOG_ERROR, __VA_ARGS__)

typedef struct MewTcpListener MewTcpListener;
typedef struct MewTcpStream MewTcpStream;

/********************************** TcpListener ************************************/

typedef bool(mew_tcplistener_bind_t)(void *data, const char *host, uint16_t port);
typedef bool(mew_tcplistener_listen_t)(void *data, uint32_t max_connections);
typedef bool(mew_tcplistener_accept_t)(void *data, MewTcpStream *stream);
typedef bool(mew_tcplistener_close_t)(void *data);

struct MewTcpListener {
    void *data;

    mew_tcplistener_bind_t *bind;
    mew_tcplistener_listen_t *listen;
    mew_tcplistener_accept_t *accept;
    mew_tcplistener_close_t *close;
};

bool mew_tcplistener_bind(MewTcpListener listener, const char *host, uint16_t port);
bool mew_tcplistener_listen(MewTcpListener listener, uint32_t max_connections);
bool mew_tcplistener_accept(MewTcpListener listener, MewTcpStream *stream);
bool mew_tcplistener_close(MewTcpListener listener);

typedef struct MewNativeTcpListenerOptions {
    uint16_t port;
    bool reuse_address;
} MewNativeTcpListenerOptions;

void mew_tcplistener_init_default_native_options(MewNativeTcpListenerOptions *options);
bool mew_tcplistener_init_native(MewTcpListener *sock, MewNativeTcpListenerOptions options);

/**********************************  TcpStream  ************************************/

typedef bool(mew_tcpstream_set_timeout_t)(void *data, uint32_t seconds);
typedef ptrdiff_t(mew_tcpstream_read_t)(void *data, char *buf, uintptr_t size);
typedef ptrdiff_t(mew_tcpstream_write_t)(void *data, const char *buf, uintptr_t size);
typedef bool(mew_tcpstream_sendfile_t)(void *data, const char *path, uintptr_t size);
typedef bool(mew_tcpstream_close_t)(void *data);

struct MewTcpStream {
    void *data;

    mew_tcpstream_set_timeout_t *set_timeout;
    mew_tcpstream_read_t *read;
    mew_tcpstream_write_t *write;
    mew_tcpstream_sendfile_t *sendfile;
    mew_tcpstream_close_t *close;
};

bool mew_tcpstream_set_timeout(MewTcpStream stream, uint32_t seconds);
ptrdiff_t mew_tcpstream_read(MewTcpStream stream, char *buf, uintptr_t size);
ptrdiff_t mew_tcpstream_write(MewTcpStream stream, const char *buf, uintptr_t size);
bool mew_tcpstream_sendfile(MewTcpStream stream, const char *path, uintptr_t size);
bool mew_tcpstream_close(MewTcpStream stream);
bool mew_tcpstream_write_cstr(MewTcpStream stream, const char *cstr);

// TODO: SSL Socket

#ifdef __cplusplus
}
#endif

#ifdef MEWLIB_IMPLEMENTATION

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#if defined(MEW_POSIX)
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#endif // MEW_POSIX

#if defined(MEW_LINUX)
#include <sys/sendfile.h>
#endif // MEW_LINUX

void mew_allocator_init(MewAllocator *alloc, void *data, const MewAllocatorFunctionTable *ftable) {
    assert(alloc != NULL);
    alloc->data = data;
    alloc->ftable = ftable;
}

MewAllocator mew_allocator_create(void *data, const MewAllocatorFunctionTable *ftable) {
    MewAllocator alloc;
    mew_allocator_init(&alloc, data, ftable);
    return alloc;
}

void *mew_alloc(MewAllocator allocator, size_t bytes) {
    return allocator.ftable->alloc(allocator.data, bytes);
}

void mew_free(MewAllocator allocator, void *ptr) {
    allocator.ftable->free(allocator.data, ptr);
}

void *mew_calloc(MewAllocator allocator, size_t count, size_t size) {
    return allocator.ftable->calloc(allocator.data, count, size);
}

void *mew_realloc(MewAllocator allocator, void *ptr, size_t old_size, size_t new_size) {
    if (new_size <= old_size) {
        return ptr;
    }

    return allocator.ftable->realloc(allocator.data, ptr, old_size, new_size);
}

char *mew_sprintf(MewAllocator allocator, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int size = vsnprintf(NULL, 0, format, args);
    va_end(args);

    assert(size >= 0);
    char *result = (char *)mew_alloc(allocator, (size_t)size + 1);
    va_start(args, format);
    vsnprintf(result, (size_t)size + 1, format, args);
    va_end(args);

    return result;
}

void *mew_memdup(MewAllocator allocator, const void *mem, size_t size) {
    void *new_ptr = mew_alloc(allocator, size);
    memcpy(new_ptr, mem, size);
    return new_ptr;
}

char *mew_strdup(MewAllocator allocator, const char *s) {
    size_t len = strlen(s);
    char *new_ptr = (char *)mew_alloc(allocator, len + 1);
    memcpy(new_ptr, s, len);
    new_ptr[len] = '\0';
    return new_ptr;
}

void *mew_malloc_alloc(void *data, size_t bytes) {
    (void)data;
    return malloc(bytes);
}

void mew_malloc_free(void *data, void *ptr) {
    (void)data;
    free(ptr);
}

void *mew_malloc_calloc(void *data, size_t count, size_t size) {
    (void)data;
    return calloc(count, size);
}

void *mew_malloc_realloc(void *data, void *ptr, size_t old_size, size_t new_size) {
    (void)data;
    (void)old_size;

    return realloc(ptr, new_size);
}

MewAllocator mew_malloc_allocator_create(void) {
    static MewAllocatorFunctionTable MEW_MALLOC_TABLE = {
        mew_malloc_alloc,
        mew_malloc_free,
        mew_malloc_calloc,
        mew_malloc_realloc,
    };

    return mew_allocator_create(NULL, &MEW_MALLOC_TABLE);
}

#define REGION_DEFAULT_CAPACITY (8 * 1024)

void *mew_arena_alloc(void *data, size_t bytes) {
    MewArena *arena = (MewArena *)data;
    size_t size = (bytes + sizeof(uintptr_t) - 1) / sizeof(uintptr_t);

    if (arena->begin == NULL) {
        assert(arena->end == NULL);
        size_t capacity = REGION_DEFAULT_CAPACITY;
        if (capacity < size)
            capacity = size;
        arena->begin = mew_new_region(capacity);
        arena->end = arena->begin;
    }

    while (arena->end->count + size > arena->end->capacity && arena->end->next != NULL) {
        arena->end = arena->end->next;
    }

    if (arena->end->count + size > arena->end->capacity) {
        assert(arena->end->next == NULL);
        size_t capacity = REGION_DEFAULT_CAPACITY;
        if (capacity < size)
            capacity = size;
        arena->end->next = mew_new_region(capacity);
        arena->end = arena->end->next;
    }

    void *result = (&arena->end) + 1 + arena->end->count;
    arena->end->count += size;
    return result;
}

void mew_arena_free(void *data, void *ptr) {
    (void)data;
    (void)ptr;
}

void *mew_arena_calloc(void *data, size_t count, size_t size) {
    void *new_ptr = mew_arena_alloc(data, count * size);
    if (new_ptr) {
        memset(new_ptr, 0, count * size);
    }

    return new_ptr;
}

void *mew_arena_realloc(void *data, void *ptr, size_t old_size, size_t new_size) {
    void *new_ptr = mew_arena_alloc(data, new_size);
    memcpy(new_ptr, ptr, old_size);

    return new_ptr;
}

MewAllocator mew_arena_allocator_create(MewArena *arena) {
    static const MewAllocatorFunctionTable MEWLIB_ARENA_TABLE = {
        mew_arena_alloc,
        mew_arena_free,
        mew_arena_calloc,
        mew_arena_realloc,
    };

    return mew_allocator_create(arena, &MEWLIB_ARENA_TABLE);
}

void mew_arena_free_arena(MewArena *arena) {
    MewArenaRegion *region = arena->begin;
    while (region != NULL) {
        MewArenaRegion *next = region->next;
        mew_free_region(region);
        region = next;
    }

    arena->begin = NULL;
    arena->end = NULL;
}

MewArenaRegion *mew_new_region(size_t capacity) {
    size_t size = sizeof(MewArenaRegion) + sizeof(uintptr_t) * capacity;
    MewArenaRegion *region = (MewArenaRegion *)malloc(size);

    region->next = NULL;
    region->count = 0;
    region->capacity = capacity;

    return region;
}

void mew_free_region(MewArenaRegion *region) {
    free(region);
}

#define MEW_VEC_INITIAL_CAP 128

static inline size_t mew_vec_new_cap(MewVector *vec) {
    return vec->capacity == 0 ? MEW_VEC_INITIAL_CAP : vec->capacity * 2;
}

void mew_vec_init(MewVector *vec, MewAllocator alloc, size_t element_size) {
    assert(vec != NULL);
    assert(element_size != 0);

    memset(vec, 0, sizeof(*vec));
    vec->alloc = alloc;
    vec->element_size = element_size;
}

void mew_vec_reserve(MewVector *vec, size_t new_capacity) {
    assert(vec != NULL);
    assert(new_capacity != 0);

    if (new_capacity <= vec->capacity) {
        return;
    }

    if (vec->data == NULL) {
        vec->data = (char *)mew_calloc(vec->alloc, new_capacity, vec->element_size);
    } else {
        vec->data =
            // TODO: mem_recalloc
            (char *)
                mew_realloc(vec->alloc, vec->data, vec->capacity * vec->element_size, new_capacity * vec->element_size);
    }

    vec->capacity = new_capacity;
}

char *mew_vec_begin(const MewVector *vec) {
    assert(vec != NULL);

    return vec->data;
}

char *mew_vec_end(const MewVector *vec) {
    assert(vec != NULL);

    return vec->data + mew_vec_size_bytes(vec);
}

usize mew_vec_size_bytes(const MewVector *vec) {
    assert(vec != NULL);

    return vec->count * vec->element_size;
}

void mew_vec_destroy(MewVector *vec) {
    assert(vec != NULL);

    if (vec->data == NULL)
        return;

    mew_free(vec->alloc, vec->data);
}

void *mew_vec_get(MewVector *vec, size_t index) {
    assert(vec != NULL);

    if (index >= vec->count) {
        return NULL;
    }

    return vec->data + index * vec->element_size;
}

void mew_vec_push(MewVector *vec, const void *element) {
    assert(vec != NULL);
    assert(element != NULL);

    if (vec->count == vec->capacity) {
        mew_vec_reserve(vec, mew_vec_new_cap(vec));
    }

    char *ptr = vec->data;
    ptr += vec->count * vec->element_size;
    memcpy(ptr, element, vec->element_size);
    vec->count++;
}

void mew_vec_insert_at(MewVector *vec, const void *element, size_t index) {
    assert(vec != NULL);
    assert(element != NULL);
    assert(index <= vec->count);

    if (index == vec->count) {
        mew_vec_push(vec, element);
        return;
    }

    if (vec->count == vec->capacity) {
        mew_vec_reserve(vec, mew_vec_new_cap(vec));
    }

    for (ptrdiff_t i = (ptrdiff_t)vec->count; i > (ptrdiff_t)index; i--) {
        char *ptr = vec->data;
        ptr += (ptrdiff_t)vec->element_size * i;
        memcpy(ptr, ptr - vec->element_size, vec->element_size);
    }

    char *ptr = vec->data;
    ptr += vec->element_size * index;
    memcpy(ptr, element, vec->element_size);
    vec->count++;
}

void mew_vec_delete_at(MewVector *vec, size_t index) {
    assert(vec != NULL);

    if (index >= vec->count)
        return;

    vec->count--;

    for (size_t i = index; i < vec->count; i++) {
        char *ptr = vec->data;
        ptr += vec->element_size * i;
        memcpy(ptr, ptr + vec->element_size, vec->element_size);
    }
}

void mew_vec_copy_to(MewVector *dst, const MewVector *src) {
    assert(src != NULL);
    assert(dst != NULL);
    assert(src->element_size == dst->element_size);

    mew_vec_reserve(dst, dst->count + src->count);
    memcpy(mew_vec_end(dst), mew_vec_begin(src), mew_vec_size_bytes(src));
    dst->count += src->count;
}

#define HASHMAP_INITIAL_BUCKETS 16

size_t round_to(size_t value, size_t roundTo);
HashMapBucket *get_bucket_for_index(HashMap *map, size_t index);
HashMapBucket *get_bucket_for_key(HashMap *map, const void *key);
void alloc_buckets(HashMap *map, size_t count);
void hashmap_expand(HashMap *map);
size_t bucket_size(HashMap *map);
bool map_equals(HashMap *map, const void *a, const void *b);
void *bucket_data(HashMap *map, HashMapBucket *bucket);
void *bucket_key(HashMapBucket *bucket);

void hashmap_init(
    HashMap *map,
    void *user_data,
    hashfunc_t *hashfunc,
    hashmap_equals_t *equals,
    size_t key_size,
    size_t value_size
) {
    map->alloc = mew_malloc_allocator_create();
    map->user_data = user_data;
    map->hashfunc = hashfunc;
    map->equals = equals;
    map->key_size = round_to(key_size, 8);
    map->value_size = round_to(value_size, 8);
    map->element_count = 0;
    alloc_buckets(map, HASHMAP_INITIAL_BUCKETS);
}

void hashmap_destroy(HashMap *map) {
    mew_free(map->alloc, map->buckets);
}

void hashmap_insert(HashMap *map, const void *key, const void *value) {
    if ((float)map->element_count / (float)map->bucket_count >= 0.75) {
        hashmap_expand(map);
    }

    map->element_count++;

    uint64_t hash = map->hashfunc((void *)key, map->user_data);
    size_t index = hash % map->bucket_count;
    size_t map_index = index;
    while (true) {
        HashMapBucket *bucket = get_bucket_for_index(map, index);
        if (!bucket->initialized) {
            // TODO: Robin-hood hashing
            bucket->map_index = map_index;
            memcpy(bucket_key(bucket), key, map->key_size);
            memcpy(bucket_data(map, bucket), value, map->value_size);
            bucket->initialized = true;
            break;
        } else if (map_equals(map, bucket_key(bucket), key)) {
            memcpy(bucket_data(map, bucket), value, map->value_size);
            break;
        }
        index++;
        if (index >= map->bucket_count) {
            index = 0;
        }
    }
}

void *hashmap_get(HashMap *map, const void *key) {
    HashMapBucket *bucket = get_bucket_for_key(map, key);
    if (bucket == NULL) {
        return NULL;
    } else {
        return bucket_data(map, bucket);
    }
}

bool hashmap_pop(HashMap *map, const void *key, void *found_key, void *value) {
    HashMapBucket *bucket = get_bucket_for_key(map, key);
    if (bucket == NULL)
        return false;
    bucket->initialized = false;
    if (found_key != NULL) {
        void *key_ptr = found_key;
        memcpy(key_ptr, bucket_key(bucket), map->key_size);
    }
    if (value != NULL) {
        void *value_ptr = value;
        memcpy(value_ptr, bucket_data(map, bucket), map->value_size);
    }
    return true;
}

bool hashmap_iterate(HashMap *map, hashmap_iter_t iter) {
    for (size_t i = 0; i < map->bucket_count; i++) {
        HashMapBucket *bucket = get_bucket_for_index(map, i);
        if (!bucket->initialized)
            continue;
        if (!iter(bucket_key(bucket), bucket_data(map, bucket), map->user_data))
            return false;
    }
    return true;
}

size_t round_to(size_t value, size_t roundTo) {
    return (value + (roundTo - 1)) & ~(roundTo - 1);
}

HashMapBucket *get_bucket_for_index(HashMap *map, size_t index) {
    size_t size = bucket_size(map);
    return (HashMapBucket *)((char *)map->buckets + size * index);
}

HashMapBucket *get_bucket_for_key(HashMap *map, const void *key) {
    uint64_t hash = map->hashfunc((void *)key, map->user_data);
    size_t index = hash % map->bucket_count;
    size_t map_index = index;
    while (true) {
        HashMapBucket *bucket = get_bucket_for_index(map, index);
        if (!bucket->initialized) {
            return NULL;
        }
        if (bucket->map_index == map_index && map_equals(map, bucket_key(bucket), key)) {
            return bucket;
        }
        index++;
        if (index >= map->bucket_count) {
            index = 0;
        }
    }
}

void alloc_buckets(HashMap *map, size_t count) {
    map->bucket_count = count;
    size_t size = bucket_size(map);
    map->buckets = (HashMapBucket *)mew_alloc(map->alloc, size * count);
    memset(map->buckets, 0, size * count);
}

void hashmap_expand(HashMap *map) {
    HashMapBucket *old_buckets = map->buckets;
    size_t old_count = map->bucket_count;
    size_t size = bucket_size(map);
    alloc_buckets(map, old_count * 2);
    map->element_count = 0;
    for (size_t i = 0; i < old_count; i++) {
        HashMapBucket *bucket = (HashMapBucket *)((char *)old_buckets + size * i);
        if (bucket->initialized) {
            hashmap_insert(map, bucket_key(bucket), bucket_data(map, bucket));
        }
    }
    mew_free(map->alloc, old_buckets);
}

size_t bucket_size(HashMap *map) {
    return round_to(sizeof(HashMapBucket) + map->key_size + map->value_size, 8);
}

bool map_equals(HashMap *map, const void *a, const void *b) {
    return (map->equals != NULL && map->equals(a, b, map->user_data)) || memcmp(a, b, map->key_size) == 0;
}

void *bucket_data(HashMap *map, HashMapBucket *bucket) {
    char *ptr = (char *)(bucket + 1);
    return (void *)(ptr + map->key_size);
}

void *bucket_key(HashMapBucket *bucket) {
    return bucket + 1;
}

// djb2 hash algorithm
uint64_t hashmap_sv_hash(const void *value, void *user_data) {
    (void)user_data;
    const MewStringView *sv = (const MewStringView *)value;
    uint64_t hash = 5381;
    for (size_t i = 0; i < sv->count; i++) {
        hash = ((hash << 5) + hash) + (uint64_t)sv->items[i];
    }
    return hash;
}

bool hashmap_sv_equals(const void *a, const void *b, void *user_data) {
    (void)user_data;
    const MewStringView *sva = (const MewStringView *)a;
    const MewStringView *svb = (const MewStringView *)b;
    return mew_sv_eq_sv(*sva, *svb);
}

bool mew_cstr_contains(const char *s, char c) {
    while (*s != '\0') {
        if (*s == c)
            return true;
        s++;
    }
    return false;
}

usize mew_cstr_len(const char *s) {
    usize result = 0;
    while (*s != '\0') {
        s++;
        result++;
    }
    return result;
}

void mew_sv_init(MewStringView *sv, const char *items, usize count) {
    sv->items = items;
    sv->count = count;
}

MewStringView mew_sv_create(const char *items, usize count) {
    MewStringView sv;
    mew_sv_init(&sv, items, count);
    return sv;
}

MewStringView mew_sv_identity(MewStringView sv) {
    return sv;
}

int mew_sv_cmp(MewStringView a, MewStringView b) {
    size_t min_size = a.count > b.count ? b.count : a.count;

    int cmp = strncmp(a.items, b.items, min_size);
    if (cmp != 0) {
        return cmp;
    }

    if (a.count < b.count) {
        return -1;
    }

    if (a.count > b.count) {
        return 1;
    }

    return 0;
}

bool mew_sv_eq_cstr(MewStringView sv, const char *cstr) {
    return strlen(cstr) == sv.count && strncmp(sv.items, cstr, sv.count) == 0;
}

bool mew_sv_eq_sv(MewStringView a, MewStringView b) {
    if (a.count != b.count)
        return false;
    return memcmp(a.items, b.items, a.count) == 0;
}

bool mew_sv_starts_with(MewStringView sv, MewStringView prefix) {
    if (sv.count < prefix.count)
        return false;
    return strncmp(sv.items, prefix.items, prefix.count) == 0;
}

bool mew_sv_starts_with_cstr(MewStringView sv, const char *prefix) {
    return mew_sv_starts_with(sv, mew_cstr_to_sv(prefix));
}

bool mew_sv_contains(MewStringView sv, char c) {
    for (size_t i = 0; i < sv.count; i++) {
        if (sv.items[i] == c)
            return true;
    }

    return false;
}

MewStringView mew_sv_slice(MewStringView sv, size_t start, size_t count) {
    if (sv.count < start + count) {
        return mew_sv_create(NULL, 0);
    }

    return mew_sv_create(sv.items + start, count);
}

MewStringView mew_sv_slice_from(MewStringView sv, size_t index) {
    if (sv.count < index) {
        return mew_sv_create(NULL, 0);
    }

    return mew_sv_create(sv.items + index, sv.count - index);
}

MewStringView mew_sv_trim_left_by_cstr(MewStringView sv, const char *chars) {
    MewStringView result = sv;

    while (result.count > 0 && strchr(chars, result.items[result.count - 1]) != NULL) {
        result.count--;
    }

    return result;
}

MewStringView mew_sv_trim_right_by_cstr(MewStringView sv, const char *chars) {
    MewStringView result = sv;

    while (result.count > 0 && strchr(chars, result.items[0]) != NULL) {
        result = mew_sv_slice_from(sv, 1);
    }

    return result;
}

MewStringView mew_sv_trim_left_to_cstr(MewStringView sv, const char *chars) {
    MewStringView result = sv;

    while (result.count > 0 && strchr(chars, result.items[result.count - 1]) == NULL) {
        result.count--;
    }

    return result;
}

MewStringView mew_sv_trim_right_to_cstr(MewStringView sv, const char *chars) {
    MewStringView result = sv;

    while (result.count > 0 && strchr(chars, result.items[0]) == NULL) {
        result = mew_sv_slice_from(sv, 1);
    }

    return result;
}

MewStringView mew_sv_trim_space(MewStringView sv) {
    const char *chars = " \r\n\t";
    return mew_sv_trim_right_by_cstr(mew_sv_trim_left_by_cstr(sv, chars), chars);
}

MewStringView mew_sv_chop_by(MewStringView *sv, char c) {
    MewStringView result = mew_sv_create(sv->items, 0);

    while (sv->count > 0 && sv->items[0] != c) {
        result.count++;
        *sv = mew_sv_slice_from(*sv, 1);
    }

    if (sv->count > 0) {
        *sv = mew_sv_slice_from(*sv, 1);
    }

    return result;
}

size_t mew_sv_count_char(MewStringView sv, char c) {
    size_t count = 0;
    for (size_t i = 0; i < sv.count; i++) {
        if (sv.items[i] == c)
            count++;
    }
    return count;
}

ptrdiff_t mew_sv_index_char(MewStringView sv, char c) {
    for (ptrdiff_t i = 0; i < (ptrdiff_t)sv.count; i++) {
        if (sv.items[i] == c)
            return i;
    }
    return -1;
}

ptrdiff_t mew_sv_last_index_char(MewStringView sv, char c) {
    for (ptrdiff_t i = (ptrdiff_t)sv.count - 1; i >= 0; i--) {
        if (sv.items[i] == c)
            return i;
    }
    return -1;
}

MewStringView mew_sv_dup(MewAllocator alloc, MewStringView sv) {
    return mew_sv_create((const char *)mew_memdup(alloc, sv.items, sv.count), sv.count);
}

void mew_sb_init(MewStringBuilder *sb, MewAllocator alloc) {
    mew_vec_init(&sb->vec, alloc, sizeof(char));
}

MewStringBuilder mew_sb_create(MewAllocator alloc) {
    MewStringBuilder sb;
    mew_sb_init(&sb, alloc);
    return sb;
}

void mew_sb_init_default(MewStringBuilder *sb) {
    mew_sb_init(sb, mew_malloc_allocator_create());
}

MewStringBuilder mew_sb_new(MewAllocator alloc) {
    MewStringBuilder sb;
    mew_sb_init(&sb, alloc);
    return sb;
}

MewStringBuilder mew_sb_new_default(void) {
    MewStringBuilder sb;
    mew_sb_init_default(&sb);
    return sb;
}

void mew_sb_destroy(MewStringBuilder *sb) {
    mew_vec_destroy(&sb->vec);
}

void mew_sb_append_char(MewStringBuilder *sb, char c) {
    mew_vec_push(&sb->vec, &c);
}

void mew_sb_append_buf(MewStringBuilder *sb, const char *buf, size_t size) {
    for (size_t i = 0; i < size; i++) {
        mew_sb_append_char(sb, buf[i]);
    }
}

void mew_sb_append_cstr(MewStringBuilder *sb, const char *s) {
    while (*s != '\0') {
        mew_sb_append_char(sb, *s);
        s++;
    }
}

MewStringBuilder mew_sb_dup(MewAllocator alloc, MewStringBuilder sb) {
    MewStringBuilder new_sb = mew_sb_create(alloc);
    mew_vec_copy_to(&new_sb.vec, &sb.vec);
    return new_sb;
}

MewStringView mew_cstr_to_sv(const char *cstr) {
    return mew_sv_create(cstr, mew_cstr_len(cstr));
}

MewStringView mew_sb_to_sv(MewStringBuilder sb) {
    return mew_sv_create(mew_sb_begin(&sb), mew_sb_count(&sb));
}

#if defined(MEW_POSIX)

mew_tcplistener_bind_t mew_tcplistener_native_bind;
mew_tcplistener_listen_t mew_tcplistener_native_listen;
mew_tcplistener_accept_t mew_tcplistener_native_accept;
mew_tcplistener_close_t mew_tcplistener_native_close;

mew_tcpstream_set_timeout_t mew_tcpstream_native_set_timeout;
mew_tcpstream_read_t mew_tcpstream_native_read;
mew_tcpstream_write_t mew_tcpstream_native_write;
mew_tcpstream_sendfile_t mew_tcpstream_native_sendfile;
mew_tcpstream_close_t mew_tcpstream_native_close;

bool mew_tcplistener_bind(MewTcpListener listener, const char *host, uint16_t port) {
    return listener.bind(listener.data, host, port);
}

bool mew_tcplistener_listen(MewTcpListener listener, uint32_t max_connections) {
    return listener.listen(listener.data, max_connections);
}

bool mew_tcplistener_accept(MewTcpListener listener, MewTcpStream *stream) {
    return listener.accept(listener.data, stream);
}

bool mew_tcplistener_close(MewTcpListener listener) {
    return listener.close(listener.data);
}

void mew_tcplistener_init_default_native_options(MewNativeTcpListenerOptions *options) {
    memset(options, 0, sizeof(*options));
    options->reuse_address = true;
}

bool mew_tcplistener_init_native(MewTcpListener *listener, MewNativeTcpListenerOptions options) {
    bool result = true;

    int sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd == -1) {
        log_error("Error creating socker: %s", strerror(errno));
        result = false;
        goto err;
    }

    if (options.reuse_address) {
        int option = 1;
        int ret = setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
        if (ret == -1) {
            log_error("Error creating socket: %s", strerror(errno));
            result = false;
            goto err;
        }
    }

    listener->data = (void *)(uintptr_t)sd;
    listener->bind = &mew_tcplistener_native_bind;
    listener->listen = &mew_tcplistener_native_listen;
    listener->accept = &mew_tcplistener_native_accept;
    listener->close = &mew_tcplistener_native_close;
    return true;

err:
    if (sd != -1) {
        close(sd);
    }
    return result;
}

bool mew_tcplistener_native_bind(void *data, const char *host, uint16_t port) {
    int ret;
    int sd = (int)(uintptr_t)data;

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET, addr.sin_port = htons(port),

    ret = inet_pton(AF_INET, host, &addr.sin_addr);
    if (ret != 1) {
        log_error("Invalid address: %s", strerror(errno));
        return false;
    }

    ret = bind(sd, (struct sockaddr *)&addr, sizeof(addr));
    if (ret == -1) {
        log_error("Error binding socket: %s", strerror(errno));
        return false;
    }

    return true;
}

bool mew_tcplistener_native_listen(void *data, uint32_t max_connections) {
    int sd = (int)(uintptr_t)data;
    int ret = listen(sd, (int)max_connections);
    if (ret == -1) {
        log_error("Error listening: %s", strerror(errno));
        return false;
    } else {
        return true;
    }
}

bool mew_tcplistener_native_accept(void *data, MewTcpStream *stream) {
    int sd = (int)(uintptr_t)data;
    int stream_sd = accept(sd, NULL, NULL);
    if (stream_sd == -1)
        return false;
    memset(stream, 0, sizeof(*stream));
    stream->data = (void *)(uintptr_t)stream_sd;
    stream->set_timeout = &mew_tcpstream_native_set_timeout;
    stream->read = &mew_tcpstream_native_read;
    stream->write = &mew_tcpstream_native_write;
    stream->sendfile = &mew_tcpstream_native_sendfile;
    stream->close = &mew_tcpstream_native_close;
    return true;
}

bool mew_tcplistener_native_close(void *data) {
    int sd = (int)(uintptr_t)data;
    if (sd == -1)
        return true;
    return (close(sd) == 0);
}

bool mew_tcpstream_native_set_timeout(void *data, uint32_t seconds) {
    int sd = (int)(uintptr_t)data;
    struct timeval tv;
    tv.tv_sec = seconds;
    tv.tv_usec = 0;
    int ret = setsockopt(sd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof tv);
    return ret != -1;
}

ptrdiff_t mew_tcpstream_native_read(void *data, char *buf, uintptr_t size) {
    int sd = (int)(uintptr_t)data;
    return recv(sd, buf, size, 0);
}

ptrdiff_t mew_tcpstream_native_write(void *data, const char *buf, uintptr_t size) {
    int sd = (int)(uintptr_t)data;
    return write(sd, buf, size);
}

bool mew_tcpstream_native_sendfile(void *data, const char *path, uintptr_t size) {
    int sd = (int)(uintptr_t)data;
    int body_fd = open(path, O_RDONLY);
    if (body_fd < 0)
        return false;

#ifdef __APPLE__
    off_t offset = (off_t)size;
    int ret = sendfile(body_fd, sd, 0, &offset, NULL, 0);
    if (close(body_fd) < 0 || ret != 0)
        return false;
#else
    ptrdiff_t ret = sendfile(sd, body_fd, NULL, size);
    if (close(body_fd) < 0 || ret != (ssize_t)size)
        return false;
#endif

    return true;
}

bool mew_tcpstream_native_close(void *data) {
    int sd = (int)(uintptr_t)data;
    return close(sd) == 0;
}

bool mew_tcpstream_set_timeout(MewTcpStream stream, uint32_t seconds) {
    return stream.set_timeout(stream.data, seconds);
}

ptrdiff_t mew_tcpstream_read(MewTcpStream stream, char *buf, uintptr_t size) {
    return stream.read(stream.data, buf, size);
}

ptrdiff_t mew_tcpstream_write(MewTcpStream stream, const char *buf, uintptr_t size) {
    return stream.write(stream.data, buf, size);
}

bool mew_tcpstream_sendfile(MewTcpStream stream, const char *path, uintptr_t size) {
    return stream.sendfile(stream.data, path, size);
}

bool mew_tcpstream_close(MewTcpStream stream) {
    return stream.close(stream.data);
}

bool mew_tcpstream_write_cstr(MewTcpStream stream, const char *cstr) {
    return stream.write(stream.data, cstr, mew_cstr_len(cstr));
}

#endif // MEW_POSIX

#endif // MEWLIB_IMPLEMENTATION

#endif // INCLUDE_MEWLIB_H_
