#include "gc_projectiles.h"
#include "gc_players.h"
#include "minig.h"

float* projectiles_vert = NULL;
float* projectiles_texc = NULL;
GLuint projectiles_vao;
GLuint projectiles_vert_vbo;
GLuint projectiles_texc_vbo;
uint num_active_projectiles = 0;

projectile_t projectiles[NUM_MAX_PROJECTILES];

void projectiles_update(void) {
    uint ix;
    uint iy;
    uint i; uint j;
    for(i = 0; i < NUM_MAX_PROJECTILES; i++) {
        if(projectiles[i].type == PROJECTILE_NULL) continue;
        ix = projectiles[i].ix;
        iy = projectiles[i].iy;
        projectiles[i].reach -= fabs(projectiles[i].dir_x);
        projectiles[i].reach -= fabs(projectiles[i].dir_y);
        if(projectiles[i].reach <= 0.0) {
            projectiles[i].type = PROJECTILE_NULL;
            continue;
        }
        if(projectiles[i].owned) {
            for(j = 0; j < NUM_MAX_GRIDLINGS; j++) {
                if(gridlings[j].type == GRIDLING_NULL) continue;
                if(ix == gridlings[j].ix) {
                    if(iy == gridlings[j].iy) {
                        gridlings[j].hp -= projectiles[i].power;
                        projectile_handle_collision(i);
                        projectiles[i].type = PROJECTILE_NULL;
                    }
                }
            }
        } else {
            for(j = 0; j < NUM_MAX_PLAYERS; j++) {
                if(ix == players[j].ix)
                    if(iy == players[j].iy) {
                        float prev_x = players[j].fx;
                        float prev_y = players[j].fy;
                        players[j].hp -= projectiles[i].power;
                        players[j].fx += projectiles[i].dir_x;
                        players[j].fy += projectiles[i].dir_y;
                        players[j].ix = floor(((players[j].fx + 1.0) / 2.0) * grid_size_x);
                        players[j].ix %= grid_size_x;
                        players[j].iy = floor(((players[j].fy + 1.0) / 2.0) * grid_size_y);
                        players[j].iy %= grid_size_y;
                        if(terrain_collision(players[j].ix, players[j].iy))
                            player_handle_collision(j, prev_x, prev_y);
                        projectile_handle_collision(i);
                        projectiles[i].type = PROJECTILE_NULL;
                    }
            }
        }
    }
    projectiles_update_data();
    projectiles_update_vertices();
}


void projectiles_init(void) {
    // if(projectiles_vert || projectiles_texc) return;
    projectiles_vert = malloc(sizeof(float) * 2 * 6 * NUM_MAX_PROJECTILES);
    projectiles_texc = malloc(sizeof(float) * 2 * 6 * NUM_MAX_PROJECTILES);
    uint i;
    for(i = 0; i <  2 * 6 * NUM_MAX_PROJECTILES; i++) {
        projectiles_vert[i] = 0.0;
        projectiles_texc[i] = 0.0;
    }

    projectiles_stats_set();

    for(i = 0; i < NUM_MAX_PROJECTILES; i++) {
        projectiles[i] = (projectile_t){0};
        projectiles[i].type = PROJECTILE_NULL;
    }
    glGenVertexArrays(1, &projectiles_vao);
    glBindVertexArray(projectiles_vao);

    glGenBuffers(1, &projectiles_vert_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, projectiles_vert_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * 6 * NUM_MAX_PROJECTILES,
        NULL, GL_DYNAMIC_DRAW);
    GLint att_projectiles_vert_loc = glGetAttribLocation(gc_shader, "att_pos");
    glVertexAttribPointer(att_projectiles_vert_loc, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(att_projectiles_vert_loc);

    glGenBuffers(1, &projectiles_texc_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, projectiles_texc_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * 6 * NUM_MAX_PROJECTILES,
        NULL, GL_DYNAMIC_DRAW);
    GLint att_projectiles_texc_loc = glGetAttribLocation(gc_shader, "att_texc");
    glVertexAttribPointer(att_projectiles_texc_loc, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(att_projectiles_texc_loc);

    glBindVertexArray(0);

}

void projectiles_quit(void) {
    // if(!projectiles_vert || !projectiles_texc) return;
    free(projectiles_vert); projectiles_vert = NULL;
    free(projectiles_texc); projectiles_texc = NULL;

    glDeleteBuffers(1, &projectiles_vert_vbo);
    glDeleteBuffers(1, &projectiles_texc_vbo);
    glDeleteVertexArrays(1, &projectiles_vao);
}

void projectiles_stats_set(void) {
    projectile_type_stats = (struct projectile_type_stats_s) {
        .p0_speed = 1.0,
        .p0_power = 1.0,
        .p0_sprite_set = PART_STAR,
        .p1_speed = 1.0,
        .p1_power = 1.0,
        .p1_sprite_set = PART_PLASMA3,
        .p2_speed = 1.0,
        .p2_power = 1.0,
        .p2_sprite_set = PART_STAR,
        .p3_speed = 1.0,
        .p3_power = 4.0,
        .p3_sprite_set = PART_PLASMA4,
        .p4_speed = 1.0,
        .p4_power = 1.0,
        .p4_sprite_set = PART_STAR,
        .p5_speed = 1.0,
        .p5_power = 1.0,
        .p5_sprite_set = PART_SUN,
        .p6_speed = 0.5,
        .p6_power = 1.0,
        .p6_sprite_set = PART_GREEN,
        .p7_speed = 1.0,
        .p7_power = 1.0,
        .p7_sprite_set = PART_SUN,
        .p8_speed = 1.0,
        .p8_power = 1.0,
        .p8_sprite_set = PART_SUN,
        .p9_speed = 1.0,
        .p9_power = 1.0,
        .p9_sprite_set = PART_PLASMA4,
        .p10_speed = 1.0,
        .p10_power = 1.0,
        .p10_sprite_set = PART_PLASMA4,
        .p11_speed = 1.0,
        .p11_power = 1.0,
        .p11_sprite_set = PART_PLASMA4,
        .p12_speed = 1.0,
        .p12_power = 1.0,
        .p12_sprite_set = PART_RAINBOW,
    };
}

void projectile_set_default(projectile_t* projectile) {
    switch(projectile->type){
        case PROJECTILE_0:
            projectile->sprite_set = projectile_type_stats.p0_sprite_set;
            projectile->power = projectile_type_stats.p0_power;
            projectile->speed = projectile_type_stats.p0_speed;
            break;
        case PROJECTILE_1:
            projectile->sprite_set = projectile_type_stats.p1_sprite_set;
            projectile->power = projectile_type_stats.p1_power;
            projectile->speed = projectile_type_stats.p1_speed;
            break;
        case PROJECTILE_2:
            projectile->sprite_set = projectile_type_stats.p2_sprite_set;
            projectile->power = projectile_type_stats.p2_power;
            projectile->speed = projectile_type_stats.p2_speed;
            break;
        case PROJECTILE_3:
            projectile->sprite_set = projectile_type_stats.p3_sprite_set;
            projectile->power = projectile_type_stats.p3_power;
            projectile->speed = projectile_type_stats.p3_speed;
            break;
        case PROJECTILE_4:
            projectile->sprite_set = projectile_type_stats.p4_sprite_set;
            projectile->power = projectile_type_stats.p4_power;
            projectile->speed = projectile_type_stats.p4_speed;
            break;
        case PROJECTILE_5:
            projectile->sprite_set = projectile_type_stats.p5_sprite_set;
            projectile->power = projectile_type_stats.p5_power;
            projectile->speed = projectile_type_stats.p5_speed;
            break;
        case PROJECTILE_6:
            projectile->sprite_set = projectile_type_stats.p6_sprite_set;
            projectile->power = projectile_type_stats.p6_power;
            projectile->speed = projectile_type_stats.p6_speed;
            break;
        case PROJECTILE_7:
            projectile->sprite_set = projectile_type_stats.p7_sprite_set;
            projectile->power = projectile_type_stats.p7_power;
            projectile->speed = projectile_type_stats.p7_speed;
            break;
        case PROJECTILE_8:
            projectile->sprite_set = projectile_type_stats.p8_sprite_set;
            projectile->power = projectile_type_stats.p8_power;
            projectile->speed = projectile_type_stats.p8_speed;
            break;
        case PROJECTILE_9:
            projectile->sprite_set = projectile_type_stats.p9_sprite_set;
            projectile->power = projectile_type_stats.p9_power;
            projectile->speed = projectile_type_stats.p9_speed;
            break;
        case PROJECTILE_10:
            projectile->sprite_set = projectile_type_stats.p10_sprite_set;
            projectile->power = projectile_type_stats.p10_power;
            projectile->speed = projectile_type_stats.p10_speed;
            break;
        case PROJECTILE_11:
            projectile->sprite_set = projectile_type_stats.p11_sprite_set;
            projectile->power = projectile_type_stats.p11_power;
            projectile->speed = projectile_type_stats.p11_speed;
            break;
        case PROJECTILE_12:
            projectile->sprite_set = projectile_type_stats.p12_sprite_set;
            projectile->power = projectile_type_stats.p12_power;
            projectile->speed = projectile_type_stats.p12_speed;
            break;
        case PROJECTILE_13:
            projectile->sprite_set = projectile_type_stats.p12_sprite_set;
            projectile->power = projectile_type_stats.p12_power;
            projectile->speed = projectile_type_stats.p12_speed;
            break;

        default: break;
    }
}

void projectile_spawn(bool owned, enum projectile_type_e type,
        float pos_x, float pos_y, float dir_x, float dir_y) {
    num_active_projectiles %= NUM_MAX_PROJECTILES;

    projectile_t projectile = {
        .owned = owned, .type = type, .fx = pos_x, .fy = pos_y,
        .dir_x = MAX_MOVE_SPEED * dir_x, .dir_y = MAX_MOVE_SPEED * dir_y,
    };

    if(owned) projectile.reach = PROJECTILE_DEF_REACH;
    else projectile.reach = PROJECTILE_DEF_REACH * 8;

    projectiles[num_active_projectiles] = projectile;

    projectile_set_default(&projectiles[num_active_projectiles]);


    num_active_projectiles++;
}


void projectiles_update_data(void) {
    uint i;

    for(i = 0; i < NUM_MAX_PROJECTILES; i++) {
        if(projectiles[i].type != PROJECTILE_NULL) {
            projectiles[i].fx += projectiles[i].dir_x * projectiles[i].speed;
            projectiles[i].fy += projectiles[i].dir_y * projectiles[i].speed;
            if(projectiles[i].fx >= 1.0) {
                projectiles[i].type = PROJECTILE_NULL;
                continue;
            }
            if(projectiles[i].fx <=-1.0) {
                projectiles[i].type = PROJECTILE_NULL;
                continue;
            }
            if(projectiles[i].fy >= 1.0) {
                projectiles[i].type = PROJECTILE_NULL;
                continue;
            }
            if(projectiles[i].fy <=-1.0) {
                projectiles[i].type = PROJECTILE_NULL;
                continue;
            }

            projectiles[i].ix = floor(((projectiles[i].fx + 1.0) / 2.0) * grid_size_x);
            projectiles[i].ix %= grid_size_x;
            projectiles[i].iy = floor(((projectiles[i].fy + 1.0) / 2.0) * grid_size_y);
            projectiles[i].iy %= grid_size_y;

            if(terrain_collision(projectiles[i].ix, projectiles[i].iy)) {
                if(
                    (projectiles[i].ix == 0) ||
                    (projectiles[i].ix == grid_size_x - 1) ||
                    (projectiles[i].iy == 0) ||
                    (projectiles[i].iy == grid_size_y - 1)
                    ) {
                    if(
                        (projectiles[i].ix == 0) ||
                        (projectiles[i].ix == grid_size_x - 1)) {
                            projectiles[i].dir_x = -projectiles[i].dir_x;
                            projectiles[i].fx += projectiles[i].dir_x;
                    }
                    else if(
                        (projectiles[i].iy == 0) ||
                        (projectiles[i].iy == grid_size_y - 1)) {
                            projectiles[i].dir_y = -projectiles[i].dir_y;
                            projectiles[i].fy += projectiles[i].dir_y;
                    }
                    continue;
                }
                projectile_handle_collision(i);
                projectiles[i].type = PROJECTILE_NULL;
            }
        }
    }
}

void projectiles_update_vertices(void) {
    double x_step = (2.0 / grid_size_x) * 0.75;
    double y_step = (2.0 / grid_size_y) * 0.75;
    uint i = 0;
    uint j = 0;
    for(i = 0, j = 0; j < NUM_MAX_PROJECTILES; j++) {
        if(projectiles[j].type == PROJECTILE_NULL) {
            if(projectiles[j].fx != VOID_POSITION) {
                projectiles[j].fx = VOID_POSITION;
                projectiles[j].fy = VOID_POSITION;
                projectiles_vert[i++] = VOID_POSITION;
                projectiles_vert[i++] = VOID_POSITION;
                projectiles_vert[i++] = VOID_POSITION;
                projectiles_vert[i++] = VOID_POSITION;
                projectiles_vert[i++] = VOID_POSITION;
                projectiles_vert[i++] = VOID_POSITION;
                projectiles_vert[i++] = VOID_POSITION;
                projectiles_vert[i++] = VOID_POSITION;
                projectiles_vert[i++] = VOID_POSITION;
                projectiles_vert[i++] = VOID_POSITION;
                projectiles_vert[i++] = VOID_POSITION;
                projectiles_vert[i++] = VOID_POSITION;
                continue;
            }
            i += 12;
            continue;
        }
        projectiles_vert[i++] = projectiles[j].fx   - x_step;
        projectiles_vert[i++] = projectiles[j].fy   - y_step;
        projectiles_vert[i++] = projectiles[j].fx + x_step;
        projectiles_vert[i++] = projectiles[j].fy   - y_step;
        projectiles_vert[i++] = projectiles[j].fx   - x_step;
        projectiles_vert[i++] = projectiles[j].fy + y_step;
        projectiles_vert[i++] = projectiles[j].fx   - x_step;
        projectiles_vert[i++] = projectiles[j].fy + y_step;
        projectiles_vert[i++] = projectiles[j].fx + x_step;
        projectiles_vert[i++] = projectiles[j].fy   - y_step;
        projectiles_vert[i++] = projectiles[j].fx + x_step;
        projectiles_vert[i++] = projectiles[j].fy + y_step;
    }
    uint maxx = particle_sprite->count_x;
    uint maxy = particle_sprite->count_y;
    uint mult = particle_sprite->pixs;

    float fmultx = (float)mult / (float)particle_tex->x;
    float fmulty = (float)mult / (float)particle_tex->y;
    uint x, y;
    for(i = 0, j = 0; i < NUM_MAX_PROJECTILES * 2 * 6; j++) {
        if(projectiles[j].type == PROJECTILE_NULL) {
            i += 12; continue;
        }
        get_tile_particle(projectiles[j].sprite_set, &x, &y);

        // projectiles_texc[i++] = ((x % maxx)   ) * fmultx;
        // projectiles_texc[i++] = ((y % maxy)   ) * fmulty;
        // projectiles_texc[i++] = ((x % maxx) +1) * fmultx;
        // projectiles_texc[i++] = ((y % maxy)   ) * fmulty;
        // projectiles_texc[i++] = ((x % maxx)   ) * fmultx;
        // projectiles_texc[i++] = ((y % maxy) +1) * fmulty;
        // projectiles_texc[i++] = ((x % maxx)   ) * fmultx;
        // projectiles_texc[i++] = ((y % maxy) +1) * fmulty;
        // projectiles_texc[i++] = ((x % maxx) +1) * fmultx;
        // projectiles_texc[i++] = ((y % maxy)   ) * fmulty;
        // projectiles_texc[i++] = ((x % maxx) +1) * fmultx;
        // projectiles_texc[i++] = ((y % maxy) +1) * fmulty;

        x += j;
        //reverse:
        projectiles_texc[i++] = ((x % maxx)   ) * fmultx;
        projectiles_texc[i++] = ((y % maxy) +1) * fmulty;
        projectiles_texc[i++] = ((x % maxx) +1) * fmultx;
        projectiles_texc[i++] = ((y % maxy) +1) * fmulty;
        projectiles_texc[i++] = ((x % maxx)   ) * fmultx;
        projectiles_texc[i++] = ((y % maxy)   ) * fmulty;
        projectiles_texc[i++] = ((x % maxx)   ) * fmultx;
        projectiles_texc[i++] = ((y % maxy)   ) * fmulty;
        projectiles_texc[i++] = ((x % maxx) +1) * fmultx;
        projectiles_texc[i++] = ((y % maxy) +1) * fmulty;
        projectiles_texc[i++] = ((x % maxx) +1) * fmultx;
        projectiles_texc[i++] = ((y % maxy)   ) * fmulty;


    }

    glBindVertexArray(projectiles_vao);

    glBindBuffer(GL_ARRAY_BUFFER, projectiles_vert_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * 6 * NUM_MAX_PROJECTILES,
        projectiles_vert, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, projectiles_texc_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * 6 * NUM_MAX_PROJECTILES,
        projectiles_texc, GL_DYNAMIC_DRAW);

    glBindVertexArray(0);
}

#define EXPLOSION_FRAMES 7
static uint explosion_walker_x = GRID_SIZE / 2;
static uint explosion_walker_y = GRID_SIZE / 2;
static uint explosion_steps;

void projectile_handle_collision(uint index) {


    switch(projectiles[index].type) {
        case PROJECTILE_NULL: return;
        case PROJECTILE_0: goto destroy_wall;
        case PROJECTILE_1: goto destroy_wall;
        case PROJECTILE_2: goto destroy_wall;
        case PROJECTILE_3: goto destroy_wall;
        case PROJECTILE_4: goto ricochet;
        case PROJECTILE_5:
            explosion_walker_x = projectiles[index].ix;
            explosion_walker_y = projectiles[index].iy;
            explosion_steps = 250;
            goto explode_wall;
        case PROJECTILE_6:
            explosion_walker_x = projectiles[index].ix;
            explosion_walker_y = projectiles[index].iy;
            explosion_steps = 250;
            goto explode_wall;
        case PROJECTILE_7:
            explosion_walker_x = projectiles[index].ix;
            explosion_walker_y = projectiles[index].iy;
            explosion_steps = 250;
            goto explode_wall;
        case PROJECTILE_8:
            explosion_walker_x = projectiles[index].ix;
            explosion_walker_y = projectiles[index].iy;
            explosion_steps = 250;
            goto explode_wall;
        case PROJECTILE_9:
            explosion_walker_x = projectiles[index].ix;
            explosion_walker_y = projectiles[index].iy;
            explosion_steps = 250;
            goto explode_wall;
        case PROJECTILE_10:
            explosion_walker_x = projectiles[index].ix;
            explosion_walker_y = projectiles[index].iy;
            explosion_steps = 250;
            goto explode_wall;
        case PROJECTILE_11:
            explosion_walker_x = projectiles[index].ix;
            explosion_walker_y = projectiles[index].iy;
            explosion_steps = 250;
            goto explode_wall;
        case PROJECTILE_12:
            explosion_walker_x = projectiles[index].ix;
            explosion_walker_y = projectiles[index].iy;
            explosion_steps = 250;
            goto destroy_wall;
        case PROJECTILE_13:
            explosion_walker_x = projectiles[index].ix;
            explosion_walker_y = projectiles[index].iy;
            explosion_steps = 250;
            goto explode_wall;
        default: return;
    }

    ricochet:; // FIXME
    uint k;
    for(k = 0; k < 10; k++) {
        projectile_spawn(true, PROJECTILE_9,
            projectiles[index].fx,
            projectiles[index].fy,
            asin(projectiles[index].dir_x),
            sin(projectiles[index].dir_y)
            );
    }

    explode_wall:;
    // explosion_walker_x = (explosion_walker_x % (grid_size_x - 2)) + 1;
    // explosion_walker_y = (explosion_walker_y % (grid_size_y - 2)) + 1;
    uint i; uint step; uint j;
    for(i = 0; (i < EXPLOSION_FRAMES) && explosion_steps; i++, explosion_steps--) {
        step = rand() % 4;
        switch(step) {
            case 0:explosion_walker_x > 2             ? explosion_walker_x-- : explosion_walker_x++; break;
            case 1:explosion_walker_x < grid_size_x-2 ? explosion_walker_x++ : explosion_walker_x--; break;
            case 2:explosion_walker_y > 2             ? explosion_walker_y-- : explosion_walker_y++; break;
            case 3:explosion_walker_y < grid_size_y-2 ? explosion_walker_y++ : explosion_walker_y--; break;
        }
        if(grid_data[explosion_walker_x * grid_size_y + explosion_walker_y] == TERR_ROCK)
            grid_data[explosion_walker_x * grid_size_y + explosion_walker_y] =
                TERR_RUBBLE;
        if(projectiles[index].owned) {
            for(j = 0; j < NUM_MAX_GRIDLINGS; j++) {
                if(
                    gridlings[j].ix == explosion_walker_x &&
                    gridlings[j].iy == explosion_walker_y) {
                        gridlings[j].hp -= projectiles[index].power;
                }
            }
        } else {
            for(j = 0; j < NUM_MAX_PLAYERS; j++) {
                if(
                    players[j].ix == projectiles[index].ix &&
                    players[j].iy == projectiles[index].iy) {
                        players[j].hp -= projectiles[index].power;
                }
            }
        }
    }

    destroy_wall:
    if(grid_data[projectiles[index].ix * grid_size_y + projectiles[index].iy] == TERR_ROCK)
        grid_data[projectiles[index].ix * grid_size_y + projectiles[index].iy] =
            TERR_RUBBLE;


    gc__grid_update_flag = true;
}
