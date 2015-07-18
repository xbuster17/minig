#ifndef GC_PROJECTILES_H
#define GC_PROJECTILES_H

#include "minig.h"
#include "gc_gridlings.h"

void projectiles_init(void);
void projectiles_quit(void);
#define NUM_MAX_PROJECTILES 1000
#define PROJECTILE_DEF_REACH 0.5

extern float* projectiles_vert;
extern float* projectiles_texc;
extern GLuint projectiles_vao;
extern GLuint projectiles_vert_vbo;
extern GLuint projectiles_texc_vbo;

extern uint num_active_projectiles;

enum projectile_type_e {
    PROJECTILE_NULL = -1,
    PROJECTILE_0, PROJECTILE_1, PROJECTILE_2, PROJECTILE_3, PROJECTILE_4,
    PROJECTILE_5, PROJECTILE_6, PROJECTILE_7, PROJECTILE_8, PROJECTILE_9,
    PROJECTILE_10, PROJECTILE_11, PROJECTILE_12, PROJECTILE_13
};

typedef struct projectile_s {
    bool owned;
    enum projectile_type_e type;
    uint ix;
    uint iy;
    float fx;
    float fy;
    float dir_x;
    float dir_y;

    float speed;
    float power;
    float reach;

    char sprite_set;
    char sprite_frame;
} projectile_t;
//stats
struct projectile_type_stats_s {
    float p0_speed;
    float p0_power;
    char  p0_sprite_set;
    float p1_speed;
    float p1_power;
    char  p1_sprite_set;
    float p2_speed;
    float p2_power;
    char  p2_sprite_set;
    float p3_speed;
    float p3_power;
    char  p3_sprite_set;
    float p4_speed;
    float p4_power;
    char  p4_sprite_set;
    float p5_speed;
    float p5_power;
    char  p5_sprite_set;
    float p6_speed;
    float p6_power;
    char  p6_sprite_set;
    float p7_speed;
    float p7_power;
    char  p7_sprite_set;
    float p8_speed;
    float p8_power;
    char  p8_sprite_set;
    float p9_speed;
    float p9_power;
    char  p9_sprite_set;
    float p10_speed;
    float p10_power;
    char  p10_sprite_set;
    float p11_speed;
    float p11_power;
    char  p11_sprite_set;
    float p12_speed;
    float p12_power;
    char  p12_sprite_set;
} projectile_type_stats;

extern projectile_t projectiles[NUM_MAX_PROJECTILES];


void projectiles_update(void);

void projectile_handle_collision(uint index);
void projectiles_update_data(void);
void projectiles_update_vertices(void);

void projectile_set_default(projectile_t* projectile);
void projectiles_stats_set(void);

void projectile_spawn(bool owned,enum projectile_type_e type, float pos_x, float pos_y,
    float dir_x, float dir_y);

#endif

