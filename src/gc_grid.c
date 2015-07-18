#include "gc_grid.h"
// #include "minig.h"

#ifndef GRID_SIZE
#   define GRID_SIZE 100
#endif

bool gc__grid_update_flag = true;

GLuint grid_vao;
float* grid_vert = NULL;
float* grid_texc = NULL;
uint* grid_data = NULL;
uint grid_size = GRID_SIZE * GRID_SIZE;
uint grid_size_x = GRID_SIZE;
uint grid_size_y = GRID_SIZE;
GLuint grid_vert_vbo = 0;
GLuint grid_texc_vbo = 0;

static uint x;
static uint y;

void grid_update_texc_vbo(void) {
    // glBindVertexArray(grid_vao);
    glBindBuffer(GL_ARRAY_BUFFER, grid_texc_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * grid_size * 2 * 6,
        grid_texc, GL_DYNAMIC_DRAW);
    // glBindVertexArray(0);
}


void grid_renew_data(void){
    uint i;
    for(i = 0; i < grid_size; i++) {
        grid_data[i] = TERR_ROCK;
    }
    grid_fill_data();
}

// void grid_update_tile(uint x, uint y, uint val){

// }

static float env_move;
void grid_update(void) {
    env_move += 0.15;
    uint maxx = terrain_sprite->count_x;
    uint maxy = terrain_sprite->count_y;
    uint mult = terrain_sprite->pixs;

    float half_texel_x = 0.00098 * 3.0; // TODO find propper value
    float half_texel_y = 0.00174 * 3.0; // TODO find propper value

    float fmultx = (float)mult / (float)terrain_tex->x;
    float fmulty = (float)mult / (float)terrain_tex->y;
    uint iset = 0;
    uint i; uint j;

    for(i = 0, j = 0; i < grid_size * 2 * 6; j++) {

        get_tile(iset++, &x, &y);

        grid_texc[i++] = (((x)   ) * fmultx) + half_texel_x;
        grid_texc[i++] = (((y) +1) * fmulty) - half_texel_y;
        grid_texc[i++] = (((x) +1) * fmultx) - half_texel_x;
        grid_texc[i++] = (((y) +1) * fmulty) - half_texel_y;
        grid_texc[i++] = (((x)   ) * fmultx) + half_texel_x;
        grid_texc[i++] = (((y)   ) * fmulty) + half_texel_y;
        grid_texc[i++] = (((x)   ) * fmultx) + half_texel_x;
        grid_texc[i++] = (((y)   ) * fmulty) + half_texel_y;
        grid_texc[i++] = (((x) +1) * fmultx) - half_texel_x;
        grid_texc[i++] = (((y) +1) * fmulty) - half_texel_y;
        grid_texc[i++] = (((x) +1) * fmultx) - half_texel_x;
        grid_texc[i++] = (((y)   ) * fmulty) + half_texel_y;

    }
    // glBindBuffer(GL_ARRAY_BUFFER, 0);

    grid_update_texc_vbo();
}

static uint grid_fill_steps = 5000;
void grid_fill_data(void) {
    uint drx = grid_size_x / 2; uint wdrx = drx;
    uint dry = grid_size_y / 2; uint wdry = dry;
    uint i; uint iw;
    uint step; uint stepw;

    drx = grid_size_x / 2; wdrx = drx;
    dry = grid_size_y / 2; wdry = dry;
    Uint8 wing;
    for(wing = 0; wing < 4; wing++) {
        switch(wing) {
            case 0:
                drx = grid_size_x / 2; wdrx = drx;
                dry = 2; wdry = dry;
                break;
            case 1:
                drx = grid_size_x / 2; wdrx = drx;
                dry = grid_size_y - 2; wdry = dry;
                break;
            case 2:
                drx = 2; wdrx = drx;
                dry = grid_size_y / 2; wdry = dry;
                break;
            case 3:
                drx = grid_size_x - 2; wdrx = drx;
                dry = grid_size_y / 2; wdry = dry;
                break;
            default: break;
        }
        for(i = 0; i < grid_fill_steps; i++) {
                step = rand() % 4;
                switch(step) {
                    case 0: drx > 1             ? drx-- : drx++; break;
                    case 1: drx < grid_size_x-2 ? drx++ : drx--; break;
                    case 2: dry > 1             ? dry-- : dry++; break;
                    case 3: dry < grid_size_y-2 ? dry++ : dry--; break;
            }
            if(i%150)
                grid_data[drx * grid_size_y + dry] = TERR_DIRT;
            else for(iw = 0; iw < grid_fill_steps/1000; iw++) {
                        stepw = rand() % 4;
                        switch(stepw) {
                            case 0: wdrx > 1             ? wdrx-- : wdrx++; break;
                            case 1: wdrx < grid_size_x-2 ? wdrx++ : wdrx--; break;
                            case 2: wdry > 1             ? wdry-- : wdry++; break;
                            case 3: wdry < grid_size_y-2 ? wdry++ : wdry--; break;
                        }
                        grid_data[wdrx * grid_size_y + wdry] = TERR_WATER;
            }
            if(!(i%151)) for(iw = 0; iw < grid_fill_steps/400; iw++) {
                    stepw = rand() % 4;
                    switch(stepw) {
                        case 0: drx > 1             ? drx-- : drx++; break;
                        case 1: drx < grid_size_x-2 ? drx++ : drx--; break;
                        case 2: dry > 1             ? dry-- : dry++; break;
                        case 3: dry < grid_size_y-2 ? dry++ : dry--; break;
                    }
                    grid_data[drx * grid_size_y + dry] = TERR_GRASS;
            }
        }
    }
    grid_data[(((grid_size_x / 2) ) * grid_size_y) + ((grid_size_y / 2) )] = TERR_PLASMA;
    grid_data[(((grid_size_x / 2) - 1) * grid_size_y) + ((grid_size_y / 2) - 1)] = TERR_PLASMA;
    grid_data[(((grid_size_x / 2) ) * grid_size_y) + ((grid_size_y / 2) - 1)] = TERR_PLASMA;
    grid_data[(((grid_size_x / 2) - 1) * grid_size_y) + ((grid_size_y / 2) )] = TERR_PLASMA;

}

static float env_move = 0.0;

void get_tile(uint i, uint* x, uint* y) {
    if(grid_data[i] == 0) {
        *x = 0; *y = 0;
    }
    uint move = floor(env_move);
    switch(grid_data[i]) {
        case TERR_DIRT:    *x = terrain_set % NUM_TERR_X; *y = 0; break;
        case TERR_ROCK:    *x = terrain_set % NUM_TERR_X; *y = 1; break;
        case TERR_RUBBLE:  *x = terrain_set % NUM_TERR_X; *y = 2; break;
        case TERR_UNBREAK: *x = terrain_set % NUM_TERR_X; *y = 3; break;
        case TERR_GRASS:   *x = move+i % NUM_TERR_X; *y = 4; break;
        case TERR_WATER:   *x = move   % NUM_TERR_X; *y = 5; break;
        case TERR_LAVA:    *x = move   % NUM_TERR_X; *y = 6; break;
        case TERR_PLASMA:  *x = move   % NUM_TERR_X; *y = 7; break;
        default: *x = 0; *y = 0; break;
    }
}


static uint x=0;
static uint y=0;
void grid_generate(void) {
    uint maxx = terrain_sprite->count_x;
    uint maxy = terrain_sprite->count_y;
    uint mult = terrain_sprite->pixs;

    // float half_texel = terrain_tex->x;

    float fmultx = (float)mult / (float)terrain_tex->x;
    float fmulty = (float)mult / (float)terrain_tex->y;

    grid_renew_data();
    uint i;


    uint iset = 0;
    for(i = 0; i < grid_size * 2 * 6;) {
        get_tile(iset++, &x, &y);

        grid_texc[i++] = (((x % maxx)   ) * fmultx);
        grid_texc[i++] = (((y % maxy) +1) * fmulty);
        grid_texc[i++] = (((x % maxx) +1) * fmultx);
        grid_texc[i++] = (((y % maxy) +1) * fmulty);
        grid_texc[i++] = (((x % maxx)   ) * fmultx);
        grid_texc[i++] = (((y % maxy)   ) * fmulty);
        grid_texc[i++] = (((x % maxx)   ) * fmultx);
        grid_texc[i++] = (((y % maxy)   ) * fmulty);
        grid_texc[i++] = (((x % maxx) +1) * fmultx);
        grid_texc[i++] = (((y % maxy) +1) * fmulty);
        grid_texc[i++] = (((x % maxx) +1) * fmultx);
        grid_texc[i++] = (((y % maxy)   ) * fmulty);
    }
    grid_update_texc_vbo();
}

void grid_create(void) {
    grid_vert = malloc(sizeof(float) * grid_size * 2 * 6);
    grid_texc = malloc(sizeof(float) * grid_size * 2 * 6);
    grid_data = malloc(sizeof(uint) * grid_size);

    float x_step = (2.0 / grid_size_x);
    float y_step = (2.0 / grid_size_y);

    double x,y = -1.0;
    uint i = 0;
    uint j,k = 0;
    for(x = -1.0, j = 0; j < (grid_size_x); x += x_step, ++j) {
        for(y = -1.0, k = 0; k < (grid_size_y); y += y_step, ++k) {

            grid_vert[i++] = (GLfloat)x;
            grid_vert[i++] = (GLfloat)y;
            grid_vert[i++] = (GLfloat)x + x_step;
            grid_vert[i++] = (GLfloat)y;
            grid_vert[i++] = (GLfloat)x;
            grid_vert[i++] = (GLfloat)y + y_step;

            grid_vert[i++] = (GLfloat)x;
            grid_vert[i++] = (GLfloat)y + y_step;
            grid_vert[i++] = (GLfloat)x + x_step;
            grid_vert[i++] = (GLfloat)y;
            grid_vert[i++] = (GLfloat)x + x_step;
            grid_vert[i++] = (GLfloat)y + y_step;
        }
    }
    glGenVertexArrays(1, &grid_vao);
    glBindVertexArray(grid_vao);

    glGenBuffers(1, &grid_vert_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, grid_vert_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * grid_size * 2 * 6,
        grid_vert, GL_STATIC_DRAW);
    GLint att_pos_loc = glGetAttribLocation(gc_shader, "att_pos");
    glVertexAttribPointer(att_pos_loc, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(att_pos_loc);

    glGenBuffers(1, &grid_texc_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, grid_texc_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * grid_size * 2 * 6,
        NULL, GL_DYNAMIC_DRAW);
    GLint att_texc_loc = glGetAttribLocation(gc_shader, "att_texc");
    glVertexAttribPointer(att_texc_loc, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(att_texc_loc);

    glBindVertexArray(0);
}
void grid_delete (void) {
    free(grid_vert);
    free(grid_texc);
    free(grid_data);
    glDeleteBuffers(1, &grid_vert_vbo);
    glDeleteBuffers(1, &grid_texc_vbo);
    glDeleteVertexArrays(1, &grid_vao);
}
