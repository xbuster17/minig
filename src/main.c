#include "minig.h"

// extern tex_t Tex[NUM_MAX_TEX];
// void tex_init(char* file_name){
//     Tex[i] = tex_create(tex_name);
// }

void main_loop_start (void) {
    Mi.running = 1;

    while(Mi.running) {
        if (Mi.touches[9].id != FINGER_NULL)
        glClearColor(Mi.touches[9].id, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        swap_buffers();
        poll_events();

        if (kbk(SDLK_ESCAPE) || kbk(SDLK_MENU) ||
                kbk(SDLK_AC_BACK) || kbk(SDLK_AC_BACK)
        ) Mi.running = false;

    }

}

int main(int argc, char** argv) {

    printf("%i:%s:\n", argc, argv[0]);

    minig_init();

/*GRID_CRITTERS*/
    run_game__grid_critters();
/*GRID_CRITTERS*/

    // main_loop_start();
    // test_audio_sequencer();
    // test_draw_controls();

    // Mi.running = true;
    // test_touch_axes();

    // void* tex = NULL;
    // void* te2 = NULL;
    // tex = texture_create("test/img_test.png");
    // // te2 = texture_create("control_screen_axis.png");
    // te2 = texture_create("test/colordefadv2.png");
    // if(te2) test_draw_texture(te2);

    // texture_delete(tex);
    // texture_delete(te2);

    // test_sprite();

    // test_touch_squares();


/*joyst test*/
#if 0
SDL_Joystick *joy;
// Initialize the joystick subsystem
// SDL_InitSubSystem(SDL_INIT_JOYSTICK);
// Check for joystick
if (SDL_NumJoysticks() > 0) {
    // Open joystick
    joy = SDL_JoystickOpen(0);

    if (joy) {
        printf("Opened Joystick 0\n");
        printf("Name: %s\n", SDL_JoystickNameForIndex(0));
        printf("Number of Axes: %d\n", SDL_JoystickNumAxes(joy));
        printf("Number of Buttons: %d\n", SDL_JoystickNumButtons(joy));
        printf("Number of Balls: %d\n", SDL_JoystickNumBalls(joy));

        // while( !kbk(SDLK_ESCAPE) || kbk(SDLK_MENU) ) { }
    } else {
        printf("Couldn't open Joystick 0\n");
    }

    // Close if opened
    if (SDL_JoystickGetAttached(joy)) {
        SDL_JoystickClose(joy);
    }
}
#endif

    minig_quit();
#if defined(ANDROID)
    exit(0);
#endif
    return 0;

}
