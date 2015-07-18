#ifndef GAME_GRID_CRITTERS_H
#define GAME_GRID_CRITTERS_H
#include "minig.h"
#include "gc_gridlings.h"


void run_game__grid_critters(void);

extern uint gc_shader;
void shader_grid_critters_create(void);

#define VOID_POSITION -5.0
#define MAX_MOVE_SPEED 2.f / GRID_SIZE

void terrain_init(void);
void terrain_quit(void);
extern texture_t* terrain_tex;
extern sprite_t* terrain_sprite;
enum terrain_e {
    TERR_DIRT, TERR_ROCK, TERR_RUBBLE, TERR_UNBREAK,
    TERR_GRASS, TERR_WATER, TERR_LAVA, TERR_PLASMA
}; //32pix
#define NUM_TERR_X 16u
#define NUM_TERR_Y 9u
extern uint terrain_set;

bool terrain_collision(uint x, uint y);


void particle_init(void);
void particle_quit(void);
extern texture_t* particle_tex;
extern sprite_t* particle_sprite;
enum particles_e {
    PART_PLASMA1, PART_PLASMA2, PART_STAR, PART_PLASMA3, PART_RING, PART_PLASMA4,
    PART_SUN, PART_BALL, PART_PLAY1, PART_PLASMA5, PART_PLAY2, PART_GREEN,
    PART_RAINBOW, PART_PORTAL
}; //64pix
#define NUM_PART_X 32u
#define NUM_PART_Y 14u

void get_tile_particle(enum particles_e type, uint* x, uint* y);
extern float env_particle_move;

void singletons_spawn(void);

extern uint level_x;
extern uint level_y;
void change_level(uint lev_x, uint lev_y);

#endif
