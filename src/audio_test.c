#include "audio_test.h"
//The music that will be played
Mix_Music *gmu = NULL;

//The sound effects that will be used
Mix_Chunk *gse = NULL;
Mix_Chunk *gHigh = NULL;
Mix_Chunk *gMedium = NULL;
Mix_Chunk *gLow = NULL;

void test_sound(void){
    Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 );
    gmu = Mix_LoadMUS( "808-bassdrum.wav" );
    gse = Mix_LoadWAV( "808-hihat.wav" );


/*TODO!!!*/
uint i = 0;
poll_events();
while (true){
    poll_events();

        Mix_PlayChannel( -1, gse, 0 );

        if( Mix_PlayingMusic() == 0 ){
            //Play the music
            Mix_PlayMusic( gmu, -1 );
        }
        //If music is being played
        else
            {
            //If the music is paused
            if( Mix_PausedMusic() == 1 )
                {
                    //Resume the music
                    Mix_ResumeMusic();
                }
            //If the music is playing
            else
            {
                //Pause the music
                Mix_PauseMusic();
            }
        }

        //Stop the music
        Mix_HaltMusic();

    swap_buffers();

    if (kbk('q') || kbk(SDLK_AC_BACK) || kbk(SDLK_ESCAPE)) return;
    if (i++ > 50) return;

    }

        Mix_FreeMusic( gmu );
        Mix_FreeChunk( gse );
}
