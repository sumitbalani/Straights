#include <iostream>
#include "Command.h"
#include "Game.h"
#include <cstdlib>


using namespace std;

int main(int argc, char *argv[]){
    //randomize seed
    if (argc == 1) {
        srand48(0);
    }
    else {
	int num= atoi(argv[1]);
        srand48( num );
    }

    //init and start game
    Game game;
    game.startGame();
    
    return 0;
}
