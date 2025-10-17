#ifndef RENDERER_H
#define RENDERER_H

#include "defines.h"
#include "entity.h"

typedef struct{
    EntityHeader* entities[MAX_ENTITIES];
    int count;
} RenderGroup;

typedef struct {
    RenderGroup* groups[MAX_GROUPS];
    int count;
} RenderSystem;

EntityHeader* PushQuad(EntityManager *em, RenderGroup* group, Vec3 worldPosition, const char* name, Vec2 size, Color color);


void render_update(RenderSystem* system);

RenderSystem* create_render_system(Arena* arena);

#endif //RENDERER_H
