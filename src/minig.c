#include "minig.h"

SDL_Window* Window;
SDL_GLContext Context;
SDL_Event Event;
const Uint8* KBstate; //keyboard state
minig_t Mi;

static int flags = SDL_WINDOW_OPENGL /*| SDL_WINDOW_FULLSCREEN*/;

void minig_init (void) {
    SDL_Init(SDL_INIT_EVERYTHING);
    window_size_get(&Mi.sx, &Mi.sy);
    gl_attributes_set();
    Window = SDL_CreateWindow("minig", 0, 0, Mi.sx, Mi.sy, flags);
    Context = SDL_GL_CreateContext(Window);
    SDL_GL_MakeCurrent(Window, Context);
    gl_ext_init();
    gl_configuration_set();

    menu_init();
    touch_init();
    square_init();

    // entity_particle_init();

}

void window_size_get (uint* sx, uint* sy) {
    SDL_DisplayMode mode;
    SDL_GetDisplayMode(0, 0, &mode);
    *sx = mode.w;
    *sy = mode.h;
}

void minig_quit (void) {
#if defined(ANDROID)
    droid_gles_ext_quit();
#endif
    // entity_particle_quit();

    SDL_GL_DeleteContext(Context);
    SDL_DestroyWindow(Window);
    SDL_Quit();
}

void gl_attributes_set (void) {
#ifdef ANDROID
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
        SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
#else
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
        SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
#endif
}

void gl_ext_init (void) {
#ifndef ANDROID
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK) {
        printf("ERROR: %s\n", glewGetErrorString(glewError));
        exit(1);
    }
    const unsigned char * glver = glGetString(GL_VERSION);
    printf("\tGL running! version: %s\n", glver);
#else
    droid_gles_ext_init();
#endif
}


#if defined(ANDROID)

PFNGLBINDVERTEXARRAYOESPROC    glBindVertexArray;
PFNGLDELETEVERTEXARRAYSOESPROC glDeleteVertexArrays;
PFNGLGENVERTEXARRAYSOESPROC    glGenVertexArrays;
PFNGLISVERTEXARRAYOESPROC      glIsVertexArray;

#include <dlfcn.h>
static bool droid_gles_ext_loaded = 0;
static void* droid_gles_ext_libhandle = NULL;
void droid_gles_ext_init (void) {
    if (!droid_gles_ext_loaded) {


        droid_gles_ext_libhandle = dlopen("libGLESv2.so",
            RTLD_LAZY|RTLD_GLOBAL);
        void* libhandle = droid_gles_ext_libhandle;
        //VAO
        glBindVertexArray = (PFNGLBINDVERTEXARRAYOESPROC)dlsym(libhandle,
            "glBindVertexArrayOES");
        glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSOESPROC)dlsym(libhandle,
            "glDeleteVertexArraysOES");
        glGenVertexArrays = (PFNGLGENVERTEXARRAYSOESPROC)dlsym(libhandle,
            "glGenVertexArraysOES");
        glIsVertexArray = (PFNGLISVERTEXARRAYOESPROC)dlsym(libhandle,
            "glIsVertexArrayOES");

            droid_gles_ext_loaded = 1;

    } else {

        printf("NOTE: tried to reload gles droid extensions!\n");
    }
}
void droid_gles_ext_quit (void) {
    dlclose(droid_gles_ext_libhandle);
}
#endif

void gl_configuration_set (void) {
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    SDL_GL_SetSwapInterval(1);
    // glDisable(GL_CULL_FACE);

}

void swap_buffers(void) {
    SDL_GL_SwapWindow(Window);
}
