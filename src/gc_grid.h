#ifndef GC_GRID_H
#define GC_GRID_H

#include "minig.h"

extern bool gc__grid_update_flag;

#ifndef GRID_SIZE
#   define GRID_SIZE 100
#endif

extern GLuint grid_vao;
extern float* grid_vert;
extern float* grid_texc;
extern uint* grid_data;
extern uint grid_size;
extern uint grid_size_x;
extern uint grid_size_y;
extern GLuint grid_vert_vbo;
extern GLuint grid_texc_vbo;

void grid_create(void);
void grid_delete(void);

void grid_fill_data(void);

void get_tile(uint i, uint* x,uint* y);

void grid_update_texc_vbo(void);

void grid_renew_data(void);
void grid_update(void);
void grid_generate(void);

#endif
