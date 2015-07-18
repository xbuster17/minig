//texture.c
#include "minig.h"

#ifndef ANDROID
static char assetsDir [] = "assets/";
#endif

texture_t* texture_create (char* fileName) {
    image_t img = image_from_PNG(fileName);
    if (!img.pixels) return NULL;
    texture_t* texture = malloc(sizeof(texture_t));
    *texture = texture_from_image(&img);
    image_delete(&img);
    if (!texture) return NULL;
    else return texture;
}
void texture_bind(texture_t* tex) {
    if (tex)
        glBindTexture(GL_TEXTURE_2D, tex->id);
}


image_t image_from_PNG(char* fileName) {
    image_t img = {0};
    uint error;
    SDL_RWops* file = NULL;

    #ifndef ANDROID
    char* newFileName = malloc(strlen(assetsDir)+strlen(fileName)+1);
    strcpy(newFileName, assetsDir);
    strcat(newFileName, fileName);
    file = SDL_RWFromFile(newFileName, "rb");
    free(newFileName);
    #else
    file = SDL_RWFromFile(fileName, "rb");
    #endif

    if (!file) {
        printf("%s\n", SDL_GetError());
        return (image_t){0};
    }
    size_t img_size = file->size(file);
    unsigned char* img_buffer = malloc(img_size);

    if (!file->read(file, img_buffer, img_size, 1)) {
        printf("%s\n", SDL_GetError());
        free(img_buffer);
        return (image_t){0};
    }

    error = lodepng_decode32(&img.pixels, &img.dx, &img.dy,
        (unsigned char*)img_buffer, img_size);

    if (error) {
        printf("error %u: %s\n", error, lodepng_error_text(error));
        free(img.pixels);
        img = (image_t){0};
    }

    file->close(file);
    free(img_buffer);
    return img;
}

void image_delete(image_t* img) {
    free(img->pixels);
    img->pixels = NULL;
}

texture_t texture_from_image (image_t* img) {
    texture_t tex = (texture_t){0};
    tex.x = img->dx;
    tex.y = img->dy;
    glGenTextures(1, &tex.id);
    glBindTexture(GL_TEXTURE_2D, tex.id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(
        GL_TEXTURE_2D, 0,          /* target, level */
        GL_RGBA,                   /* internal format */
        tex.x, tex.y, 0,           /* width, height, border */
        GL_RGBA, GL_UNSIGNED_BYTE, /* external format, type */
        img->pixels                /* pixels */
    );
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    return tex;
}
void texture_delete (texture_t* tex) {
    if (tex) {
        glDeleteTextures(1, &tex->id);
        tex->id = 0;
    }
}
