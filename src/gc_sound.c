#include "gc_sound.h"
#include "minig.h"

struct sfx sound;
void* gmu;
#ifndef ANDROID
#define ASSET_PATH "assets/"
#else
#define ASSET_PATH ""
#endif

void init_sound(void) {
    // Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 );
// Mix_Init(
// MIX_INIT_FLAC ||
// MIX_INIT_MOD ||
// MIX_INIT_MP3 ||
// MIX_INIT_OGG
// );
    // Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 4096 );
    // Mix_OpenAudio( 8000, MIX_DEFAULT_FORMAT, 2, 4096 );
    Mix_OpenAudio( 48000, MIX_DEFAULT_FORMAT, 2, 2048 );
#define num_chan 128
    Mix_AllocateChannels(num_chan);


char track = (rand() * time(0)) % 2;
    switch(track){
        case 0: gmu = Mix_LoadMUS(ASSET_PATH"external.xm");
            break;
        case 1: gmu = Mix_LoadMUS(ASSET_PATH"aryx.s3m");
            break;
        default: gmu = Mix_LoadMUS(ASSET_PATH"aryx.s3m"); break;
    }
    if(gmu == NULL) printf("%s\n", "\tcouldn't load .mod");
    Mix_PlayMusic( gmu, -1 );

    uint i = 0;
    for(; i < num_chan; i++)
        Mix_Volume(i,MIX_MAX_VOLUME / 8);
    // Mix_Volume(32,MIX_MAX_VOLUME);
    sound.s = Mix_LoadWAV( ASSET_PATH"808-hihat.wav" );

    // print sample chunk decoders available

    int im,max=Mix_GetNumChunkDecoders();
    for(im=0; im<max; ++im)
        printf("\tSample chunk decoder %d is for %s\n",im,Mix_GetChunkDecoder(im));

    int max_mus=Mix_GetNumMusicDecoders();
    for(im=0; im<max_mus; ++im)
        printf("\tMusic decoder %d is for %s\n",im,Mix_GetMusicDecoder(im));
}

#undef num_chan


void quit_sound(void) {
    Mix_FreeChunk( sound.s );
    Mix_FreeMusic( gmu );
    Mix_Quit();
}

void play_sound(void){
    Mix_PlayChannel( -1, sound.s, 0 );

}
