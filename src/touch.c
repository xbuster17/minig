#include "minig.h"

// Touch_t Touch;
struct Touch_s Touch;
struct Touch_conf_s Touch_conf;

static void touch_update_pointers(void); // updates 'Touch' global

void touch_init(void) {
    Uint8 i;
    for (i=0; i < NUM_MAX_TOUCHES; i++) {
        Mi.touches[i].id = FINGER_NULL;
        Mi.touches[i].type = -1;
        Mi.touches[i].active = false;
    }
    touch_config_default();
    touch_update_pointers();
}

void def_touch_handler(touch_t* touch){}

#define conf_set(button,ix0,iy0,ix1,iy1){\
    button.active = true;\
    button.x0 = ix0; button.y0 = iy0;\
    button.x1 = ix1; button.y1 = iy1;\
    button.fn = &def_touch_handler;\
}
void touch_config_default(void) {
    conf_set(Touch_conf.buttons[0], 0.4, 0.0, 0.5, 0.1);
    conf_set(Touch_conf.buttons[1], 0.5, 0.0, 0.6, 0.1);
    conf_set(Touch_conf.buttons[2], 0.0, 0.4, 0.1, 0.5);
    conf_set(Touch_conf.buttons[3], 0.9, 0.4, 1.0, 0.5);

    conf_set(Touch_conf.buttons[4], 0.5, 0.9, 0.6, 1.0);
    conf_set(Touch_conf.buttons[5], 0.4, 0.9, 0.5, 1.0);
    conf_set(Touch_conf.buttons[6], 0.9, 0.5, 1.0, 0.6);
    conf_set(Touch_conf.buttons[7], 0.0, 0.5, 0.1, 0.6);

    conf_set(Touch_conf.buttons[8], 0.9, 0.0, 1.0, 0.2);
    Touch_conf.buttons[8].active = false;
    conf_set(Touch_conf.buttons[9], 0.0, 0.8, 0.1, 1.0);
    Touch_conf.buttons[9].active = false;

    // conf_set(Touch_conf.axes[0], 0.0, 0.0, 0.5, 0.5);//down left
    conf_set(Touch_conf.axes[0], 0.0, 0.0, 0.5, 1.0);//down left
    // conf_set(Touch_conf.axes[1], 0.5, 0.0, 1.0, 0.5);//down right
    conf_set(Touch_conf.axes[1], 0.5, 0.0, 1.0, 1.0);//down right
    // conf_set(Touch_conf.axes[2], 0.5, 0.5, 1.0, 1.0);//up right
    // conf_set(Touch_conf.axes[3], 0.0, 0.5, 0.5, 1.0);//up left

    Touch_conf.axes_radius = 0.05;
}
#undef conf_set

void touch_set_area(touch_conf_t* touch, float ix0, float iy0, float ix1, float iy1) {
    touch->x0 = ix0;
    touch->y0 = iy0;
    touch->x1 = ix1;
    touch->y1 = iy1;
}
void touch_set_function(touch_conf_t* touch, void(*fn)(touch_t*)) {
    touch->fn = fn;
}

static touch_t t_null = {0,FINGER_NULL,0,0,0,0,0,0,0,0};

static touch_t* get_touch_via_type(touch_type_t type) {
    if (type > TOUCH_A3) return &t_null;

    touch_t* axis_candidate = NULL;
    Uint8 i;

    for (i=0; i < NUM_MAX_TOUCHES; i++) {
        if (Mi.touches[i].type == type) {
            if (type < TOUCH_A0)
                if (&Mi.touches[i].active)
                    return &Mi.touches[i];
                else continue;

            else if (!axis_candidate) {
                if (&Mi.touches[i].active)
                    axis_candidate = &Mi.touches[i];
                else continue;
            }

            else if (axis_candidate->time_start < Mi.touches[i].time_start) {
                if (&Mi.touches[i].active)
                    axis_candidate = &Mi.touches[i];
                else continue;
            }
        }
    }

    if (axis_candidate)
        return axis_candidate;
    else return &t_null;
}

static void touch_set_type(touch_t* touch) {
    if (touch->id == FINGER_NULL) return;

    Uint8 i; float x,y;
    touch_conf_t* tc;

    x = touch->x;
    y = touch->y;

    tc = Touch_conf.buttons;

    for (i=0; i < NUM_MAX_TOUCH_BUTTONS; i++){
        if (tc[i].active){
            if (
                x > tc[i].x0 &&
                x < tc[i].x1 &&
                y > tc[i].y0 &&
                y < tc[i].y1
            ) {
                touch->type = TOUCH_B0 + i;
                return;
            }
        }
    }

    tc = Touch_conf.axes;

    for (i=0; i < NUM_MAX_TOUCH_AXES; i++){
        if (tc[i].active){
            if (
                x > tc[i].x0 &&
                x < tc[i].x1 &&
                y > tc[i].y0 &&
                y < tc[i].y1
            ) {
                touch->type = TOUCH_A0 + i;

                return;
            }
        }
    }
    return;
}

static void touch_update_pointers(void){
    Uint8 i;
    for (i = 0; i < NUM_MAX_TOUCH_BUTTONS; i++) {
        Touch.b[i] = get_touch_via_type(TOUCH_B0 + i);
        if (!Touch.b[i]->active) {
            Touch.b[i] = &t_null;
        }
    }
    for (i = 0; i < NUM_MAX_TOUCH_AXES; i++) {
        Touch.a[i] = get_touch_via_type(TOUCH_A0 + i);
        if (!Touch.a[i]->active) {
            Touch.a[i] = &t_null;
        }
    }
}
// static touch_t touch;

void touch_down(SDL_Event event) {
    if (event.tfinger.fingerId >= NUM_MAX_TOUCHES)
        return;

    touch_t touch;

    touch.active = true;
    touch.id = event.tfinger.fingerId;
    touch.x = event.tfinger.x;
    touch.y = 1.0 - event.tfinger.y;// REVERSE Y-AXIS !!!
    touch.time_start = event.tfinger.timestamp;
    touch.dx = touch.dy = 0;

    Mi.touches[touch.id] = touch;

    touch_set_type(&Mi.touches[touch.id]);

    touch_update_pointers();
}

static float raw_dx[NUM_MAX_TOUCHES];
static float raw_dy[NUM_MAX_TOUCHES];

void touch_motion(SDL_Event event) {
    Uint8 id = event.tfinger.fingerId;
    if (id >= NUM_MAX_TOUCHES) return;

    // Mi.touches[id].x = event.tfinger.x;
    // Mi.touches[id].y = 1.0 - event.tfinger.y;

    raw_dx[id] += event.tfinger.dx * (1.0 / Touch_conf.axes_radius * ((float)Mi.sx / (float)Mi.sy));

    if (raw_dx[id] > 1.0)
        Mi.touches[id].dx = 1.0;

    else if (raw_dx[id] < -1.0)
        Mi.touches[id].dx = -1.0;
    else Mi.touches[id].dx = raw_dx[id];

    raw_dy[id] -= event.tfinger.dy * (1.0 / Touch_conf.axes_radius);  // REVERSE Y-AXIS !!!

    if (raw_dy[id] > 1.0)
        Mi.touches[id].dy = 1.0;

    else if (raw_dy[id] < -1.0)
        Mi.touches[id].dy = -1.0;
    else Mi.touches[id].dy = raw_dy[id];

    Mi.touches[id].time_delta =
        event.tfinger.timestamp - Mi.touches[id].time_start;
}

void touch_up(SDL_Event event) {
    uint id = event.tfinger.fingerId;
    if (id >= NUM_MAX_TOUCHES) return;

    Mi.touches[id].active = false;
    Mi.touches[id].id = FINGER_RELEASED;

    Mi.touches[id].time_end = event.tfinger.timestamp;
    Mi.touches[id].x = event.tfinger.x;
    Mi.touches[id].y = 1.0 - event.tfinger.y; // REVERSE Y-AXIS !!!
    Mi.touches[id].dx = 0;
    Mi.touches[id].dy = 0;

    raw_dx[id] = 0.0;
    raw_dy[id] = 0.0;
    touch_update_pointers();
}
