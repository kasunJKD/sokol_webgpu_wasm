#ifndef RENDERER_H
#define RENDERER_H

#include "entity.h"

typedef struct{
    EntityHeader* entities[MAX_ENTITIES];
    int count;
} RenderGroup;

EntityHeader* PushQuad(EntityManager *em, RenderGroup* group, Vec3 worldPosition, const char* name, Vec2 size, Color color);

#endif //RENDERER_H
