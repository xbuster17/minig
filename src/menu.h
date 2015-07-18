#ifndef MENU_H
#define MENU_H
#include "minig.h"

typedef struct menu_s {
    bool shown;
    int items;
} menu_t;

void menu_init(void);

#endif
