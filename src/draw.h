#ifndef DRAW_H
#define DRAW_H

#include "renderer.h"
#include "entity.h"

void draw_quad(EntityManager* em, RenderGroup* group,
               Vec3 position, Vec2 size, Color color, const char* name);

#endif // DRAW_H
