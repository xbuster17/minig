#include "minig.h"

joystick_t joysticks[NUM_MAX_JOYSTICKS];

void joy_axismot(SDL_Event event) {
    printf("%i\n", event.type);
}

void joy_button_down(SDL_Event event) {
    printf("%i\n", event.type);
}

void joy_button_up(SDL_Event event) {
    printf("%i\n", event.type);
}

void joy_dev_add(SDL_Event event) {
    printf("%i\n", event.type);
}

void joy_dev_rem(SDL_Event event) {
    printf("%i\n", event.type);
}

