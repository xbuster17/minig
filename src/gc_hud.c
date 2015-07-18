#include "gc_hud.h"
static bool hud_initialized = false;
static uint hud_shader;
static GLuint hud_vao;
static GLuint hud_vert_vbo;

static float hud_vert_data[2 * 6 * NUM_MAX_PLAYERS];

static void hud_init(void);
static void hud_update(void);

void gc_hud_draw(void) {
    if(!hud_initialized) hud_init();
    glUseProgram(hud_shader);
    glBindVertexArray(hud_vao);
    hud_update();
    // glDisable(GL_CULL_FACE);

    glDrawArrays(GL_TRIANGLES, 0, NUM_MAX_PLAYERS * 6);
    glBindVertexArray(0);
}

void gc_hud_terminate(void) {
    if(!hud_initialized) return;
    hud_initialized = false;
    glUseProgram(0);
    glDeleteProgram(hud_shader);
    glDeleteBuffers(1, &hud_vert_vbo);
    glDeleteVertexArrays(1, &hud_vao);
}


static void hud_init(void) {
    hud_initialized = true;
    hud_vert_data[0] =  -1.0;
    hud_vert_data[1] =  -1.0;
    hud_vert_data[2] =   1.0;//
    hud_vert_data[3] =  -1.0;
    hud_vert_data[4] =  -1.0;
    hud_vert_data[5] =  -0.95;

    hud_vert_data[6] =  -1.0;
    hud_vert_data[7] =  -0.95;
    hud_vert_data[8] =   1.0;//
    hud_vert_data[9] =  -1.0;
    hud_vert_data[10] =  1.0;//
    hud_vert_data[11] = -0.95;

    hud_vert_data[12] =  -1.0;//
    hud_vert_data[13] =   0.95;
    hud_vert_data[14] =   1.0;//**
    hud_vert_data[15] =   0.95;
    hud_vert_data[16] =  -1.0;//
    hud_vert_data[17] =   1.0;

    hud_vert_data[18] =  -1.0;//
    hud_vert_data[19] =   1.0;
    hud_vert_data[20] =   1.0;//**
    hud_vert_data[21] =   0.95;
    hud_vert_data[22] =   1.0;//**
    hud_vert_data[23] =   1.0;

    const GLchar gc_hud_vert_glsl[] =
        #ifdef ANDROID
        "precision mediump float;\n"
        #endif
        "attribute vec2 att_pos; \n"
        "void main() {\n"
            "gl_Position = vec4(att_pos, 0.0, 1.0);\n"
        "}";
    const GLchar gc_hud_frag_glsl[] =
        #ifdef ANDROID
        "precision mediump float;\n"
        #endif
        "void main() {\n"
            "gl_FragColor = vec4(1.0, 0.0, 0.0, 0.5);\n"
        "}";
    shader_create(&hud_shader, gc_hud_vert_glsl, gc_hud_frag_glsl,
        sizeof(gc_hud_vert_glsl), sizeof(gc_hud_frag_glsl));
    glUseProgram(hud_shader);

    glGenVertexArrays(1, &hud_vao);
    glBindVertexArray(hud_vao);
    glGenBuffers(1, &hud_vert_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, hud_vert_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * 6 * NUM_MAX_PLAYERS,
        hud_vert_data, GL_DYNAMIC_DRAW);
    GLint att_hud_vert_loc = glGetAttribLocation(hud_shader, "att_pos");
    glVertexAttribPointer(att_hud_vert_loc, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(att_hud_vert_loc);
    glBindVertexArray(0);


}
static float test = 1.0;
static void hud_update(void) {
    // float p1health = -1.0 + test;
    float p1health = -1.0 + (players[0].hp / (gridling_type_stats.p1_hp / 2.0));
    // float p2health = -1.0 + test;
    float p2health =  1.0 - (players[1].hp / (gridling_type_stats.p1_hp / 2.0));
    test +=0.1;
    hud_vert_data[2]  = p1health;
    hud_vert_data[8]  = p1health;
    hud_vert_data[10] = p1health;

    hud_vert_data[12] = p2health;
    hud_vert_data[16] = p2health;
    hud_vert_data[18] = p2health;
    glBindVertexArray(hud_vao);
    glBindBuffer(GL_ARRAY_BUFFER, hud_vert_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * 6 * NUM_MAX_PLAYERS,
        hud_vert_data, GL_DYNAMIC_DRAW);
    // glBindVertexArray(0);

}
