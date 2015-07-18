#ifndef TEXTURE_H
#define TEXTURE_H
#include "minig.h"

typedef struct image_s {
    unsigned char* pixels;
    unsigned int dx, dy;
} image_t;

typedef struct texture_s {
    GLuint id;
    unsigned int x,y;
    GLuint ul;
} texture_t;

texture_t* texture_create (char* fileName);

image_t image_from_PNG(char* fileName);
void image_delete(image_t* img);
texture_t texture_from_image (image_t* img);
void texture_delete (texture_t* tex);

void texture_bind(texture_t* tex);

#endif
