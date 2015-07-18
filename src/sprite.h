#ifndef SPRITE_H
#define SPRITE_H
#include "minig.h"

typedef struct sprite_s {
    uint count_x;
    uint count_y;
    uint pixs;
} sprite_t;

void sprite_create(void);
void sprite_delete(void);

#endif
