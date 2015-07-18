#include "minig.h"
#include "gc_grid.h"
#include "gc_gridlings.h"
#include "gc_projectiles.h"
#include "gc_players.h"
#include "gc_hud.h"
#include "gc_sound.h"

#define GRID_SIZE 100
// #include "grid.h"

uint gc_shader;
static GLuint portals_vao;

//textures
texture_t* particle_tex = NULL;
sprite_t* particle_sprite = NULL;
float env_particle_move = 0.0;

uint terrain_set = 12;

texture_t* terrain_tex = NULL;
sprite_t* terrain_sprite = NULL;


//fn
static void terminate_game(void);
static void portals_draw(void);
static void portals_terminate(void);
uint level_x = 127;
uint level_y = 127;
static uint seed = 0;


//
//main
//
void run_game__grid_critters(void) {
    srand(time(0));
    float prev_axes_radius = Touch_conf.axes_radius;
    Touch_conf.axes_radius = 0.1;

    // printf("\tmax_move_speed: %.5f\n", MAX_MOVE_SPEED);

    init_sound();

    shader_grid_critters_create();
    glUseProgram(gc_shader);

    // srand();
    particle_init();

    terrain_init();

    //grid
    grid_size = grid_size_x * grid_size_y;
    grid_create();
    grid_generate();

    players_init();
    gridlings_init();
    projectiles_init();

    uni_zoom_loc = glGetUniformLocation(gc_shader, "uni_zoom");
    glUniform2f(uni_zoom_loc, 0.0, 0.0);
    GLuint uni_col_loc = glGetUniformLocation(gc_shader, "uni_col");
    glUniform4f(uni_col_loc, 0.0, 0.0, 0.0, 0.0);
    uni_cam_loc = glGetUniformLocation(gc_shader, "uni_cam");
    glUniform2f(uni_cam_loc, 0.0, 0.0);

    GLuint uni_screen_ratio_loc = glGetUniformLocation(gc_shader, "uni_screen_ratio");
    glUniform2f(uni_screen_ratio_loc, ((float)Mi.sy / (float)Mi.sx), 1.0);

    change_level(level_x, level_y);

    // singletons_spawn();

    float r,g,b,a, color;
    //begin
    for(;;) {

        poll_events();

        // if(kbk(SDLK_UP)) {
        //     level_x += 1;
        //     change_level(level_x, level_y);
        // }

        glUseProgram(gc_shader);

        if(gc__grid_update_flag){
            grid_update(); gc__grid_update_flag = false;}
        players_update();
        gridlings_update();
        projectiles_update();

    //draw
        color = fmin(fmax(sin(SDL_GetTicks() / 1e4) + 1.0, 0.5), 1.0);
        r =
        g =
        b =
        a = color;


        glUniform4f(uni_col_loc, r,g,b,a);

        glClear(GL_COLOR_BUFFER_BIT);
        //draw_grid();
        texture_bind(terrain_tex);
        glBindVertexArray(grid_vao);
        glDrawArrays(GL_TRIANGLES, 0, grid_size * 6);

        glUniform4f(uni_col_loc, 1.0, 1.0, 1.0, 1.0);

        texture_bind(particle_tex);

        portals_draw();


        glBindVertexArray(gridlings_vao);
        glDrawArrays(GL_TRIANGLES, 0, NUM_MAX_GRIDLINGS * 6);
        glBindVertexArray(projectiles_vao);
        glDrawArrays(GL_TRIANGLES, 0, NUM_MAX_PROJECTILES * 6);
        glBindVertexArray(players_vao);
        glDrawArrays(GL_TRIANGLES, 0, 2 * 6);

        gc_hud_draw();

        swap_buffers();

        // TEMP death

        if(players[0].hp <= 0.0)
        for(r = 1.0;;) {
            glClearColor(r, 0.0, 0.0, 1.0);
            glClear(GL_COLOR_BUFFER_BIT);
            swap_buffers();
            r -= 0.02;
            if(r <= 0.0) {
                players[0].fx = 0.0;
                players[0].fy = 0.0;
                players[0].hp = 100.0;
                players[1].fx = 0.0;
                players[1].fy = 0.0;
                players[1].hp = 100.0;
                break;
            }
        }

        if(kbk(SDLK_MENU) || kbk(SDLK_ESCAPE) || kbk(SDLK_AC_BACK)) goto end;
    }
end:
    terminate_game();

}

static void terminate_game(void) {
    glUseProgram(0);
    glDeleteProgram(gc_shader);

    particle_quit();
    terrain_quit();

    players_quit();
    gridlings_quit();
    projectiles_quit();

    grid_delete();

    gc_hud_terminate();

    portals_terminate();

    quit_sound();
}

//
//shader
//
void shader_grid_critters_create(void) {
    const GLchar gc_vert_glsl[] =
        #ifdef ANDROID
        "precision mediump float;\n"
        #endif
        "attribute vec2 att_pos; \n"
        "attribute vec2 att_texc; \n"
        "uniform vec2 uni_zoom; \n"
        "uniform vec2 uni_screen_ratio; \n"
        "uniform vec2 uni_cam; \n"
        "varying vec2 var_texc; \n"
        "varying vec2 var_pos; \n"
        "void main() {\n"
            "gl_Position = vec4(((att_pos- uni_cam) * uni_zoom) * uni_screen_ratio, 0.0, 1.0);\n"
            "var_texc = att_texc;\n"
            "var_pos = att_pos;\n"
        "}";
    const GLchar gc_frag_glsl[] =
        #ifdef ANDROID
        "precision mediump float;\n"
        #endif
        "varying vec2 var_texc;\n"
        "varying vec2 var_pos;\n"
        "uniform sampler2D uni_tex;\n"
        "uniform vec4 uni_col;\n"
        "void main() {\n"
            "gl_FragColor = texture2D(uni_tex, var_texc) * uni_col;\n"
        "}";
    shader_create(&gc_shader, gc_vert_glsl, gc_frag_glsl,
        sizeof(gc_vert_glsl), sizeof(gc_frag_glsl));
}

//
//textures
//
void particle_init(void) {
    if(particle_tex || particle_sprite) return;
    particle_tex = texture_create("particles_sprite_sheet_x2_fx.png");
    particle_sprite = malloc(sizeof(sprite_t));
    particle_sprite->count_x = NUM_PART_X;
    particle_sprite->count_y = NUM_PART_Y;
    // particle_sprite->pixs = 32;
    particle_sprite->pixs = 32;
}

void particle_quit(void) {
    if(!particle_tex || !particle_sprite) return;
    texture_delete(particle_tex); particle_tex = NULL;
    free(particle_sprite); particle_sprite = NULL;
}

void terrain_init(void) {
    if(terrain_tex || terrain_sprite) return;
    terrain_tex = texture_create("terrain_sprite_sheet_x2_smooth.png");
    terrain_sprite = malloc(sizeof(sprite_t));
    terrain_sprite->count_x = NUM_TERR_X;
    terrain_sprite->count_y = NUM_TERR_Y;
    terrain_sprite->pixs = 32;
}

void terrain_quit(void) {
    if(!terrain_tex || !terrain_sprite) return;
    texture_delete(terrain_tex); terrain_tex = NULL;
    free(terrain_sprite); terrain_sprite = NULL;
}

void get_tile_particle(enum particles_e type, uint* x, uint* y) {
    uint move = floor(env_particle_move);
    switch(type) {
        case PART_PLASMA1: *x = move % NUM_PART_X; *y = 0;  break;
        case PART_PLASMA2: *x = move % NUM_PART_X; *y = 1;  break;
        case PART_STAR:    *x = move % NUM_PART_X; *y = 2;  break;
        case PART_PLASMA3: *x = move % NUM_PART_X; *y = 3;  break;
        case PART_RING:    *x = move % NUM_PART_X; *y = 4;  break;
        case PART_PLASMA4: *x = move % NUM_PART_X; *y = 5;  break;
        case PART_SUN:     *x = move % NUM_PART_X; *y = 6;  break;
        case PART_BALL:    *x = move % NUM_PART_X; *y = 7;  break;
        case PART_PLAY1:   *x = move % NUM_PART_X; *y = 8;  break;
        case PART_PLASMA5: *x = move % NUM_PART_X; *y = 9;  break;
        case PART_PLAY2:   *x = move % NUM_PART_X; *y = 10; break;
        case PART_GREEN:   *x = move % NUM_PART_X; *y = 11; break;
        case PART_RAINBOW:   *x = move % NUM_PART_X; *y = 12; break;
        case PART_PORTAL:    *x = move % NUM_PART_X; *y = 13; break;
        default: *x = 0; *y = 0; break;
    }
}



//
//game-logic
//
bool terrain_collision(uint x, uint y) {
    if(grid_data[x * grid_size_y + y] == TERR_ROCK)
        return true;
    else return false;
}

void singletons_spawn(void) {
    uint i; uint j; uint type; uint spawn_type;
    float x_step = 2.0 / grid_size_x;
    float y_step = 2.0 / grid_size_y;
    for(i = 0; i < grid_size_x -1; i++) {
        for(j = 0; j < grid_size_y -1; j++) {
            if((i > 1) && (j > 1))
                if((i < grid_size_x-1) && (j < grid_size_y-1)) {
                    type = grid_data[(i * grid_size_y) + j];
                    if(
                        (grid_data[((i-1) * grid_size_y) + (j  )] != type) &&
                        (grid_data[((i-1) * grid_size_y) + (j-1)] != type) &&
                        (grid_data[((i  ) * grid_size_y) + (j-1)] != type) &&
                        (grid_data[((i+1) * grid_size_y) + (j-1)] != type) &&
                        (grid_data[((i+1) * grid_size_y) + (j  )] != type) &&
                        (grid_data[((i+1) * grid_size_y) + (j+1)] != type) &&
                        (grid_data[((i  ) * grid_size_y) + (j+1)] != type) &&
                        (grid_data[((i-1) * grid_size_y) + (j+1)] != type)
                        ) {
                        switch(type) {
                            case TERR_ROCK:  spawn_type = GRIDLING_E1_SPAWNER; break;
                            case TERR_GRASS: spawn_type = GRIDLING_E2_SPAWNER; break;
                            case TERR_WATER: spawn_type = GRIDLING_E3_SPAWNER; break;
                            case TERR_DIRT:  spawn_type = GRIDLING_E4_SPAWNER; break;
                            default: spawn_type = GRIDLING_E1_SPAWNER; break;
                        }
                        gridling_spawn(spawn_type,
                            ((i * x_step) - 1.0) + (x_step / 2.0),
                            ((j * y_step) - 1.0) + (y_step / 2.0));

                        grid_data[(i * grid_size_y) + j] = TERR_PLASMA;

                    }
                }
        }
    }
}

void change_level(uint lev_x, uint lev_y) {
    uint i;
    for(i = 0; i < NUM_MAX_GRIDLINGS; i++) {
        gridlings[i].type = GRIDLING_NULL;
    }
    num_active_gridlings = 0;
    for(i = 0; i < NUM_MAX_PROJECTILES; i++) {
        projectiles[i].type = PROJECTILE_NULL;
    }
    num_active_projectiles = 0;

#ifndef ANDROID
    seed = (lev_x << 16) | lev_y;
    // seed <<= 8;
    // seed |= lev_y;
#else
    seed = (lev_x * 265) + lev_y; // TEMP
#endif

    srand(seed);

    terrain_set = rand() % NUM_TERR_X;
    grid_renew_data();

    float x_step = (2.0 / grid_size_x);
    float y_step = (2.0 / grid_size_y);

    if(player_warp_left) {
        players[0].fx =  1.0 - x_step * 3.0;
        players[1].fx =  1.0 - x_step * 3.0;
        players[0].fy =  0.0;
        players[1].fy =  0.0;
        player_warp_left = false;
    }
    else if(player_warp_right) {
        players[0].fx =  -1.0 + x_step * 3.0;
        players[1].fx =  -1.0 + x_step * 3.0;
        players[0].fy =  0.0;
        players[1].fy =  0.0;
        player_warp_right = false;
    }
    else if(player_warp_up) {
        players[0].fx =  0.0;
        players[1].fx =  0.0;
        players[0].fy =  -1.0 + y_step * 3.0;
        players[1].fy =  -1.0 + y_step * 3.0;
        player_warp_up = false;
    }
    else if(player_warp_down) {
        players[0].fx =  0.0;
        players[1].fx =  0.0;
        players[0].fy =  1.0 - y_step * 3.0;
        players[1].fy =  1.0 - y_step * 3.0;
        player_warp_down = false;
    }

    singletons_spawn();
    gc__grid_update_flag = true;

}

static void portals_init(void);
static bool portals_initialized = false;
static void portals_update_texc(void);
static void portals_draw(void) {
    if(!portals_initialized) portals_init();

    portals_update_texc();
    glBindVertexArray(portals_vao);
    glDrawArrays(GL_TRIANGLES, 0 ,4 * 6);
}

float portals_vert[4 * 2 * 6];
float portals_texc[4 * 2 * 6];
GLuint portals_vert_vbo;
GLuint portals_texc_vbo;
gridling_t portals[4];
static void portals_init(void) {
    portals_initialized = true;
    portals[0].fx = -1.0;
    portals[0].fy =  0.0;
    portals[1].fx =  1.0;
    portals[1].fy =  0.0;
    portals[2].fx =  0.0;
    portals[2].fy =  1.0;
    portals[3].fx =  0.0;
    portals[3].fy = -1.0;

    float x_step = (2.0 / grid_size_x) * 2.0;
    float y_step = (2.0 / grid_size_y) * 2.0;
    uint i = 0;
    uint j = 0;
    for(j = 0; j < 4; j++) {
        portals_vert[i++] = portals[j].fx   - x_step;
        portals_vert[i++] = portals[j].fy   - y_step;
        portals_vert[i++] = portals[j].fx + x_step;
        portals_vert[i++] = portals[j].fy   - y_step;
        portals_vert[i++] = portals[j].fx   - x_step;
        portals_vert[i++] = portals[j].fy + y_step;
        portals_vert[i++] = portals[j].fx   - x_step;
        portals_vert[i++] = portals[j].fy + y_step;
        portals_vert[i++] = portals[j].fx + x_step;
        portals_vert[i++] = portals[j].fy   - y_step;
        portals_vert[i++] = portals[j].fx + x_step;
        portals_vert[i++] = portals[j].fy + y_step;
    }

    glGenVertexArrays(1, &portals_vao);
    glBindVertexArray(portals_vao);

    glGenBuffers(1, &portals_vert_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, portals_vert_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 2 * 6,
        portals_vert, GL_STATIC_DRAW);
    GLint att_portal_pos_loc = glGetAttribLocation(gc_shader, "att_pos");
    glVertexAttribPointer(att_portal_pos_loc, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(att_portal_pos_loc);

    glGenBuffers(1, &portals_texc_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, portals_texc_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 2 * 6,
        NULL, GL_DYNAMIC_DRAW);
    GLint att_portal_texc_loc = glGetAttribLocation(gc_shader, "att_texc");
    glVertexAttribPointer(att_portal_texc_loc, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(att_portal_texc_loc);

    glBindVertexArray(0);
}
static void portals_terminate(void) {
    if(!portals_initialized) return;
    glDeleteBuffers(1, &portals_vert_vbo);
    glDeleteBuffers(1, &portals_texc_vbo);
    glDeleteVertexArrays(1, &portals_vao);
}


static void portals_update_texc(void) {
    uint maxx = particle_sprite->count_x;
    uint maxy = particle_sprite->count_y;
    uint mult = particle_sprite->pixs;

    float fmultx = (float)mult / (float)particle_tex->x;
    float fmulty = (float)mult / (float)particle_tex->y;
    uint x,y;
    uint i;
    for(i = 0; i < 4 * 2 * 6;) {
        get_tile_particle(PART_PORTAL, &x, &y);

        //reverse:
        portals_texc[i++] = ((x % maxx)   ) * fmultx;
        portals_texc[i++] = ((y % maxy) +1) * fmulty;
        portals_texc[i++] = ((x % maxx) +1) * fmultx;
        portals_texc[i++] = ((y % maxy) +1) * fmulty;
        portals_texc[i++] = ((x % maxx)   ) * fmultx;
        portals_texc[i++] = ((y % maxy)   ) * fmulty;
        portals_texc[i++] = ((x % maxx)   ) * fmultx;
        portals_texc[i++] = ((y % maxy)   ) * fmulty;
        portals_texc[i++] = ((x % maxx) +1) * fmultx;
        portals_texc[i++] = ((y % maxy) +1) * fmulty;
        portals_texc[i++] = ((x % maxx) +1) * fmultx;
        portals_texc[i++] = ((y % maxy)   ) * fmulty;
    }
    glBindVertexArray(portals_vao);
    glBindBuffer(GL_ARRAY_BUFFER, portals_texc_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 2 * 6,
        portals_texc, GL_DYNAMIC_DRAW);
}
