#ifndef MINIG_H
#define MINIG_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

#include "lodepng.h"
#if defined (ANDROID)
    #include <GLES2/gl2ext.h>
    #include "../android/jni/SDL2/include/SDL.h"
    #include "../android/jni/SDL2_mixer/SDL_mixer.h"
#else
    #include <GL/glew.h>
    #include <SDL2/SDL.h>
    #include <SDL2/SDL_mixer.h>
#endif

typedef unsigned int uint;
// #include "minig_types.h"
#include "minig_globals.h"
#include "minig_types.h"
#if defined (ANDROID)
    extern PFNGLBINDVERTEXARRAYOESPROC    glBindVertexArray;
    extern PFNGLDELETEVERTEXARRAYSOESPROC glDeleteVertexArrays;
    extern PFNGLGENVERTEXARRAYSOESPROC    glGenVertexArrays;
    extern PFNGLISVERTEXARRAYOESPROC      glIsVertexArray;
#endif
#include "shader.h"
#include "texture.h"
#include "sprite.h"
#include "entity.h"
#include "menu.h"
#include "events.h"

#include "tests.h"
#include "soundtest.h"

#include "game_grid_critters.h"
#include "audio_test.h"
typedef struct minig_s {
    bool running;
    uint sx, sy;
    menu_t menu;

    touch_t touches[NUM_MAX_TOUCHES];
    joystick_t joysticks[NUM_MAX_JOYSTICKS];

    // void* controllers[NUM_MAX_CONTROLLERS];

    uint num_players;
} minig_t;
extern minig_t Mi;




void minig_init (void);
void minig_quit (void);
void window_size_get (uint* sx, uint* sy);
void gl_attributes_set (void);
void gl_ext_init (void);
void droid_gles_ext_init (void);
void droid_gles_ext_quit (void);
void gl_configuration_set (void);

void swap_buffers(void);


#endif
