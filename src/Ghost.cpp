#include <iostream>

#include "Constants.h"

#include "Game.h"
#include "Ghost.h"
#include "SetConsoleAttributes.h"

using namespace std;

Ghost::Ghost(Game *const g)
: game(g) {}

// движение привидения основанное на текущем режиме каждый раз когда таймер = 0
void Ghost::Move(int playerY, int playerX) {
    if (wait) {
        --wait;
    }
    else {
        switch (mode) {
        // if 'waiting'
        // "вверх вниз"
        case 'w':
            Hide();
            if (y == GATE_Y + 2) {
                ++y;
            }
            else {
                --y;
            }
            Show();
            wait = GHOST_MAX;
            break;
        // if 'exiting'
        // путь для выхода из ворот
        // далее 'chase'
        case 'e':
            Hide();
            wait = GHOST_MAX;
            if (y > GATE_Y + 1) {
                --y;
            }
            else if (x < GATE_X) {
                ++x;
            }
            else if (x > GATE_X) {
                --x;
            }
            else if (y != GATE_Y - 1) {
                --y;
                SetCursorPosition(GATE_Y, GATE_X + 1);
                cout << game->GetLevel(GATE_Y, GATE_X + 1);
            }
            else {
                modeOld = mode;
                if (color == BLUE) {
                    mode = 'r';
                }
                else {
                    mode = 'c';
                }
                dirOld = 'w';
                wait = 0;
            }
            Show();
            break;

        // if 'scattering'
        // движение в случайное направление
        case 's':
            GetOpposite();
            if (modeOld == 'e') {
                modeOld = mode;
            }
            if (mode != modeOld) {
                dir = dirOpp;
                ChangeCoords();
                modeOld = mode;
            }
            else {
                RandomDirection();
            }
            dirOld = dir;
            wait = GHOST_MAX;
            break;
        // if 'chasing'
        // охота за игроком
        case 'c':
            GetOpposite();
            if (modeOld == 'e') {
                modeOld = mode;
            }
            if (mode != modeOld) {
                dir = dirOpp;
                ChangeCoords();
                modeOld = mode;
            }
            else {
                bool down = y < playerY;
                bool up = y > playerY;
                bool right = x < playerX;
                bool left = x > playerX;
                bool favorableDirs[4] = { up, left, down, right };
                TargetObject(favorableDirs);
            }
            dirOld = dir;
            wait = GHOST_MAX;
            break;

        }
    }
}

void Ghost::TargetObject(bool favorableDirs[4]) {
    int good = 0;
    char goodDirs[4] = {' ',' ',' ',' '};
    for (int i = 0; i < 4; ++i) {
        dir = ALL_DIRS[i];
        if (favorableDirs[i] && TestForCollision() == false && dir != dirOpp) {
            goodDirs[good] = dir;
            ++good;
        }
    }
    if (good == 0) {
        RandomDirection();
    }
    else {
        dir = goodDirs[rand() % good];
        ChangeCoords();
    }
}

void Ghost::RandomDirection() {
    GetOpposite();
    // выбор случайного направления без столновений
    do {
        // выбор случайного направления которое не противоположно предыдущему
        do {
            dir = ALL_DIRS[rand() % 4];
        } while (dir == dirOpp);
    } while (TestForCollision() == true);
    ChangeCoords();
}

bool Ghost::TestForCollision() {
    // если значение перед приведением пусто, движение в подходящем направлении
    switch(dir) {
    case 'a':
        // если через тоннель
        if (x == 0 || strchr(NO_COLLISION_TILES, game->GetLevel(y, x - 1))) {
            return false;
        }
        break;
    case 'd':
        // если через тоннель
        if (x == LEVEL_WIDTH - 1 || strchr(NO_COLLISION_TILES, game->GetLevel(y, x + 1))) {
            return false;
        }
        break;
    case 'w':
        if (strchr(NO_COLLISION_TILES, game->GetLevel(y - 1, x))) {
            return false;
        }
        break;
    case 's':
        if (strchr(NO_COLLISION_TILES, game->GetLevel(y + 1, x))) {
            return false;
        }
        break;
    }
    return true;
}

void Ghost::ChangeCoords() {
    Hide();
    if (dir == 'a') {
        if (x == 0) {
            x = LEVEL_WIDTH - 1;
        }
        else {
            --x;
        }
    }
    if (dir == 'd') {
        if (x == LEVEL_WIDTH - 1) {
            x = 0;
        }
        else {
            ++x;
        }
    }
    if (dir == 'w') {
        --y;
    }
    if (dir == 's') {
        ++y;
    }
    Show();
}

void Ghost::GetOpposite() {
    if (dirOld == 'a') {
        dirOpp = 'd';
    }
    if (dirOld == 'd') {
        dirOpp = 'a';
    }
    if (dirOld == 'w') {
        dirOpp = 's';
    }
    if (dirOld == 's') {
        dirOpp = 'w';
    }
}


void Ghost::Show() {
    SetTextColor(color);
    SetCursorPosition(y, x);
    cout << icon;
}

void Ghost::Hide() {
    SetTextColor(WHITE);
    SetCursorPosition(y, x);
    cout << game->GetLevel(y, x);
}
