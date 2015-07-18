#include "minig.h"

#include "test_touch.h"


float Square[8] = {
    -1.0 ,  1.0,
     1.0 ,  1.0,
    -1.0 , -1.0,
     1.0 , -1.0,
};

float Square_tex_coord[8] = {
     0.0 ,  0.0,
     1.0 ,  0.0,
     0.0 ,  1.0,
     1.0 ,  1.0,
};

static bool square_inited = false;
void square_init(void){
    if (square_inited) return;
    screen_ratio = (float)Mi.sx / (float)Mi.sy;
    if (screen_ratio != 0.0) {
        Square[0] /= screen_ratio;
        Square[2] /= screen_ratio;
        Square[4] /= screen_ratio;
        Square[6] /= screen_ratio;
        square_inited = true;
    }
}

static void create_test_draw_tex_shader(void);

void test_draw_texture(texture_t* tex) {
    glBindVertexArray(0);
    create_test_draw_tex_shader();
    glUseProgram(shader);

    glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, tex->id);
    texture_bind(tex);

    GLuint quad_vbo;
    glGenBuffers(1, &quad_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Square),
        Square, GL_STATIC_DRAW);

    GLint att_pos_loc = glGetAttribLocation(shader, "att_pos");
    glVertexAttribPointer(att_pos_loc, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), 0);
    glEnableVertexAttribArray(att_pos_loc);

    GLuint quad_tex_coord_vbo;
    glGenBuffers(1, &quad_tex_coord_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, quad_tex_coord_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Square_tex_coord),
        Square_tex_coord, GL_STATIC_DRAW);

    GLint att_tex_coord_pos = glGetAttribLocation(shader, "att_tex_coord");
    glVertexAttribPointer(att_tex_coord_pos, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), 0);
    glEnableVertexAttribArray(att_tex_coord_pos);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    swap_buffers();
    SDL_Delay(250);
    glClearColor(0.0,0.0,0.0,1.0);

    for(;;) {
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        swap_buffers();
        poll_events();

        if (kbk(SDLK_ESCAPE) || kbk(SDLK_MENU) || kbk(SDLK_AC_BACK) || !Mi.running) {
            glDisableVertexAttribArray(att_pos_loc);
            glUseProgram(0);
            glDeleteBuffers(1, &quad_vbo);
            glDeleteProgram(shader);
            shader = 0;
            return;
        }
    }
}


static void create_test_draw_tex_shader(void){
    const GLchar vert_glsl[] =
        "attribute vec2 att_pos;\n"
        "attribute vec2 att_tex_coord;\n"
        "varying vec2 var_tex_coord;\n"
        "void main() {\n"
            "gl_Position = vec4(att_pos, 0.0, 1.0);\n"
            "var_tex_coord = att_tex_coord;\n"
        "}"
    ;

    const GLchar frag_glsl[] =
#ifdef ANDROID
        "precision highp float;\n"
#endif
        "uniform sampler2D tex;\n"
        "varying vec2 var_tex_coord;\n"
        "void main() {\n"
            "gl_FragColor = texture2D(tex,var_tex_coord);\n"
        "}"
    ;
    shader_create(&shader, vert_glsl, frag_glsl,
        sizeof(vert_glsl), sizeof(frag_glsl));
}

static void create_test_sprite_shader(void);
void test_sprite(void) {
    glBindVertexArray(0);
    create_test_sprite_shader();
    glUseProgram(shader);

    entity_t* ent = entity_create_particle();

    glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, tex->id);
    texture_bind(ent->texture);

    GLuint quad_vbo;
    glGenBuffers(1, &quad_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Square),
        Square, GL_STATIC_DRAW);

    GLint att_pos_loc = glGetAttribLocation(shader, "att_pos");
    glVertexAttribPointer(att_pos_loc, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), 0);
    glEnableVertexAttribArray(att_pos_loc);

    glBindBuffer(GL_ARRAY_BUFFER, ent->tex_coord_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*8,
        ent->tex_coord, GL_DYNAMIC_DRAW);

    GLint att_tex_coord_pos = glGetAttribLocation(shader, "att_tex_coord");
    glVertexAttribPointer(att_tex_coord_pos, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), 0);
    glEnableVertexAttribArray(att_tex_coord_pos);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    swap_buffers();
    SDL_Delay(250);
    glClearColor(0.0,0.0,0.0,1.0);

    float sc = 0;
    Uint8 st = 9;
    for(;;) {
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        sc+=0.5;
        entity_update_sprite(ent, floor(sc), st);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*8,
            ent->tex_coord, GL_DYNAMIC_DRAW);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        swap_buffers();
        poll_events();

        if (sc>32.0*4) {st++;sc=0;}
        // if (st==7) sc%=18;

        if (kbk(SDLK_ESCAPE) || kbk(SDLK_MENU) || kbk(SDLK_AC_BACK) || !Mi.running) {
            glDisableVertexAttribArray(att_pos_loc);
            glDisableVertexAttribArray(att_tex_coord_pos);
            glUseProgram(0);
            glDeleteBuffers(1, &quad_vbo);
            glDeleteProgram(shader);
            entity_delete(ent);
            shader = 0;
            return;
        }
    }
}

static void create_test_sprite_shader(void) {
    const GLchar vert_glsl[] =
        "attribute vec2 att_pos;\n"
        "attribute vec2 att_tex_coord;\n"
        "varying vec2 var_tex_coord;\n"
        "void main() {\n"
            "gl_Position = vec4(att_pos/2.0, 0.0, 1.0);\n"
            "var_tex_coord = att_tex_coord;\n"
        "}"
    ;

    const GLchar frag_glsl[] =
    #ifdef ANDROID
        "precision highp float;\n"
    #endif
        "uniform sampler2D tex;\n"
        "varying vec2 var_tex_coord;\n"
        "void main() {\n"
            "gl_FragColor = texture2D(tex,var_tex_coord);\n"
        "}"
    ;
    shader_create(&shader, vert_glsl, frag_glsl,
        sizeof(vert_glsl), sizeof(frag_glsl));
}
