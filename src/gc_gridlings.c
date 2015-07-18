#include "gc_gridlings.h"
// #include "minig.h"

gridling_t gridlings[NUM_MAX_GRIDLINGS];
float* gridlings_vert = NULL;
float* gridlings_texc = NULL;
uint num_active_gridlings = 0;
GLuint gridlings_vert_vbo;
GLuint gridlings_texc_vbo;
GLuint gridlings_vao;


void gridlings_init(void) {
    // if(gridlings_vert || gridlings_texc) return;
    gridlings_vert = malloc(sizeof(float) * 2 * 6 * NUM_MAX_GRIDLINGS);
    gridlings_texc = malloc(sizeof(float) * 2 * 6 * NUM_MAX_GRIDLINGS);
    uint i;
    for(i = 0; i <  2 * 6 * NUM_MAX_GRIDLINGS; i++) {
        gridlings_vert[i] = 0.0;
        gridlings_texc[i] = 0.0;
    }
    gridlings_stats_set();

    for(i = 0; i < NUM_MAX_GRIDLINGS; i++) {
        gridlings[i] = (gridling_t){0};
        gridlings[i].type = GRIDLING_NULL;
    }
    glGenVertexArrays(1, &gridlings_vao);
    glBindVertexArray(gridlings_vao);

    glGenBuffers(1, &gridlings_vert_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, gridlings_vert_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * 6 * NUM_MAX_GRIDLINGS,
        NULL, GL_DYNAMIC_DRAW);
    GLint att_gridlings_vert_loc = glGetAttribLocation(gc_shader, "att_pos");
    glVertexAttribPointer(att_gridlings_vert_loc, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(att_gridlings_vert_loc);

    glGenBuffers(1, &gridlings_texc_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, gridlings_texc_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * 6 * NUM_MAX_GRIDLINGS,
        NULL, GL_DYNAMIC_DRAW);
    GLint att_gridlings_texc_loc = glGetAttribLocation(gc_shader, "att_texc");
    glVertexAttribPointer(att_gridlings_texc_loc, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(att_gridlings_texc_loc);

    glBindVertexArray(0);
}

void gridlings_quit(void) {
    // if(!gridlings_vert || !gridlings_texc) return;
    free(gridlings_vert); gridlings_vert = NULL;
    free(gridlings_texc); gridlings_texc = NULL;

    glDeleteBuffers(1, &gridlings_vert_vbo);
    glDeleteBuffers(1, &gridlings_texc_vbo);
    glDeleteVertexArrays(1, &gridlings_vao);
}

void gridlings_update_data(void) {
    uint i; float prev_x, prev_y;
    for(i = 0; i < NUM_MAX_GRIDLINGS; i++) {
        if(gridlings[i].type != GRIDLING_NULL) {
            if(gridlings[i].hp <= 0.0) {
                gridlings[i].type = GRIDLING_NULL;
                continue;
            }

            prev_x = gridlings[i].fx;
            gridlings[i].fx += gridlings[i].dir_x * gridlings[i].speed;
            prev_y = gridlings[i].fy;
            gridlings[i].fy += gridlings[i].dir_y * gridlings[i].speed;

            gridlings[i].ix = floor(((gridlings[i].fx + 1.0) / 2.0) * grid_size_x);
            gridlings[i].ix %= grid_size_x;
            gridlings[i].iy = floor(((gridlings[i].fy + 1.0) / 2.0) * grid_size_y);
            gridlings[i].iy %= grid_size_y;

            if(terrain_collision(gridlings[i].ix, gridlings[i].iy)) {
                gridling_handle_collision(gridlings, i,
                    prev_x, prev_y);
            }
        }
    }
}

void gridlings_update_vertices(void) {
    env_particle_move += 0.5;

    double x_step = 2.0 / grid_size_x * 0.75;
    double y_step = 2.0 / grid_size_y * 0.75;
    uint i = 0;
    uint j = 0;
    for(j = 0; j < NUM_MAX_GRIDLINGS; j++) {
        if(gridlings[j].type == GRIDLING_NULL) {
            if(gridlings[j].fx != VOID_POSITION) {
                gridlings[j].fx = VOID_POSITION;
                gridlings[j].fy = VOID_POSITION;
                gridlings_vert[i++] = VOID_POSITION;
                gridlings_vert[i++] = VOID_POSITION;
                gridlings_vert[i++] = VOID_POSITION;
                gridlings_vert[i++] = VOID_POSITION;
                gridlings_vert[i++] = VOID_POSITION;
                gridlings_vert[i++] = VOID_POSITION;
                gridlings_vert[i++] = VOID_POSITION;
                gridlings_vert[i++] = VOID_POSITION;
                gridlings_vert[i++] = VOID_POSITION;
                gridlings_vert[i++] = VOID_POSITION;
                gridlings_vert[i++] = VOID_POSITION;
                gridlings_vert[i++] = VOID_POSITION;
                continue;
            } else i += 12; continue;
        }
        gridlings_vert[i++] = gridlings[j].fx   - x_step;
        gridlings_vert[i++] = gridlings[j].fy   - y_step;
        gridlings_vert[i++] = gridlings[j].fx + x_step;
        gridlings_vert[i++] = gridlings[j].fy   - y_step;
        gridlings_vert[i++] = gridlings[j].fx   - x_step;
        gridlings_vert[i++] = gridlings[j].fy + y_step;
        gridlings_vert[i++] = gridlings[j].fx   - x_step;
        gridlings_vert[i++] = gridlings[j].fy + y_step;
        gridlings_vert[i++] = gridlings[j].fx + x_step;
        gridlings_vert[i++] = gridlings[j].fy   - y_step;
        gridlings_vert[i++] = gridlings[j].fx + x_step;
        gridlings_vert[i++] = gridlings[j].fy + y_step;
    }
    uint maxx = particle_sprite->count_x;
    uint maxy = particle_sprite->count_y;
    uint mult = particle_sprite->pixs;

    float fmultx = (float)mult / (float)particle_tex->x;
    float fmulty = (float)mult / (float)particle_tex->y;
    uint x, y;
    for(i = 0, j = 0; i < NUM_MAX_GRIDLINGS * 2 * 6; j++) {
        if(gridlings[j].type == GRIDLING_NULL) {
            i += 12; continue;
        }
        get_tile_particle(gridlings[j].sprite_set, &x, &y);

        x+=j;
        //reverse:
        gridlings_texc[i++] = ((x % maxx)   ) * fmultx;
        gridlings_texc[i++] = ((y % maxy) +1) * fmulty;
        gridlings_texc[i++] = ((x % maxx) +1) * fmultx;
        gridlings_texc[i++] = ((y % maxy) +1) * fmulty;
        gridlings_texc[i++] = ((x % maxx)   ) * fmultx;
        gridlings_texc[i++] = ((y % maxy)   ) * fmulty;
        gridlings_texc[i++] = ((x % maxx)   ) * fmultx;
        gridlings_texc[i++] = ((y % maxy)   ) * fmulty;
        gridlings_texc[i++] = ((x % maxx) +1) * fmultx;
        gridlings_texc[i++] = ((y % maxy) +1) * fmulty;
        gridlings_texc[i++] = ((x % maxx) +1) * fmultx;
        gridlings_texc[i++] = ((y % maxy)   ) * fmulty;
    }
    glBindVertexArray(gridlings_vao);
    glBindBuffer(GL_ARRAY_BUFFER, gridlings_vert_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * 6 * NUM_MAX_GRIDLINGS,
        gridlings_vert, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, gridlings_texc_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * 6 * NUM_MAX_GRIDLINGS,
        gridlings_texc, GL_DYNAMIC_DRAW);
    glBindVertexArray(0);
}

void gridlings_update(void) {
    gridlings_ai_tick();

    gridlings_update_data();
    gridlings_update_vertices();
}

void gridling_handle_collision(gridling_t* array, uint i,
    float prev_x, float prev_y
        ) {
    uint prev_test = floor(((prev_x + 1.0) / 2.0) * grid_size_x);
    uint test =      floor(((array[i].fx + 1.0) / 2.0) * grid_size_x);
    bool done = false;

    if(array[i].fx > prev_x) {
        if (terrain_collision(
            (((prev_x +1.0) / 2.0) * grid_size_x + 1),
            (((prev_y +1.0) / 2.0) * grid_size_y)
            )
        ) {
            prev_test = floor(((prev_x + 1.0) / 2.0) * grid_size_x);
            test =      floor(((array[i].fx + 1.0) / 2.0) * grid_size_x);
            if(prev_test != test) {
                array[i].fx = prev_x;
                array[i].ix = floor(((array[i].fx + 1.0) / 2.0) * grid_size_x);
                array[i].ix %= grid_size_x;
                done = true;
            }
        }
    }

    if(array[i].fx < prev_x) {
        if (terrain_collision(
            (((prev_x +1.0) / 2.0) * grid_size_x - 1),
            (((prev_y +1.0) / 2.0) * grid_size_y)
            )
        ) {
            prev_test = floor(((prev_x + 1.0) / 2.0) * grid_size_x);
            test =      floor(((array[i].fx + 1.0) / 2.0) * grid_size_x);
            if(prev_test != test) {
                array[i].fx = prev_x;
                array[i].ix = floor(((array[i].fx + 1.0) / 2.0) * grid_size_x);
                array[i].ix %= grid_size_x;
                done = true;
            }
        }
    }

    if(array[i].fy > prev_y) {
        if (terrain_collision(
            (((prev_x +1.0) / 2.0) * grid_size_x),
            (((prev_y +1.0) / 2.0) * grid_size_y + 1)
            )
        ) {
            prev_test = floor(((prev_y + 1.0) / 2.0) * grid_size_y);
            test =      floor(((array[i].fy + 1.0) / 2.0) * grid_size_y);
            if(prev_test != test) {
                array[i].fy = prev_y;
                array[i].iy = floor(((array[i].fy + 1.0) / 2.0) * grid_size_y);
                array[i].iy %= grid_size_y;
                done = true;
            }
        }
    }

    if(array[i].fy < prev_y) {
        if (terrain_collision(
            (((prev_x +1.0) / 2.0) * grid_size_x),
            (((prev_y +1.0) / 2.0) * grid_size_y - 1)
            )
        ) {
            prev_test = floor(((prev_y + 1.0) / 2.0) * grid_size_y);
            test =      floor(((array[i].fy + 1.0) / 2.0) * grid_size_y);
            if(prev_test != test) {
                array[i].fy = prev_y;
                array[i].iy = floor(((array[i].fy + 1.0) / 2.0) * grid_size_y);
                array[i].iy %= grid_size_y;
                done = true;
            }
        }
    }

    if(!done) {
        array[i].fx = prev_x;
        array[i].fy = prev_y;
        done = true;
    } // this handles corner case
}

static uint ai_tick0;
static uint ai_spawner_e1_tick = 0u;
static uint ai_spawner_e2_tick = 0u;
static uint ai_spawner_e3_tick = 0u;
static uint ai_spawner_e4_tick = 0u;
static bool level_clear;
void gridlings_ai_tick(void) {
    level_clear = true;
    uint i;
    for(i = 0; i < NUM_MAX_GRIDLINGS; i++) {
        if(gridlings[i].type != GRIDLING_NULL) {
            level_clear = false;
            switch(gridlings[i].type) {
                case GRIDLING_E1_SPAWNER:
                    if(!ai_spawner_e1_tick) {
                        if(num_active_gridlings < NUM_MAX_GRIDLINGS)
                            gridling_spawn(GRIDLING_E1,
                                gridlings[i].fx, gridlings[i].fy);
                        ai_spawner_e1_tick += 60*2;
                    }
                    else ai_spawner_e1_tick--;
                    break;
                case GRIDLING_E2_SPAWNER:
                    if(!ai_spawner_e2_tick) {
                        if(num_active_gridlings < NUM_MAX_GRIDLINGS)
                            gridling_spawn(GRIDLING_E2,
                                gridlings[i].fx, gridlings[i].fy);
                        ai_spawner_e2_tick += 95*2;
                    }
                    else ai_spawner_e2_tick--;
                    break;
                case GRIDLING_E3_SPAWNER:
                    if(!ai_spawner_e3_tick) {
                        if(num_active_gridlings < NUM_MAX_GRIDLINGS)
                            gridling_spawn(GRIDLING_E3,
                                gridlings[i].fx, gridlings[i].fy);
                        ai_spawner_e3_tick += 120*2;
                    }
                    else ai_spawner_e3_tick--;
                    break;
                case GRIDLING_E4_SPAWNER:
                    if(!ai_spawner_e4_tick) {
                        if(num_active_gridlings < NUM_MAX_GRIDLINGS)
                            gridling_spawn(GRIDLING_E4,
                                gridlings[i].fx, gridlings[i].fy);
                        ai_spawner_e4_tick += 150*2;
                    }
                    else ai_spawner_e4_tick--;
                    break;
                case GRIDLING_E1:
                    if(!gridlings[i].tick) {
                        if(
                            (abs(gridlings[i].ix - players[0].ix) < 50) &&
                            (abs(gridlings[i].iy - players[0].iy) < 50)
                            ) {
                            gridlings[i].dir_x = fmod(players[0].fx - gridlings[i].fx, MAX_MOVE_SPEED) * 0.2;
                            gridlings[i].dir_y = fmod(players[0].fy - gridlings[i].fy, MAX_MOVE_SPEED) * 0.2;
                            if(!(rand()%40)) projectile_spawn(false, PROJECTILE_6,
                                gridlings[i].fx, gridlings[i].fy,
                                (players[0].fx - gridlings[i].fx),
                                (players[0].fy - gridlings[i].fy)
                            );
                            gridlings[i].tick += 60;
                        } else {
                            gridlings[i].dir_x = (((rand() % 1000) / 500.0) -1.0) * 0.001;
                            gridlings[i].dir_y = (((rand() % 1000) / 500.0) -1.0) * 0.001;
                            gridlings[i].tick += 60;
                        }
                    } else gridlings[i].tick--;
                    break;
                case GRIDLING_E2:
                    if(!gridlings[i].tick) {
                        if(
                            (abs(gridlings[i].ix - players[0].ix) < 50) &&
                            (abs(gridlings[i].iy - players[0].iy) < 50)
                            ) {
                            gridlings[i].dir_x = fmod(players[0].fx - gridlings[i].fx, MAX_MOVE_SPEED);
                            gridlings[i].dir_y = fmod(players[0].fy - gridlings[i].fy, MAX_MOVE_SPEED);
                            gridlings[i].tick += 60;
                        } else {
                        gridlings[i].dir_x = (((rand() % 1000) / 500.0) -1.0) * 0.001;
                        gridlings[i].dir_y = (((rand() % 1000) / 500.0) -1.0) * 0.001;
                        gridlings[i].tick += 60;
                        }
                    } else gridlings[i].tick--;
                    break;
                case GRIDLING_E3:
                    if(!gridlings[i].tick) {
                                                if(
                            (abs(gridlings[i].ix - players[0].ix) < 50) &&
                            (abs(gridlings[i].iy - players[0].iy) < 50)
                            ) {
                            gridlings[i].dir_x = fmod(players[0].fx - gridlings[i].fx, MAX_MOVE_SPEED);
                            gridlings[i].dir_y = fmod(players[0].fy - gridlings[i].fy, MAX_MOVE_SPEED);
                            gridlings[i].tick += 60;
                        } else {
                        gridlings[i].dir_x = (((rand() % 1000) / 500.0) -1.0) * 0.001;
                        gridlings[i].dir_y = (((rand() % 1000) / 500.0) -1.0) * 0.001;
                        gridlings[i].tick += 60;
                        }
                    } else gridlings[i].tick--;
                    break;
                case GRIDLING_E4:
                    if(!gridlings[i].tick) {
                                                if(
                            (abs(gridlings[i].ix - players[0].ix) < 50) &&
                            (abs(gridlings[i].iy - players[0].iy) < 50)
                            ) {
                            gridlings[i].dir_x = fmod(players[0].fx - gridlings[i].fx, MAX_MOVE_SPEED);
                            gridlings[i].dir_y = fmod(players[0].fy - gridlings[i].fy, MAX_MOVE_SPEED);
                            gridlings[i].tick += 60;
                        } else {
                        gridlings[i].dir_x = (((rand() % 1000) / 500.0) -1.0) * 0.001;
                        gridlings[i].dir_y = (((rand() % 1000) / 500.0) -1.0) * 0.001;
                        gridlings[i].tick += 60;
                        }
                    } else gridlings[i].tick--;
                    break;
                default: break;
            }
        }
    }
    if(level_clear) {
        for(i = 0; i < NUM_MAX_PROJECTILES; i++) {
            if(!projectiles[i].owned) projectiles[i].type = PROJECTILE_NULL;
        }
    }
}

void gridlings_stats_set(void) {
    gridling_type_stats = (struct gridling_type_stats_s) {
        .p1_speed = 1.0,
        .p1_hp = 100.0,
        .p1_mp = 10.0,
        .p1_sprite_set = PART_PLAY1,
        .p2_sprite_set = PART_PLAY2,
        .e1_speed = 0.15,
        .e1_hp = 2.0,
        .e1_mp = 0.0,
        .e1_sprite_set = PART_PLASMA1,
        .e2_speed = 0.25,
        .e2_hp = 1.0,
        .e2_mp = 10.5,
        .e2_sprite_set = PART_PLASMA2,
        .e3_speed = 0.20,
        .e3_hp = 1.0,
        .e3_mp = 10.5,
        .e3_sprite_set = PART_PLASMA3,
        .e4_speed = 0.35,
        .e4_hp = 1.5,
        .e4_mp = 10.5,
        .e4_sprite_set = PART_PLASMA4,
        .exit_sprite_set = PART_PORTAL
    };
}

void gridling_set_default(gridling_t* gridling) {
    switch(gridling->type){
        case GRIDLING_P1:
            gridling->hp = gridling_type_stats.p1_hp;
            gridling->mp = gridling_type_stats.p1_mp;
            gridling->sprite_set = gridling_type_stats.p1_sprite_set;
            gridling->speed = gridling_type_stats.p1_speed;
            break;
        case GRIDLING_P2:
            gridling->hp = gridling_type_stats.p1_hp;
            gridling->mp = gridling_type_stats.p1_mp;
            gridling->sprite_set = gridling_type_stats.p2_sprite_set;
            gridling->speed = gridling_type_stats.p1_speed;
            break;
        case GRIDLING_E1:
            gridling->hp = gridling_type_stats.e1_hp;
            gridling->mp = gridling_type_stats.e1_mp;
            gridling->sprite_set = gridling_type_stats.e1_sprite_set;
            gridling->speed = gridling_type_stats.e1_speed;
            break;
        case GRIDLING_E2:
            gridling->hp = gridling_type_stats.e2_hp;
            gridling->mp = gridling_type_stats.e2_mp;
            gridling->sprite_set = gridling_type_stats.e2_sprite_set;
            gridling->speed = gridling_type_stats.e2_speed;
            break;
        case GRIDLING_E3:
            gridling->hp = gridling_type_stats.e3_hp;
            gridling->mp = gridling_type_stats.e3_mp;
            gridling->sprite_set = gridling_type_stats.e3_sprite_set;
            gridling->speed = gridling_type_stats.e3_speed;
            break;
        case GRIDLING_E4:
            gridling->hp = gridling_type_stats.e4_hp;
            gridling->mp = gridling_type_stats.e4_mp;
            gridling->sprite_set = gridling_type_stats.e4_sprite_set;
            gridling->speed = gridling_type_stats.e4_speed;
            break;
        case GRIDLING_E1_SPAWNER :
            gridling->hp = gridling_type_stats.e1_hp;
            gridling->mp = gridling_type_stats.e1_mp;
            gridling->sprite_set = PART_RING;
            gridling->speed = gridling_type_stats.e1_speed;
            break;
        case GRIDLING_E2_SPAWNER :
            gridling->hp = gridling_type_stats.e2_hp;
            gridling->mp = gridling_type_stats.e2_mp;
            gridling->sprite_set = PART_RING;
            gridling->speed = gridling_type_stats.e2_speed;
            break;
        case GRIDLING_E3_SPAWNER :
            gridling->hp = gridling_type_stats.e2_hp;
            gridling->mp = gridling_type_stats.e2_mp;
            gridling->sprite_set = PART_RING;
            gridling->speed = gridling_type_stats.e2_speed;
            break;
        case GRIDLING_E4_SPAWNER :
            gridling->hp = gridling_type_stats.e2_hp;
            gridling->mp = gridling_type_stats.e2_mp;
            gridling->sprite_set = PART_RING;
            gridling->speed = gridling_type_stats.e2_speed;
            break;
        default: break;
    }
}

void gridling_spawn(enum gridling_type_e type, float pos_x, float pos_y) {
    num_active_gridlings %= NUM_MAX_GRIDLINGS;

    gridling_t gridling = {
        .type = type, .fx = pos_x, .fy = pos_y, .tick = 0,
        .ix = floor(((pos_x + 1.0) / 2.0) * grid_size_x),
        .iy = floor(((pos_y + 1.0) / 2.0) * grid_size_y),
        .power = 1.f
    };
    gridlings[num_active_gridlings] = gridling;
    gridling_set_default(&gridlings[num_active_gridlings]);

    num_active_gridlings++;
}
