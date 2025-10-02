#include "renderer.h"
#include "entity.h"
#include "defines.h"
#include "entity.h"

RenderGroup create_render_group() {
    RenderGroup g = {0};
    g.count = 0;
    return g;
}

EntityHeader* PushQuad(EntityManager *em, RenderGroup* group, Vec3 worldPosition, const char* name, Vec2 size, Color color) {
   ET_QUAD* quad = create_entity_quad(em, name, worldPosition, size, color);
    if (!quad) return NULL;

    // just push into group
    group->entities[group->count++] = (EntityHeader*)quad;

    return (EntityHeader*)quad;
}
