#include "renderer.h"
#include "entity.h"
#include "defines.h"
#include "entity.h"

#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_glue.h" 

RenderSystem create_render_system() {
   RenderSystem s = {0};
   s.count = 0;
   return s;
}

RenderGroup create_render_group(RenderSystem* system) {
    RenderGroup g = {0};
    g.count = 0;

    system->groups[system->count] = &g;
    system->count++;
    return g;
}
//Push rendables to stack================
EntityHeader* PushQuad(EntityManager *em, RenderGroup* group, Vec3 worldPosition, const char* name, Vec2 size, Color color) {
   ET_QUAD* quad = create_entity_quad(em, name, worldPosition, size, color);
    if (!quad) return NULL;

    // just push into group
    group->entities[group->count++] = (EntityHeader*)quad;

    return (EntityHeader*)quad;
}

//=========================================
// sokol render functions
//========================================
static void _draw_quad(ET_QUAD* q) {
    if (!q || !q->header.active) return;

    // Setup vertex data
    float x = q->header.world_position.x;
    float y = q->header.world_position.y;
    float w = q->size.x;
    float h = q->size.y;

    // Vertex data (x, y, color)
    float vertices[] = {
        x,     y,     q->color.r, q->color.g, q->color.b, q->color.a,
        x + w, y,     q->color.r, q->color.g, q->color.b, q->color.a,
        x + w, y + h, q->color.r, q->color.g, q->color.b, q->color.a,
        x,     y + h, q->color.r, q->color.g, q->color.b, q->color.a,
    };

    sg_apply_bindings(&(sg_bindings){});
    sg_apply_uniforms(0, &(sg_range){ vertices, sizeof(vertices) });
    sg_draw(0, 6, 1);
}

//Update system==================================================
void render_update(RenderSystem* system) {
    if (!system) return;
   sg_pass_action pass_action;
   pass_action = (sg_pass_action) {
        .colors[0] = {
            .load_action = SG_LOADACTION_CLEAR,
            .clear_value = { 1.0f, 0.0f, 0.0f, 1.0f }
        }
    };

   sg_begin_pass(&(sg_pass){ .action = pass_action, .swapchain = sglue_swapchain() });


    // Iterate over all groups
    for (int gi = 0; gi < system->count; gi++) {
        RenderGroup* group = system->groups[gi];
        if (!group) continue;

        for (int ei = 0; ei < group->count; ei++) {
            EntityHeader* e = group->entities[ei];
            if (!e || !e->active) continue;

            switch (e->type) {
               case QUAD:
                    _draw_quad((ET_QUAD*)e);
                    break;

                // case ENTITY_SPRITE:
                //     draw_sprite((ET_SPRITE*)e);
                //     break;

                default:
                    break;
            }
        }
    }

    sg_end_pass();
    sg_commit();
}
