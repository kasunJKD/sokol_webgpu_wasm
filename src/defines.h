#ifndef DEFINES_H
#define DEFINES_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

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

static void ArenaReset(Arena* a) {
    a->used = 0;
}

#endif //DEFINES_H
