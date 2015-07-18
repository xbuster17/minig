#include "gc_players.h"
#include "gc_sound.h"


gridling_t players[NUM_MAX_PLAYERS];

float zoom = 3.0;
GLuint uni_cam_loc;
GLuint uni_zoom_loc;
float cam_x = 0.0;
float cam_y = 0.0;

GLuint players_vao;
float* players_vert = NULL;
float* players_texc = NULL;
GLuint players_vert_vbo;
GLuint players_texc_vbo;

bool player_warp_left = false;
bool player_warp_right = false;
bool player_warp_up = false;
bool player_warp_down = false;


void players_init(void) {
    // if(players_vert || players_texc) return;
    players_vert = malloc(sizeof(float) * 2 * 6 * NUM_MAX_PLAYERS);
    players_texc = malloc(sizeof(float) * 2 * 6 * NUM_MAX_PLAYERS);
    uint i;
    for(i = 0; i <  2 * 6 * NUM_MAX_PLAYERS; i++) {
        players_vert[i] = 0.0;
        players_texc[i] = 0.0;
    }
    gridlings_stats_set();

    for(i = 0; i < NUM_MAX_PLAYERS; i++){
        players[i] = (gridling_t){0};
    }

    glGenVertexArrays(1, &players_vao);
    glBindVertexArray(players_vao);

    glGenBuffers(1, &players_vert_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, players_vert_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * 6 * NUM_MAX_PLAYERS,
        NULL, GL_DYNAMIC_DRAW);
    GLint att_players_vert_loc = glGetAttribLocation(gc_shader, "att_pos");
    glVertexAttribPointer(att_players_vert_loc, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(att_players_vert_loc);

    glGenBuffers(1, &players_texc_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, players_texc_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * 6 * NUM_MAX_PLAYERS,
        NULL, GL_DYNAMIC_DRAW);
    GLint att_players_texc_loc = glGetAttribLocation(gc_shader, "att_texc");
    glVertexAttribPointer(att_players_texc_loc, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(att_players_texc_loc);

    glBindVertexArray(0);

    players[0].type = GRIDLING_P1;
    players[1].type = GRIDLING_P2;
    gridling_set_default(&players[0]);
    gridling_set_default(&players[1]);
}

void players_quit(void) {
    // if(!players_vert || !players_texc) return;
    free(players_vert); players_vert = NULL;
    free(players_texc); players_texc = NULL;

    glDeleteBuffers(1, &players_vert_vbo);
    glDeleteBuffers(1, &players_texc_vbo);
    glDeleteVertexArrays(1, &players_vao);
}

void players_update_data(void) {
    uint i; float prev_x, prev_y;
    for(i = 0; i < NUM_MAX_PLAYERS; i++) {
        // if(gridlings[i].type != GRIDLING_NULL) {
            prev_x = players[i].fx;
            players[i].fx += players[i].dir_x ;
            prev_y = players[i].fy;
            players[i].fy += players[i].dir_y ;

            players[i].ix = floor(((players[i].fx + 1.0) / 2.0) * grid_size_x);
            players[i].ix %= grid_size_x;
            players[i].iy = floor(((players[i].fy + 1.0) / 2.0) * grid_size_y);
            players[i].iy %= grid_size_y;
            if(terrain_collision(players[i].ix, players[i].iy)) {
                players[i].fx = prev_x;
                players[i].fy = prev_y;
            }
        // }
    }
}

void players_update_vertices(void) {
    double x_step = (2.0 / grid_size_x) * 1.5;
    double y_step = (2.0 / grid_size_y) * 1.5;
    uint i = 0;
    uint j = 0;
    for(j = 0; j < NUM_MAX_PLAYERS; j++) {
        // if(gridlings[j].type == GRIDLING_NULL) {
        //     i += 12; continue;
        // }
        players_vert[i++] = players[j].fx   - x_step;
        players_vert[i++] = players[j].fy   - y_step;
        players_vert[i++] = players[j].fx + x_step;
        players_vert[i++] = players[j].fy   - y_step;
        players_vert[i++] = players[j].fx   - x_step;
        players_vert[i++] = players[j].fy + y_step;
        players_vert[i++] = players[j].fx   - x_step;
        players_vert[i++] = players[j].fy + y_step;
        players_vert[i++] = players[j].fx + x_step;
        players_vert[i++] = players[j].fy   - y_step;
        players_vert[i++] = players[j].fx + x_step;
        players_vert[i++] = players[j].fy + y_step;
    }
    uint maxx = particle_sprite->count_x;
    uint maxy = particle_sprite->count_y;
    uint mult = particle_sprite->pixs;

    float fmultx = (float)mult / (float)particle_tex->x;
    float fmulty = (float)mult / (float)particle_tex->y;
    uint x, y;
    for(i = 0, j = 0; i < NUM_MAX_PLAYERS * 2 * 6; j++) {
        // if(gridlings[j].type == GRIDLING_NULL) {
        //     i += 12; continue;
        // }
        get_tile_particle(players[j].sprite_set, &x, &y);

        //reverse:
        players_texc[i++] = ((x % maxx)   ) * fmultx;
        players_texc[i++] = ((y % maxy) +1) * fmulty;
        players_texc[i++] = ((x % maxx) +1) * fmultx;
        players_texc[i++] = ((y % maxy) +1) * fmulty;
        players_texc[i++] = ((x % maxx)   ) * fmultx;
        players_texc[i++] = ((y % maxy)   ) * fmulty;
        players_texc[i++] = ((x % maxx)   ) * fmultx;
        players_texc[i++] = ((y % maxy)   ) * fmulty;
        players_texc[i++] = ((x % maxx) +1) * fmultx;
        players_texc[i++] = ((y % maxy) +1) * fmulty;
        players_texc[i++] = ((x % maxx) +1) * fmultx;
        players_texc[i++] = ((y % maxy)   ) * fmulty;
    }
    glBindVertexArray(players_vao);

    glBindBuffer(GL_ARRAY_BUFFER, players_vert_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * 6 * NUM_MAX_PLAYERS,
        players_vert, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, players_texc_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * 6 * NUM_MAX_PLAYERS,
        players_texc, GL_DYNAMIC_DRAW);

    glBindVertexArray(0);
}

static uint player1_reloading = 1;
static uint player2_reloading = 1;
static float prev_cam_x;
static float prev_cam_y;
// static float cam_x = 0.0;
// static float cam_y = 0.0;
void players_update(void) {
    float prev_x, prev_y;
    prev_cam_x = (players[1].fx + players[0].fx) / 2.0;
    prev_cam_y = (players[1].fy + players[0].fy) / 2.0;

    prev_x = players[0].fx;
    prev_y = players[0].fy;

    players[0].fx += (Touch.a[0]->dx * MAX_MOVE_SPEED*.15/*0.0025*/)/* * players[0].speed*/; // TEMP
    players[0].fx += kbk(SDLK_LSHIFT) ? kbk('d') * MAX_MOVE_SPEED*.05 : kbk('d') * MAX_MOVE_SPEED*.15;
    players[0].fx -= kbk(SDLK_LSHIFT) ? kbk('a') * MAX_MOVE_SPEED*.05 : kbk('a') * MAX_MOVE_SPEED*.15;
    if(players[0].fx >  1.0) players[0].fx =  1.0;
    if(players[0].fx < -1.0) players[0].fx = -1.0;

    players[0].fy += (Touch.a[0]->dy * MAX_MOVE_SPEED*.15/*0.0025*/)/* * players[0].speed*/; // TEMP
    players[0].fy += kbk(SDLK_LSHIFT) ? kbk('w') * MAX_MOVE_SPEED*.05 : kbk('w') * MAX_MOVE_SPEED*.15;
    players[0].fy -= kbk(SDLK_LSHIFT) ? kbk('s') * MAX_MOVE_SPEED*.05 : kbk('s') * MAX_MOVE_SPEED*.15;
    if(players[0].fy >  1.0) players[0].fy =  1.0;
    if(players[0].fy < -1.0) players[0].fy = -1.0;

    players[0].ix = floor(((players[0].fx + 1.0) / 2.0) * grid_size_x);
    players[0].ix %= grid_size_x;
    players[0].iy = floor(((players[0].fy + 1.0) / 2.0) * grid_size_y);
    players[0].iy %= grid_size_y;

    if(terrain_collision(players[0].ix, players[0].iy))
        player_handle_collision(0, prev_x, prev_y);

    //2nd player
#if defined(ANDROID_2P_TEST)
    prev_x = players[1].fx;
    prev_y = players[1].fy;

    players[1].fx += (Touch.a[2]->dx * 0.0025) * players[1].speed;
    players[1].fx += kbk(SDLK_LSHIFT) ? kbk(SDLK_RIGHT) * MAX_MOVE_SPEED*.05 : kbk(SDLK_RIGHT) * MAX_MOVE_SPEED*.15;
    players[1].fx -= kbk(SDLK_LSHIFT) ? kbk(SDLK_LEFT)  * MAX_MOVE_SPEED*.05 : kbk(SDLK_LEFT)  * MAX_MOVE_SPEED*.15;
    if(players[1].fx >  1.0) players[1].fx =  1.0;
    if(players[1].fx < -1.0) players[1].fx = -1.0;

    players[1].fy += (Touch.a[2]->dy * 0.0025) * players[1].speed;
    players[1].fy += kbk(SDLK_LSHIFT) ? kbk(SDLK_UP)   * MAX_MOVE_SPEED*.05 : kbk(SDLK_UP)   * MAX_MOVE_SPEED*.15;
    players[1].fy -= kbk(SDLK_LSHIFT) ? kbk(SDLK_DOWN) * MAX_MOVE_SPEED*.05 : kbk(SDLK_DOWN) * MAX_MOVE_SPEED*.15;
    if(players[1].fy >  1.0) players[1].fy =  1.0;
    if(players[1].fy < -1.0) players[1].fy = -1.0;

    players[1].ix = floor(((players[1].fx + 1.0) / 2.0) * grid_size_x);
    players[1].ix %= grid_size_x;
    players[1].iy = floor(((players[1].fy + 1.0) / 2.0) * grid_size_y);
    players[1].iy %= grid_size_y;

    if(terrain_collision(players[1].ix, players[1].iy))
        player_handle_collision(1, prev_x, prev_y);
#else
    players[1].fx =players[0].fx;
    players[1].fy =players[0].fy;
#endif

    //camera
    float p_dist_x = (players[1].fx + players[0].fx) / 2.0;
    float p_dist_y = (players[1].fy + players[0].fy) / 2.0;
    cam_x += (p_dist_x - cam_x) * 0.05;
    cam_y += (p_dist_y - cam_y) * 0.05;

    glUniform2f(uni_cam_loc, cam_x, cam_y);

    //update_zoom();
    #ifdef ANDROID
    #define PLAYERS_TEMP_TEST 1
    #else
    #define PLAYERS_TEMP_TEST 0
    #endif
        if(PLAYERS_TEMP_TEST) { //two players TODO
            float scale = (float)Mi.sx / (float)Mi.sy;
            float norm = sqrt(pow(players[0].fx - players[1].fx, 2) +
                              pow(players[0].fy * scale - players[1].fy * scale, 2));

            zoom = 3.0 - norm;
        } else {
            if(kbk('+')) zoom += 0.01;
            if(kbk('-')) zoom -= 0.01;
        }
        if(zoom < 1.0) zoom = 1.0;
        if(zoom > 8.0) zoom = 8.0;
        glUniform2f(uni_zoom_loc, zoom, zoom);

    //shoot
    if (!player1_reloading) {
        if(Touch.a[1]->dx || Touch.a[1]->dy) {
            projectile_spawn(true, PROJECTILE_12,
                players[0].fx, players[0].fy,
                Touch.a[1]->dx * 0.5, Touch.a[1]->dy * 0.5 //TEMP
            );
            player1_reloading += 0;
            play_sound();
        } else if(Mouse.b1) {
            projectile_spawn(true, PROJECTILE_12,
                players[0].fx, players[0].fy,
                 ((((float)Mouse.x / (float)Mi.sx) * 2.0) - 1.0) * .5,
                -((((float)Mouse.y / (float)Mi.sy) * 2.0) - 1.0) * .5
            );
            player1_reloading += 0;
            play_sound();
        }
    }
    else player1_reloading--;

    if (!player2_reloading) {
        if(Touch.a[3]->dx || Touch.a[3]->dy) {
            projectile_spawn(true, PROJECTILE_12,
                players[1].fx, players[1].fy,
                Touch.a[3]->dx * 0.5, Touch.a[3]->dy * 0.5 // TEMP
            );
            player2_reloading += 2;
        } else if(Mouse.b2) {
            projectile_spawn(true, PROJECTILE_12,
                players[1].fx, players[1].fy,
                 ((((float)Mouse.x / (float)Mi.sx) * 2.0) - 1.0) * .5,
                -((((float)Mouse.y / (float)Mi.sy) * 2.0) - 1.0) * .5
            );
            player2_reloading += 2;
        }
    }
    else player2_reloading--;

    //get hit
    uint i;
    for(i = 0; i < NUM_MAX_GRIDLINGS; i++) {
        if(gridlings[i].type != GRIDLING_NULL) {
            if((gridlings[i].ix == players[0].ix) &&
                (gridlings[i].iy == players[0].iy)) {
                    players[0].hp -= gridlings[i].power;
                    prev_x = players[0].fx;
                    prev_y = players[0].fy;
                    players[0].fx += gridlings[i].dir_x;
                    players[0].fy += gridlings[i].dir_y;
                    players[0].ix = floor(((players[0].fx + 1.0) / 2.0) * grid_size_x);
                    players[0].ix %= grid_size_x;
                    players[0].iy = floor(((players[0].fy + 1.0) / 2.0) * grid_size_y);
                    players[0].iy %= grid_size_y;
                    if(terrain_collision(players[0].ix, players[0].iy))
                        player_handle_collision(0, prev_x, prev_y);
            }
            if((gridlings[i].ix == players[1].ix) &&
                (gridlings[i].iy == players[1].iy)) {
                    players[1].hp -= gridlings[i].power;
                    prev_x = players[1].fx;
                    prev_y = players[1].fy;
                    players[1].fx += gridlings[i].dir_x;
                    players[1].fy += gridlings[i].dir_y;
                    players[1].ix = floor(((players[1].fx + 1.0) / 2.0) * grid_size_x);
                    players[1].ix %= grid_size_x;
                    players[1].iy = floor(((players[1].fy + 1.0) / 2.0) * grid_size_y);
                    players[1].iy %= grid_size_y;
                    if(terrain_collision(players[1].ix, players[1].iy))
                        player_handle_collision(1, prev_x, prev_y);

            }
        }
    }

    //change level
    if( //level left
        (players[0].ix <= 1) &&
        (players[0].iy <= (grid_size_y / 2) + 2) &&
        (players[0].iy >= (grid_size_y / 2) - 2)) {
            player_warp_left = true;
            change_level(--level_x, level_y);
        }
    if( //level right
        (players[0].ix >= grid_size_x - 2) &&
        (players[0].iy <= (grid_size_y / 2) + 2) &&
        (players[0].iy >= (grid_size_y / 2) - 2)) {
            player_warp_right = true;
            change_level(++level_x, level_y);
        }
    if( //level up
        (players[0].iy >= grid_size_y - 2) &&
        (players[0].ix <= (grid_size_x / 2) + 2) &&
        (players[0].ix >= (grid_size_x / 2) - 2)) {
            player_warp_up = true;
            change_level(level_x, ++level_y);
        }
    if( //level down
        (players[0].iy <= 1) &&
        (players[0].ix <= (grid_size_x / 2) + 2) &&
        (players[0].ix >= (grid_size_x / 2) - 2)) {
            player_warp_down = true;
            change_level(level_x, --level_y);
        }


    players_update_data();
    players_update_vertices();
}


void player_handle_collision(uint i, float prev_x, float prev_y) {
    gridling_handle_collision(players, i, prev_x, prev_y);
}

