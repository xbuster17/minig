#ifndef GC_GRIDLINGS_H
#define GC_GRIDLINGS_H

// #include "minig.h"
#include "gc_grid.h"
#include "gc_projectiles.h"

#define NUM_MAX_GRIDLINGS 1000

enum gridling_type_e {
    GRIDLING_NULL = -1,
    GRIDLING_P1, GRIDLING_P2,
    GRIDLING_E1, GRIDLING_E1_SPAWNER, GRIDLING_E2, GRIDLING_E2_SPAWNER,
    GRIDLING_E3, GRIDLING_E3_SPAWNER, GRIDLING_E4, GRIDLING_E4_SPAWNER,
    GRIDLING_EXIT
};
void gridling_spawn(enum gridling_type_e type, float pos_x, float pos_y);

typedef struct gridling_s {
    enum gridling_type_e type;
    uint ix;
    uint iy;
    float fx;
    float fy;
    float size; //?

    float dir_x;
    float dir_y;

    float speed;
    float hp;
    float mp;

    float power;

    uint sprite_set;
    uint sprite_frame;

    uint tick; //ai =p
} gridling_t;

#include "gc_players.h"

// #define NUM_MAX_PLAYERS 1



void gridlings_init(void);
void gridlings_quit(void);
void gridlings_update(void);

extern GLuint gridlings_vao;
extern GLuint gridlings_vert_vbo;
extern GLuint gridlings_texc_vbo;


void gridlings_update_data(void);
void gridlings_update_vertices(void);

void gridlings_ai_tick(void);
// void singletons_spawn(void);
void gridlings_stats_set(void);



// extern gridling_t players[NUM_MAX_PLAYERS];

extern gridling_t gridlings[NUM_MAX_GRIDLINGS];
extern float* gridlings_vert;
extern float* gridlings_texc;
extern uint num_active_gridlings;

void gridling_handle_collision(gridling_t* array, uint i,
        float prev_x, float prev_y);

//stats
struct gridling_type_stats_s {
    float p1_speed;
    float p1_hp;
    float p1_mp;
    uint p1_sprite_set;
    uint p2_sprite_set;
    float e1_speed;
    float e1_hp;
    float e1_mp;
    uint e1_sprite_set;
    float e2_speed;
    float e2_hp;
    float e2_mp;
    uint e2_sprite_set;
    float e3_speed;
    float e3_hp;
    float e3_mp;
    uint e3_sprite_set;
    float e4_speed;
    float e4_hp;
    float e4_mp;
    uint e4_sprite_set;

    float exit_sprite_set;
} gridling_type_stats;

void gridling_set_default(gridling_t* gridling);

#endif
