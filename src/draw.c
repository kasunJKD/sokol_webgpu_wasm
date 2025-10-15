#include "draw.h"
#include "renderer.h"
#include "entity.h"

void draw_quad(EntityManager* em, RenderGroup* group,
               Vec3 position, Vec2 size, Color color, const char* name)
{
    // high-level API -> just forwards to renderer
    PushQuad(em, group, position, name, size, color);
}

void draw_update(RenderSystem *system) {
    render_update(system);
}
