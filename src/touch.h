#ifndef TOUCH_H
#define TOUCH_H
#include "minig.h"

#define NUM_MAX_TOUCHES 10u
#define FINGER_NULL (NUM_MAX_TOUCHES+1)
#define FINGER_RELEASED (FINGER_NULL+1)

typedef enum touch_type_e {
    //buttons
    TOUCH_B0, TOUCH_B1, TOUCH_B2, TOUCH_B3, TOUCH_B4,
    TOUCH_B5, TOUCH_B6, TOUCH_B7, TOUCH_B8, TOUCH_B9,
    //axes
    TOUCH_A0, TOUCH_A1, TOUCH_A2, TOUCH_A3,
} touch_type_t;

typedef struct touch_s {
    bool active;
    touch_type_t type;
    SDL_FingerID id;
    float x, y;
    float dx, dy;
    Uint32 time_start;
    Uint32 time_delta;
    Uint32 time_end;
} touch_t;

#define NUM_MAX_TOUCH_BUTTONS 10
#define NUM_MAX_TOUCH_AXES 4

extern struct Touch_s {
    touch_t* b[NUM_MAX_TOUCH_BUTTONS];
    touch_t* a[NUM_MAX_TOUCH_AXES];
} Touch;

typedef struct touch_conf_s {
    bool active;
    float x0, y0, x1, y1;//area must be rectangle, bot left to top right
    void (*fn)(touch_t*);
} touch_conf_t;

extern struct Touch_conf_s {
    struct touch_conf_s buttons[NUM_MAX_TOUCH_BUTTONS];
    struct touch_conf_s axes[NUM_MAX_TOUCH_AXES];
    float axes_radius;
} Touch_conf;

void touch_init(void);
void touch_down(SDL_Event event);
void touch_motion(SDL_Event event);
void touch_up(SDL_Event event);

void touch_config_default(void);
// static touch_t* get_touch_via_type(touch_type_t type);
// static void touch_update_pointers(void);

#endif
