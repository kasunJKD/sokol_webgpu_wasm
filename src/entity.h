#ifndef ENTITY_H
#define ENTITY_H

/*
* each component/object will be a entity
* each tree added to a stack to the render order
*
* example
* 	group_1
* 		drawQuad
* 		drawText
* 		
*
* 	group_2
* 		drawquad
* 		drawbutton
* */
#include "defines.h"

#define MAX_ENTITIES 2024

typedef enum {
	CIRCLE,
	QUAD,
	TRIANGLE,
	BUTTON,
	TEXT,
} EntityType;

typedef struct {
	bool active;
	size_t id;
	char* name;
	Vec3 local_position;
	Vec3 world_position;
	float local_rotation;

	int child_id;
	int next_sibling_id;

	EntityType type;
} EntityHeader;

typedef struct {
	EntityHeader header;
	float scale;
	Vec2 size;
    	Color color;
} ET_QUAD;

typedef struct {
	EntityHeader* entities[MAX_ENTITIES];
	Arena* arena;
	int count;
}EntityManager;

ET_QUAD* create_entity_quad(EntityManager* em, const char* name, Vec3 position, Vec2 size, Color color);

#endif //ENTITIY_H
