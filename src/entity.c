#include "minig.h"

void entity_update_sprite(entity_t* ent, uint x, uint y) {
    uint maxx = ent->sprite->count_x;
    uint maxy = ent->sprite->count_y;
    uint mult = ent->sprite->pixs;
    // float x0,y0, x1, y1;
    float fmultx = (float)mult / (float)ent->texture->x;
    float fmulty = (float)mult / (float)ent->texture->y;
    ent->tex_coord[0] = ((x % maxx)   ) * fmultx;
    ent->tex_coord[1] = ((y % maxy)   ) * fmulty;
    ent->tex_coord[2] = ((x % maxx) +1) * fmultx;
    ent->tex_coord[3] = ((y % maxy)   ) * fmulty;
    ent->tex_coord[4] = ((x % maxx)   ) * fmultx;
    ent->tex_coord[5] = ((y % maxy) +1) * fmulty;
    ent->tex_coord[6] = ((x % maxx) +1) * fmultx;
    ent->tex_coord[7] = ((y % maxy) +1) * fmulty;
}



static bool particle_created = false;
static texture_t* ent_particle_tex = NULL;
static sprite_t* ent_particle_sprite = NULL;

entity_t* entity_create_particle(void) {
    if (!particle_created) {
        ent_particle_tex = texture_create("particles_sprite_sheet_x2_smooth.png");
        ent_particle_sprite = malloc(sizeof(sprite_t));
        ent_particle_sprite->count_x = 32;
        ent_particle_sprite->count_y = 11;
        // ent_particle_sprite->pixs = 16*2;
        ent_particle_sprite->pixs = 32;

        particle_created = true;
    }
    entity_t* ent = malloc(sizeof(entity_t));
    ent->pos_x = 0.0;
    ent->pos_y = 0.0;
    ent->texture = ent_particle_tex;
    ent->sprite = ent_particle_sprite;
    ent->sprite_x = 0;
    ent->sprite_y = 0;
    glGenBuffers(1, &ent->tex_coord_buffer);

    return ent;
}

void entity_delete(entity_t* ent){
    glDeleteBuffers(1, &ent->tex_coord_buffer);
    free(ent);
    ent = NULL;
}

void entity_particle_init(void) {
    if (!particle_created) {
        ent_particle_tex = texture_create("particles_sprite_sheet_x2_smooth.png");
        ent_particle_sprite = malloc(sizeof(sprite_t));
        ent_particle_sprite->count_x = 32;
        ent_particle_sprite->count_y = 11;
        ent_particle_sprite->pixs = 16*2;

        particle_created = true;
    }
}

void entity_particle_quit(void) {
    if (particle_created) {
        texture_delete(ent_particle_tex);
        ent_particle_tex = NULL;
        free(ent_particle_sprite);
        ent_particle_sprite = NULL;
        particle_created = false;
    }
}
