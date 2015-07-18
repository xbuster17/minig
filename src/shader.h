#ifndef SHADER_H
#define SHADER_H

#include "minig.h"

void shader_delete (uint* shader);

void shader_create (uint* shader,
    const GLchar*vs, const GLchar*fs,
    int vs_size, int fs_size
);

#ifndef ANDROID
void show_shader_log (GLuint object,
    PFNGLGETSHADERIVPROC glGet__iv,
    PFNGLGETSHADERINFOLOGPROC glGet__InfoLog
);
#else
#define show_shader_log(x,y,z) ;
#endif

#endif

