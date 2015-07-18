#ifndef EVENTS_H
#define EVENTS_H

#include "minig.h"
#include "touch.h"
#include "joystick.h"

extern float Accel[3];

extern struct mouse {
    int x;
    int y;
    bool b1;
    bool b2;
    bool b3;
} Mouse;

// #define NUM_MAX_CONTROLLERS (NUM_MAX_JOYSTICK_BUTTONS + NUM_MAX_JOYSTICK_AXIS + 4 + 1) /*touches+touchbutton*/
// typedef enum controller_type_e {
//     CTP1L, CTP1R, CTP2L, CTP2R, CTBUTTON,
//     CONTROLLER_TOUCH_AXIS_1,
//     CONTROLLER_TOUCH_AXIS_2,
//     CONTROLLER_TOUCH_AXIS_3,
//     CONTROLLER_TOUCH_AXIS_4,
// } controller_type_t;

void update_kbkeys(void);

bool kbk(SDL_Keycode keycode);
void reset_kbk(SDL_Keycode keycode);

void poll_events(void);
#endif
