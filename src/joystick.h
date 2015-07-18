#ifndef JOYSTICK_H
#define JOYSTICK_H

#define NUM_MAX_JOYSTICKS 4
#define NUM_MAX_JOYSTICK_BUTTONS 12
#define NUM_MAX_JOYSTICK_AXIS 4

typedef struct joystick_s {
    int id;
    bool buttons[NUM_MAX_JOYSTICK_BUTTONS];
    float a0,a1,a2,a3;
} joystick_t;

extern joystick_t joysticks[NUM_MAX_JOYSTICKS];

void joy_axismot(SDL_Event event);
void joy_button_down(SDL_Event event);
void joy_button_up(SDL_Event event);
void joy_dev_add(SDL_Event event);
void joy_dev_rem(SDL_Event event);

#endif
