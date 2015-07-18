#include "minig.h"


int main(int argc, char** argv) {

    printf("%i:%s:\n", argc, argv[0]);

    minig_init();

    run_game__grid_critters();



    minig_quit();
#if defined(ANDROID)
    exit(0);
#endif
    return 0;

}
