#ifndef TESTS_H
#define TESTS_H

#include "minig.h"

extern float Square[8];
void square_init(void);

void test_draw_texture(texture_t* tex);
void test_sprite(void);
// void test_sprite(entity_t ent);

/*android*/
void test_touch_squares(void);
void test_draw_controls(void);
void test_touch_axes(void);

#endif
