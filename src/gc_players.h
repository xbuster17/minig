#ifndef GC_PLAYERS_H
#define GC_PLAYERS_H

#include "minig.h"
#include "gc_gridlings.h"
#include "gc_projectiles.h"

#define NUM_MAX_PLAYERS 2

extern float zoom;
extern GLuint uni_cam_loc;
extern GLuint uni_zoom_loc;
extern float cam_x;
extern float cam_y;


extern gridling_t players[NUM_MAX_PLAYERS];

extern GLuint players_vao;
extern float* players_vert;
extern float* players_texc;
extern GLuint players_vert_vbo;
extern GLuint players_texc_vbo;

extern bool player_warp_left;
extern bool player_warp_right;
extern bool player_warp_up;
extern bool player_warp_down;

void players_init(void);
void players_quit(void);

void players_update_data(void);
void players_update_vertices(void);

void player_handle_collision(uint index, float prev_x, float prev_y);
void players_update(void);

#endif
