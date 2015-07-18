#ifndef TEST_TOUCH_H
#define TEST_TOUCH_H
#include "minig.h"

static uint shader;
static void create_test_shader(void);

static float screen_ratio;




void test_touch_squares(void) {
    float prev_axes_radius = Touch_conf.axes_radius;
    Touch_conf.axes_radius = 0.25;

    glBindVertexArray(0);
    create_test_shader();
    glUseProgram(shader);

    GLuint uni_pos_loc0 = glGetUniformLocation(shader, "uni_pos0");
    glUniform2f(uni_pos_loc0, 0.0, 0.0);
    GLuint uni_pos_loc1 = glGetUniformLocation(shader, "uni_pos1");
    glUniform2f(uni_pos_loc1, 0.0, 0.0);
    GLuint uni_scale_loc = glGetUniformLocation(shader, "uni_scale");
    glUniform2f(uni_scale_loc, 1.0, 1.0);
    GLuint uni_accel_loc = glGetUniformLocation(shader, "uni_accel");
    glUniform3f(uni_accel_loc, Accel[0], Accel[1], Accel[2]);

    GLuint quad_vbo;
    glGenBuffers(1, &quad_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Square),
        Square, GL_STATIC_DRAW);
    GLint att_pos_loc = glGetAttribLocation(shader, "att_pos");
    glVertexAttribPointer(att_pos_loc, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), 0);
    glEnableVertexAttribArray(att_pos_loc);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    swap_buffers();
    SDL_Delay(250);
    glClearColor(0.0,0.0,0.0,0.0);

    for(;;) {
        glClear(GL_COLOR_BUFFER_BIT);

        glUniform2f(uni_scale_loc, Mi.touches[0].y, Mi.touches[0].y);
        glUniform3f(uni_accel_loc, Accel[0], Accel[1], Accel[2]);
        //quad00
        glUniform2f(uni_pos_loc0, Touch.a[0]->dx, Touch.a[0]->dy);
        glUniform2f(uni_pos_loc1, 0.0, 0.0);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        //quad01
        glUniform2f(uni_pos_loc0, 0.0 ,0.0);
        glUniform2f(uni_pos_loc1, Touch.a[1]->dx, Touch.a[1]->dy);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        swap_buffers();
        poll_events();

        if (kbk(SDLK_ESCAPE) || kbk(SDLK_MENU) || kbk(SDLK_AC_BACK)) {
            glDisableVertexAttribArray(att_pos_loc);
            glUseProgram(0);
            glDeleteBuffers(1, &quad_vbo);
            glDeleteProgram(shader);
            shader = 0;
            Touch_conf.axes_radius = prev_axes_radius;
            return;
        }
    }
}

static void create_test_shader(void) {
    const GLchar vert_glsl[] =
        "attribute vec2 att_pos;"
        "uniform vec2 uni_scale;"
        "uniform vec3 uni_accel;"
        "uniform vec2 uni_pos0;"
        "uniform vec2 uni_pos1;"
        "void main() {"
            "gl_Position = vec4(att_pos * uni_scale + uni_pos0 + uni_pos1, 0.0, 1.0);"
        "}"
    ;

    const GLchar frag_glsl[] =
#ifdef ANDROID
        "precision highp float;\n"
#endif
        "uniform vec2 uni_pos0;\n"
        "uniform vec2 uni_pos1;\n"
        "uniform vec3 uni_accel;\n"
        "void main() {\n"
            "gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0)"
                "* vec4(abs(uni_accel.x), abs(uni_accel.y), abs(uni_accel.z), 1.0);"
        "}"
    ;

    shader_create(&shader, vert_glsl, frag_glsl,
        sizeof(vert_glsl), sizeof(frag_glsl));
}

static void create_test_controls_shader(void);
void test_draw_controls(void) {
    short vert_count; short quad_count;
    //scan
    Uint8 i; Uint8 bc = 0;
    for (i=0; i < NUM_MAX_TOUCH_BUTTONS; i++) {
        if (Touch_conf.buttons[i].active) bc++;
    }
    for (i=0; i < NUM_MAX_TOUCH_AXES;    i++) {
        if (Touch_conf.axes[i].active) bc++;
    }
    vert_count = bc * 4 * 2;
    quad_count = bc;
    // float vertices[vert_count];
    float* vertices = malloc(vert_count * sizeof(float));
    //load
    bc = 0;
    for (i=0; i < NUM_MAX_TOUCH_BUTTONS; i++) {
        if (Touch_conf.buttons[i].active) {
            vertices[bc] = Touch_conf.buttons[i].x0;
            bc++;
            vertices[bc] = Touch_conf.buttons[i].y0;
            bc++;
            vertices[bc] = Touch_conf.buttons[i].x1;
            bc++;
            vertices[bc] = Touch_conf.buttons[i].y0;
            bc++;
            vertices[bc] = Touch_conf.buttons[i].x0;
            bc++;
            vertices[bc] = Touch_conf.buttons[i].y1;
            bc++;
            vertices[bc] = Touch_conf.buttons[i].x1;
            bc++;
            vertices[bc] = Touch_conf.buttons[i].y1;
            bc++;
        }
    }
    for (i=0; i < NUM_MAX_TOUCH_AXES;    i++) {
        if (Touch_conf.axes[i].active) {
            vertices[bc] = Touch_conf.axes[i].x0;
            bc++;
            vertices[bc] = Touch_conf.axes[i].y0;
            bc++;
            vertices[bc] = Touch_conf.axes[i].x1;
            bc++;
            vertices[bc] = Touch_conf.axes[i].y0;
            bc++;
            vertices[bc] = Touch_conf.axes[i].x0;
            bc++;
            vertices[bc] = Touch_conf.axes[i].y1;
            bc++;
            vertices[bc] = Touch_conf.axes[i].x1;
            bc++;
            vertices[bc] = Touch_conf.axes[i].y1;
            bc++;
        }
    }
    //make gl resources
    glBindVertexArray(0);
    create_test_controls_shader();
    glUseProgram(shader);

    GLuint uni_col = glGetUniformLocation(shader, "uni_col");
    glUniform4f(uni_col, 1.0, 0.0, 0.0, 0.5);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vert_count * sizeof(float),
        vertices, GL_STATIC_DRAW);
    GLint att_pos_loc = glGetAttribLocation(shader, "att_pos");
    glVertexAttribPointer(att_pos_loc, 2, GL_FLOAT, GL_FALSE,
        0, 0);
    glEnableVertexAttribArray(att_pos_loc);

    SDL_Delay(250);
    glClearColor(0.0,0.0,0.0,0.0);
    //draw
    float r,g,b,a;
    for(;;){
        glClear(GL_COLOR_BUFFER_BIT);

        for(i=0; i<quad_count; i++){
            switch (i%4) {
                case 0: r = 1.0; g = 0.0; b = 0.0; break;
                case 1: r = 0.0; g = 1.0; b = 0.0; break;
                case 2: r = 1.0; g = 1.0; b = 0.0; break;
                case 3: r = 0.0; g = 0.0; b = 1.0; break;
            }
            a = 0.5;

            // touch_t* a0 = get_touch_via_type(TOUCH_B9);
            // // if (a0->active) {
            // //     a = a0->x;
            // //     g = a0->y;
            // // } else a = 0.5;
            // a = Touch.b[9]->x;
            // g = Touch.b[9]->y;

            glUniform4f(uni_col, r, g, b, a);

            glDrawArrays(GL_TRIANGLE_STRIP, i*4, 4);
        }
        swap_buffers();
        poll_events();

        if (kbk(SDLK_ESCAPE) || kbk(SDLK_MENU) || kbk(SDLK_AC_BACK) || Touch.b[5]->id == FINGER_RELEASED) {
            glDisableVertexAttribArray(att_pos_loc);
            glUseProgram(0);
            glDeleteBuffers(1, &vbo);
            glDeleteProgram(shader);
            shader = 0;
            free(vertices);
            return;
        }
    }
}


static void create_test_controls_shader(void) {
    const GLchar vert_glsl[] =
        "attribute vec2 att_pos;"
        // "uniform vec3 uni_col;"
        "void main() {"
            "gl_Position = vec4((att_pos*2.0)-1.0, 0.0, 1.0);"
        "}"
    ;

    const GLchar frag_glsl[] =
    #ifdef ANDROID
        "precision highp float;\n"
    #endif
        "uniform vec4 uni_col;\n"
        "void main() {\n"
            "gl_FragColor = vec4(uni_col);"
        "}"
    ;
    // TODO gl_PointCoord research

    shader_create(&shader, vert_glsl, frag_glsl,
        sizeof(vert_glsl), sizeof(frag_glsl));
}

static void create_test_axes_shader(void);

void test_touch_axes(void) {
    float prev_axes_radius = Touch_conf.axes_radius;
    Touch_conf.axes_radius = .33;

    float prev_y = Touch_conf.axes[0].y1;
    Touch_conf.axes[0].y1 = 1.0;
    // float prev_x = Touch_conf.axes[0].x1;

    glBindVertexArray(0);
    create_test_axes_shader();
    glUseProgram(shader);

    GLuint uni_pos_loc = glGetUniformLocation(shader, "uni_pos");
    glUniform2f(uni_pos_loc, 0.0, 0.0);
    GLuint uni_scale_loc = glGetUniformLocation(shader, "uni_scale");
    glUniform2f(uni_scale_loc, 1.0, 1.0);

    GLuint quad_vbo;
    glGenBuffers(1, &quad_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Square),
        Square, GL_STATIC_DRAW);

    GLint att_pos_loc = glGetAttribLocation(shader, "att_pos");
    glVertexAttribPointer(att_pos_loc, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), 0);
    glEnableVertexAttribArray(att_pos_loc);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    swap_buffers();
    SDL_Delay(250);
    glClearColor(0.0,0.0,0.0,1.0);

    struct char_pos{
        float x,y;
    } joe;
    joe.x = Touch.a[0]->x;
    joe.y = Touch.a[0]->y;

    float joypad[8];
    memcpy(joypad, Square, 8*4);


    for(;;) {
        glClearColor(fabs(Touch.a[0]->dx),fabs(Touch.a[0]->dy),0.0,1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        // joe.x += (Touch.a[0]->dx / 20.0);
        joe.x += Touch.a[0]->dx / 10.0;
        // joe.y += (Touch.a[0]->dy / 20.0);
        joe.y += Touch.a[0]->dy / 10.0;
        if      (joe.x >=  1.0) joe.x =  1.0;
        else if (joe.x <= -1.0) joe.x = -1.0;
        if      (joe.y >=  1.0) joe.y =  1.0;
        else if (joe.y <= -1.0) joe.y = -1.0;

//draw joe
        glUniform2f(uni_pos_loc, joe.x, joe.y);
        glUniform2f(uni_scale_loc, 0.1, 0.1);

        glBufferData(GL_ARRAY_BUFFER, sizeof(Square),
            Square, GL_STATIC_DRAW);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

//draw joypad
        float rad = Touch_conf.axes_radius;
        float rad_scaled = rad * screen_ratio;

        joypad[0] = ((Touch.a[0]->x * 2.0) - 1.0) - rad;
        joypad[1] = ((Touch.a[0]->y * 2.0) - 1.0) - rad_scaled;
        joypad[2] = ((Touch.a[0]->x * 2.0) - 1.0) + rad;
        joypad[3] = ((Touch.a[0]->y * 2.0) - 1.0) - rad_scaled;
        joypad[4] = ((Touch.a[0]->x * 2.0) - 1.0) - rad;
        joypad[5] = ((Touch.a[0]->y * 2.0) - 1.0) + rad_scaled;
        joypad[6] = ((Touch.a[0]->x * 2.0) - 1.0) + rad;
        joypad[7] = ((Touch.a[0]->y * 2.0) - 1.0) + rad_scaled;

        glBufferData(GL_ARRAY_BUFFER, sizeof(joypad),
            joypad, GL_STATIC_DRAW);

        glUniform2f(uni_pos_loc, 0.0,0.0);
        glUniform2f(uni_scale_loc, 1.0, 1.0);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        swap_buffers();
        poll_events();

        if (kbk(SDLK_ESCAPE) || kbk(SDLK_MENU) || kbk(SDLK_AC_BACK) || !Mi.running) {
            glDisableVertexAttribArray(att_pos_loc);
            glUseProgram(0);
            glDeleteBuffers(1, &quad_vbo);
            glDeleteProgram(shader);
            shader = 0;
            Touch_conf.axes_radius = prev_axes_radius;
            Touch_conf.axes[0].y1 = prev_y;
            return;
        }
    }
}


static void create_test_axes_shader(void){
    const GLchar vert_glsl[] =
        "attribute vec2 att_pos;"
        "uniform vec2 uni_scale;"
        "uniform vec2 uni_pos;"
        "void main() {"
            "gl_Position = vec4((att_pos*uni_scale) + uni_pos, 0.0, 1.0);"
        "}"
    ;

    const GLchar frag_glsl[] =
#ifdef ANDROID
        "precision highp float;\n"
#endif
        "void main() {\n"
            "gl_FragColor = vec4(0.0, 0.0, 1.0, 0.5);"
        "}"
    ;
    shader_create(&shader, vert_glsl, frag_glsl,
        sizeof(vert_glsl), sizeof(frag_glsl));
}

#endif
