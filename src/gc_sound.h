#ifndef GC_SOUND_H
#define GC_SOUND_H

extern struct sfx{
    void* s;
} sound;

void init_sound(void);
void quit_sound(void);
void play_sound(void);

#endif
