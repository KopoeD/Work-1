#include <iostream>
#include <Windows.h>

#include "Constants.h"
#include "conio.h"
#include "Game.h"
#include "Pacman.h"
#include "Ghost.h"
#include "SetConsoleAttributes.h"

using namespace std;

Game::Game() {
    SetWindowTitle("PACMAN");
    SetWindowSize(LEVEL_HEIGHT + 4, LEVEL_WIDTH);
    SetCursorVisibility(false);
    player = new Pacman(this);
    for (int i = 0; i < 2; ++i) {
        ghosts[i] = new Ghost(this);
    }
}

Game::~Game() {
    delete player;
    for (int i = 0; i < 2; ++i) {
        delete ghosts[i];
    }
}

void Game::Menu() {
	xc = 9;
	Sleep(500);
	SetTextColor(RED);
	SetCursorPosition(6, 11);
	cout << "PACMAN";
	SetTextColor(WHITE);
	SetCursorPosition(9, 8);
	cout << "1.Start game";
	SetCursorPosition(11, 8);
	cout << "2.Difficulty";
	SetCursorPosition(13, 8);
	cout << "3.Rules";
	Go();
}

void Game::Go() {
	while (true)
	{
		GetTimer(xc,7);
		WaitPress();
	}
}

void Game::MainLoop() {
    player->SetScore(0);
    player->SetLives(3);
    bool gameOver = false;
	for (int levelNum = 1; levelNum <= 3; ++levelNum) {
		LoadLevel();
		// пока есть точки
		while (player->GetLeft() != 0) {
			player->Move();
			CheckForDeath();
			if (!player->GetLives()) {
				gameOver = true;
				break;
			}
			MoveGhosts();
			CheckForDeath();
			if (!player->GetLives()) {
				gameOver = true;
				break;
			}
			UpdateTimers();
		}
		if (gameOver != true && levelNum == 3) {
			PrintYouWon();
			GameOverMenu();
		}
		NextLevel();
		difficulty = difficulty - 2;
	}
}

void Game::LoadLevel() {
    char levelMap[LEVEL_HEIGHT][LEVEL_WIDTH + 1] = {
        "1555555555555555555555555552",
        "6............^^............6",
        "6.!%%@.!%%%@.^^.!%%%@.!%%@.6",
        "6.^  ^.^   ^.^^.^   ^.^  ^.6",
        "6.#%%$.#%%%$.#$.#%%%$.#%%$.6",
        "6..........................6",
        "6.!%%@.!@.!%%%%%%@.!@.!%%@.6",
        "6.#%%$.^^.#%%@!%%$.^^.#%%$.6",
        "6......^^....^^....^^......6",
        "355552.^#%%@ ^^ !%%$^.155554",
        "     6.^!%%$ #$ #%%@^.6     ",
        "     6.^^    B     ^^.6     ",
        "     6.^^ 155&&552 ^^.6     ",
        "555554.#$ 6      6 #$.355555",
        "      .   6      6   .      ",
        "555552.!@ 6  P   6 !@.155555",
        "     6.^^ 35555554 ^^.6     ",
        "     6.^^          ^^.6     ",
        "     6.^^ !%%%%%%@ ^^.6     ",
        "155554.#$ #%%@!%%$ #$.355552",
        "6............^^............6",
        "6.!%%@.!%%%@.^^.!%%%@.!%%@.6",
        "6.#%@^.#%%%$.#$.#%%%$.^!%$.6",
        "6...^^.......X .......^^...6",
        "6%@.^^.!@.!%%%%%%@.!@.^^.!%6",
        "6%$.#$.^^.#%%@!%%$.^^.#$.#%6",
        "6......^^....^^....^^......6",
        "6.!%%%%$#%%@.^^.!%%$#%%%%@.6",
        "6.#%%%%%%%%$.#$.#%%%%%%%%$.6",
        "6..........................6",
        "3555555555555555555555555554"};
    char curChar;
    SetTextColor(WHITE);
    SetCursorPosition(-3, 3);
    cout << "SCORE";
    SetCursorPosition(-3, 15);
    cout << "HIGH SCORE";
    player->PrintScore(0);
    SetCursorPosition(0, 0);
    player->SetLeft(0);
    for (int y = 0; y < LEVEL_HEIGHT; ++y) {
        for (int x = 0; x < LEVEL_WIDTH; ++x) {
            curChar = levelMap[y][x];
            SetTextColor(DARK_BLUE);
            switch (curChar) {
            case 'X':
                player->SetYInit(y);
                player->SetXInit(x);
                level[y][x] = ' ';
                break;
            case 'B':
                ghosts[BLINKY]->SetYInit(y);
                ghosts[BLINKY]->SetXInit(x);
                ghosts[BLINKY]->SetColorInit(RED);
                ghosts[BLINKY]->SetDirOpp('s');
                level[y][x] = ' ';
                break;
            case 'P':
                ghosts[PINKY]->SetYInit(y);
                ghosts[PINKY]->SetXInit(x);
                ghosts[PINKY]->SetColorInit(MAGENTA);
                level[y][x] = ' ';
                break;
            case '.':
                SetTextColor(WHITE);
                level[y][x] = char(250);
                player->SetLeft(player->GetLeft() + 1);
                break;
            case ' ':
                level[y][x] = curChar;
                break;
            case '&':
                SetTextColor(WHITE);
                curChar = '%';
            }
            if (curChar == '1') {
                level[y][x] = char(201);
            }
            else if (curChar == '2') {
                level[y][x] = char(187);
            }
            else if (curChar == '3') {
                level[y][x] = char(200);
            }
            else if (curChar == '4') {
                level[y][x] = char(188);
            }
            else if (curChar == '5') {
                level[y][x] = char(205);
            }
            else if (curChar == '6') {
                level[y][x] = char(186);
            }
            else if (curChar == '!') {
                level[y][x] = char(218);
            }
            else if (curChar == '@') {
                level[y][x] = char(191);
            }
            else if (curChar == '#') {
                level[y][x] = char(192);
            }
            else if (curChar == '$') {
                level[y][x] = char(217);
            }
            else if (curChar == '%') {
                level[y][x] = char(196);
            }
            else if (curChar == '^') {
                level[y][x] = char(179);
            }
            cout << level[y][x];
        }
        SetCursorPosition(y + 1, 0);
    }
    InitAll();
    ShowAll();
    player->PrintLives();
    PrintReady();
}

void Game::NextLevel() {
	Sleep(1000);
	HideAll();
	SetCursorPosition(12, 13);
	cout << "  ";
	for (int i = 0; i < 4; ++i) {
		SetScreenColor(WHITE);
		player->Show();
		Sleep(200);
		SetScreenColor(DARK_BLUE);
		player->Show();
		Sleep(200);
	}
	SetCursorPosition(0, 0);
	InitAll();
}

void Game::GameOverMenu() {
	ClearAll();
	Sleep(400);
	SetTextColor(RED);
	SetCursorPosition(6, 11);
	cout << "PACMAN";
	SetTextColor(WHITE);
	SetCursorPosition(9, 8);
	cout << "1.Retry";
	SetCursorPosition(11, 8);
	cout << "2.Difficulty";
	SetCursorPosition(13, 8);
	cout << "3.Menu";
	state = 3;
	xc = 9;
	WaitPress();

}

void Game::PrintReady() {
    SetTextColor(YELLOW);
    SetCursorPosition(17, 11);
    cout << "READY!";
    Sleep(2000);
    SetCursorPosition(17, 11);
    cout << "      ";
}

void Game::GameOver() {
    SetCursorPosition(17, 9);
    SetTextColor(RED);
    cout << "GAME  OVER";
	Sleep(1500);
	ClearAll();
	GameOverMenu();
}

void Game::PrintYouWon() {
	SetCursorPosition(17, 9);
	SetTextColor(RED);
	cout << " YOU  WON";
	Sleep(1000);
}

void Game::MoveGhosts() {
    // проверка на смену режима
  
    if (player->GetLeft() == 235 && ghosts[PINKY]->GetMode() == 'w') {
        ghosts[PINKY]->SetMode('e');
    }
    if (player->GetLeft() == 200 && ghosts[BLINKY]->GetMode() == 'w') {
        ghosts[BLINKY]->SetMode('e');
    }
    for (int i = 0; i < 2; ++i) {
        ghosts[i]->Move(player->GetY(), player->GetX());
    }
    ShowAll();
}

void Game::WaitPress() {
	while (true) {
		int xc_temp = xc;
		if (GetAsyncKeyState(VK_UP)) {
			ClearYk(xc,7);
			if (xc == 9) {
				xc = 13;
			}
			else {
				xc = xc - 2;
			}
		}
		if (GetAsyncKeyState(VK_DOWN)) {
			ClearYk(xc,7);
			if (xc == 13) { xc = 9;}
			else {
				xc = xc + 2;
			}
		}
		if (xc_temp == xc) {
			GetTimer(xc,7);
		}
		else {
			for (int i = 0; i < 15; i++) {
				GetTimer(xc,7);
			}
		}
		if (GetAsyncKeyState(VK_RETURN)) {
			if (xc == 9 && state != 2) {
				while (true) {
					MainLoop(); break;
				}
			}
			if (xc == 11 && state == 0) {
				state = 2;
				ClearAll();
				Sleep(400);
				SetTextColor(RED);
				SetCursorPosition(6, 11);
				cout << "PACMAN";
				SetTextColor(WHITE);
				SetCursorPosition(9, 8);
				cout << "1.Easy";
				SetCursorPosition(11, 8);
				cout << "2.Medium";
				SetCursorPosition(13, 8);
				cout << "3.Hard";
				xc = 9;
				GetTimer(xc, 7);
				break;
				
			
			}
			if (xc == 13 && state == 0) {
				ClearAll();
				Sleep(400); 
				SetTextColor(RED);
				SetCursorPosition(6, 11);
				cout << "PACMAN";
				SetTextColor(WHITE);
				SetCursorPosition(9, 0);
				cout << "1.Arrow keys to move Pacman";
				SetCursorPosition(11, 0);
				cout << "2.Do not get caught by the  Monsters";
				SetCursorPosition(14, 0);
				cout << "3.Every 3500 point you will have extra life";
				SetCursorPosition(17, 12);
				cout << "Back";
				while (true) {
					GetTimer(17, 11);
					if (GetAsyncKeyState(VK_RETURN)) { 
						ClearAll();
						Menu(); 
					}
				}

				
			}

			if (xc == 9 && state==2) {
				difficulty = 14;
				state = 0;
				ClearAll();
				Menu();
			}

			if (xc == 11 && state==2) {
				difficulty = 13;
				state = 0;
				ClearAll();
				Menu();

			}

			if (xc == 13 && state == 2) {
				difficulty = 12;
				state = 0;
				ClearAll();
				Menu();
			}
			
			if (xc == 11 && state == 3) {
				state = 2;
				ClearAll();
				Sleep(400);
				SetTextColor(RED);
				SetCursorPosition(6, 11);
				cout << "PACMAN";
				SetTextColor(WHITE);
				SetCursorPosition(9, 8);
				cout << "1.Easy";
				SetCursorPosition(11, 8);
				cout << "2.Medium";
				SetCursorPosition(13, 8);
				cout << "3.Hard";
				xc = 9;
				GetTimer(xc, 7);
			}

			if (xc == 13 && state == 3) {
				state = 0;
				ClearAll();
				Menu();
			}
		}
	}	
}

void Game::GetTimer(int xco, int yco) {
	if (oneUpTimer) {
		--oneUpTimer;
	}
	else {
		if (oneUpColor == WHITE) {
			oneUpColor = BLACK;
		}
		else {
			oneUpColor = WHITE;
		}
		SetTextColor(oneUpColor);
		SetCursorPosition(xco,yco);
		cout << ">";
		oneUpTimer = ONE_UP_MAX;
	}
	Sleep(15);
}

void Game::ClearYk(int xco,int yco) {
	SetCursorPosition(xco, yco);
	cout << ' ';
}

void Game::UpdateTimers() {
	
	if (oneUpTimer) {
		--oneUpTimer;
	}
	else {
		if (oneUpColor == WHITE) {
			oneUpColor = BLACK;
		}
		else {
			oneUpColor = WHITE;
		}
		SetTextColor(oneUpColor);
		SetCursorPosition(-3, 3);
		cout << "SCORE";
		oneUpTimer = ONE_UP_MAX;
	}
	
	// смена режима привидений
	if (ghostModeTimer) {
		--ghostModeTimer;
		if (ghostModeTimer == MODE_MAX / 4) {
			for (int i = 0; i < 2; ++i) {
				if (ghosts[i]->GetMode() == 'c') {
					ghosts[i]->SetMode('s');
				}
			}
		}
	}
	else {
		for (int i = 0; i < 2; ++i) {
			if (ghosts[i]->GetMode() == 's') {
				ghosts[i]->SetMode('c');
			}
		}
		ghostModeTimer = MODE_MAX;
	}
	Sleep(difficulty);
}

void Game::CheckForDeath() {
    for (int i = 0; i < 2; ++i) {
        if (player->GetX() == ghosts[i]->GetX() && player->GetY() == ghosts[i]->GetY() &&
            ghosts[i]->GetMode() != 'd' && ghosts[i]->GetMode() != 'n') {
            if (ghosts[i]->GetMode() != 'r') {
                player->Dead();
            }
        }
    }
}

void Game::ShowAll() {
    player->Show();
    for (int i = 0; i < 2; ++i) {
        ghosts[i]->Show();
    }
}

void Game::HideAll() {
    player->Hide();
    for (int i = 0; i < 2; ++i) {
        ghosts[i]->Hide();
    }
}

void Game::ClearAll() {
	system("cls");
}

void Game::InitAll() {
    player->SetY(player->GetYInit());
    player->SetX(player->GetXInit());
    player->SetColor(YELLOW);
    player->SetIcon(ICONS[1]);
    player->SetDirOld('a');
    player->SetWait(0);
    player->SetSuper(0);
    for (int i = 0; i < 2; ++i) {
        ghosts[i]->SetY(ghosts[i]->GetYInit());
        ghosts[i]->SetX(ghosts[i]->GetXInit());
        ghosts[i]->SetColor(ghosts[i]->GetColorInit());
        ghosts[i]->SetMode('w');
        ghosts[i]->SetWait(0);
        ghosts[i]->SetIcon(GHOST_ICON);
    }
    ghosts[BLINKY]->SetMode('c');
    ghosts[BLINKY]->SetModeOld('c');
    if (player->GetLeft() <= 235) {
        ghosts[PINKY]->SetMode('e');
    }
}
