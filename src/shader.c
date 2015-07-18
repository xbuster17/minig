#include "minig.h"

void shader_delete(uint* shader) {
    glDeleteProgram(*shader);
    *shader = 0;
}

void shader_create(uint* shader,
    const GLchar*vs, const GLchar*fs,
    int vs_size, int fs_size
) {
    //vars
    GLuint vs_id, fs_id;
    GLint vs_len, fs_len;
    vs_len = vs_size;
    fs_len = fs_size;
    GLint shader_ok;
    //vs
    vs_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs_id, 1, (const GLchar**)&vs, &vs_len);
    // char* vs_bug = malloc(vs_size);
    // glGetShaderSource(vs_id, vs_len, &vs_len, vs_bug);
    // printf("%s\n", vs_bug);
    // free(vs_bug);
    glCompileShader(vs_id);
    glGetShaderiv(vs_id, GL_COMPILE_STATUS, &shader_ok);
    if (!shader_ok) {
        fprintf(stderr, "Failed to compile %s\n", vs);
        show_shader_log(vs_id, glGetShaderiv, glGetShaderInfoLog);
        glDeleteShader(vs_id);
    }
    //fs
    fs_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs_id, 1, (const GLchar**)&fs, &fs_len);
    glCompileShader(fs_id);

    glGetShaderiv(fs_id, GL_COMPILE_STATUS, &shader_ok);
    if (!shader_ok) {
        fprintf(stderr, "Failed to compile %s\n", fs);
        show_shader_log(fs_id, glGetShaderiv, glGetShaderInfoLog);
        glDeleteShader(fs_id);
    }
    //program - shader
    *shader = glCreateProgram();
    glAttachShader(*shader, vs_id);
    glAttachShader(*shader, fs_id);
    glLinkProgram(*shader);
    GLint program_ok;
    glGetProgramiv(*shader, GL_LINK_STATUS, &program_ok);
    if (!program_ok) {
        fprintf(stderr, "Failed to link shader program:\n");
        show_shader_log(*shader, glGetProgramiv, glGetProgramInfoLog);
        glDeleteProgram(*shader);
    }
}

#ifndef ANDROID
    void show_shader_log (GLuint object,
        PFNGLGETSHADERIVPROC glGet__iv,
        PFNGLGETSHADERINFOLOGPROC glGet__InfoLog
    ) {
        GLint log_length;
        char *log_;
        glGet__iv(object, GL_INFO_LOG_LENGTH, &log_length);
        log_ = malloc(log_length);
        glGet__InfoLog(object, log_length, NULL, log_);
        fprintf(stderr, "%s", log_);
        free(log_);
    }
#endif
