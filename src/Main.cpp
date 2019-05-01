#include "Game.h"

int main() {
    Game* game = new Game();
    game->Menu();
    delete game;
    return 0;
}
