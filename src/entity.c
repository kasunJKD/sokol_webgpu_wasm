#include "entity.h"
#include <string.h>

void create_entity_manager(EntityManager* em, Arena* arena_)
{
    if (!em) return;

    memset(em->entities, 0, sizeof(em->entities));

    em->count = 0;
    em->arena = arena_;
}

ET_QUAD* create_entity_quad(EntityManager* em, const char* name, Vec3 position, Vec2 size, Color color)
{
    if (!em || !em->arena || em->count >= MAX_ENTITIES) return NULL;

    // Allocate a quad entity from the arena
    ET_QUAD* quad = ArenaPush(em->arena, sizeof(ET_QUAD));
    if (!quad) return NULL;

    // Setup header
    quad->header.active = true;
    quad->header.id = em->count;
    quad->header.name = NULL; 
    if (name) {
        size_t len = strlen(name) + 1;
        char* n = ArenaPush(em->arena, len);
        if (n) {
            memcpy(n, name, len);
            quad->header.name = n;
        }
    }

    quad->header.local_position = position;
    quad->header.world_position = position;
    quad->header.local_rotation = 0.0f;
    quad->header.child_id = -1;
    quad->header.next_sibling_id = -1;
    quad->header.type = QUAD;

    // Quad-specific
    quad->scale = 1.0f;
    quad->size = size;
    quad->color = color;

    // Register in entity manager
    em->entities[em->count++] = (EntityHeader*)quad;

    return quad;
}
