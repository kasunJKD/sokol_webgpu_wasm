#ifndef DEFINES_H
#define DEFINES_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>


#define KB(x) ((size_t)(x) * 1024ULL)
#define MB(x) ((size_t)(x) * 1024ULL * 1024ULL)

typedef struct {
    float x, y;
} Vec2;

typedef struct {
    float x, y, z;
} Vec3;

typedef struct {
    float r, g, b, a;
} Color;


//arena
//
//
//
#define ArenaPushStruct(arena, type) \
    (type*)ArenaPush((arena), sizeof(type))

#define ArenaPushArray(arena, type, count) \
    (type*)ArenaPush((arena), sizeof(type) * (count))

#define ArenaPushZeroStruct(arena, type) \
    (type*)arena_push_zero((arena), sizeof(type))

#define ArenaPushZeroArray(arena, type, count) \
    (type*)arena_push_zero((arena), sizeof(type) * (count))

typedef struct {
    unsigned char* base;
    size_t size;
    size_t used;
} Arena;

static void ArenaInit(Arena* a, void* memory, size_t size) {
    a->base = (unsigned char*)memory;
    a->size = size;
    a->used = 0;
}

static void* ArenaPush(Arena* a, size_t size) {
    if (a->used + size > a->size) return NULL; // out of memory
    void* ptr = a->base + a->used;
    a->used += size;
    return ptr;
}
static void* arena_push_zero(Arena* a, size_t size) {
    void* ptr = ArenaPush(a, size);
    if (ptr) memset(ptr, 0, size);
    return ptr;
}
static void ArenaReset(Arena* a) {
    a->used = 0;
}

static Arena create_arena(void* memory, size_t size_in_bytes) {
    Arena arena = {0};
    ArenaInit(&arena, memory, size_in_bytes);
    return arena;
}

#endif //DEFINES_H
