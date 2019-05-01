#include <iostream>
#include <iomanip>
#include <conio.h>
#include <Windows.h>

#include "Constants.h"

#include "Game.h"
#include "Pacman.h"
#include "SetConsoleAttributes.h"

using namespace std;

Pacman::Pacman(Game *const g)
: game(g) {
    hiScore = 0;
}


void Pacman::Move() {
    if (wait) {
        --wait;
    }
    else {
        GetDirection();
        if (TestForCollision() == false) {
            SetCursorPosition(yOld, xOld);
            cout << game->GetLevel(yOld, xOld);
            if (game->GetLevel(y, x) != ' ') {
                int scoreInc;
                scoreInc = 10;
                PrintScore(scoreInc);
                game->SetLevel(y, x, ' ');
                --left;
            }
            Show();
            dirOld = dir;
            wait = PACMAN_MAX;
        }
    }
}

void Pacman::GetDirection() {
	dir = 'x';
	if (GetAsyncKeyState(VK_UP)) {
		dir = 'w';
	}
	if (GetAsyncKeyState(VK_DOWN)) {
		dir = 's';
	}
	if (GetAsyncKeyState(VK_LEFT)) {
		dir = 'a';
	}
	if (GetAsyncKeyState(VK_RIGHT)) {
		dir = 'd';
	}
    // если не получилось, пытаемся идти в прошлом направлении
    if (!strchr(ALL_DIRS, dir)) {
        dir = dirOld;
    }
}

bool Pacman::TestForCollision() {
    // сохранить старые координаты
    xOld = x;
    yOld = y;
	// если значение перед игроком пусто, движение в подходящем направлении
    switch (dir) {
    case 'a':
        // через тоннель
        if (x == 0) {
            x = LEVEL_WIDTH - 1;
            icon = ICONS[1];
        }
        else if (strchr(NO_COLLISION_TILES, game->GetLevel(y, x - 1))) {
            --x;
            icon = ICONS[1];
        }
        break;
    case 'd':
        // через тоннель
        if (x == LEVEL_WIDTH - 1) {
            x = 0;
            icon = ICONS[3];
        }
        else if (strchr(NO_COLLISION_TILES, game->GetLevel(y, x + 1))) {
            ++x;
            icon = ICONS[3];
        }
        break;
    case 'w':
        if (strchr(NO_COLLISION_TILES, game->GetLevel(y - 1, x))) {
            --y;
            icon = ICONS[0];
        }
        break;
    case 's':
        if (strchr(NO_COLLISION_TILES, game->GetLevel(y + 1, x))) {
            ++y;
            icon = ICONS[2];
        }
    }
    // если координаты не поменялись - столкновение
    if (x == xOld && y == yOld) {
        return true;
    }
    return false;
}

void Pacman::PrintScore(int scoreInc) {
	if (score / 3500 < (score + scoreInc) / 3500) {
		++lives;
		PrintLives();
	}
    score += scoreInc;
    SetTextColor(WHITE);
    SetCursorPosition(-2, 0);
    if (score == 0) {
        cout << setw(7) << "00";
    }
    else {
        cout << setw(7) << score;
    }
    if (score > hiScore) {
        hiScore = score;
        cout << setw(11) << hiScore;
    }
	else {
		cout << setw(11) << hiScore;
	}
}

void Pacman::PrintLives() {
    SetTextColor(color);
    SetCursorPosition(LEVEL_HEIGHT, 2);
    for (int i = 1; i < lives; ++i) {
        cout << ICONS[1] << " ";
    }
    cout << " ";
}


void Pacman::Dead() {
    Sleep(1000);
    game->HideAll();
    for (int i = 0; i < 8; ++i) {
        icon = ICONS[i % 4];
        Show();
        Sleep(100);
    }
    Hide();
    Sleep(500);
    --lives;
    if (lives != 0) {
        game->InitAll();
        game->ShowAll();
        PrintLives();
        game->PrintReady();
    }
    else {
        game->GameOver();
    }
}

void Pacman::Show() {
    SetTextColor(color);
    SetCursorPosition(y, x);
    cout << icon;
}

void Pacman::Hide() {
    SetCursorPosition(y, x);
    cout << game->GetLevel(y, x);
}
