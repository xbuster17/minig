#ifndef ENTITY_H
#define ENTITY_H
#include "minig.h"

typedef struct entity_s {
    float pos_x, pos_y;
    float size_x, size_y;

    texture_t* texture;
    sprite_t* sprite;
    uint sprite_x;
    uint sprite_y;
    float tex_coord[8];
    GLuint tex_coord_buffer;
} entity_t;

entity_t* entity_create_particle(void);
void entity_particle_init(void);
void entity_particle_quit(void);

void entity_update_sprite(entity_t* ent, uint x, uint y);


void entity_delete(entity_t* ent);


#endif
