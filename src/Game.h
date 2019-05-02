#ifndef GAME_H
#define GAME_H

#include "Constants.h"

class Pacman;
class Ghost;

class Game {
private:
    char level[LEVEL_HEIGHT][LEVEL_WIDTH];
    int oneUpTimer = ONE_UP_MAX;
    int oneUpColor = WHITE;
    int ghostModeTimer = MODE_MAX;

    Pacman* player;
    Ghost* ghosts[2];

    void MainLoop();

public:
    Game();
    ~Game();
    void Go();
	int state = 0;
	int difficulty = 14;
	int DiffFirst;
	int xc=9;
	int levelNum;

    void LoadLevel();
	void NextLevel();
    void PrintReady();
	void PrintLevel();
    void GameOver();
	void PrintYouWon();
	void ClearAll();
	void GetTimer(int xco, int yco);
	void WaitPress();
	void ClearYk(int xco, int yco);
	void Menu();
	void GameOverMenu();
		
    void MoveGhosts();
    void UpdateTimers();
    void CheckForDeath();
    void ShowAll();
    void HideAll();
    void InitAll();

    char GetLevel(int y, int x) { return level[y][x];    }
    int GetOneUpTimer()         { return oneUpTimer;     }
    int GetOneUpColor()         { return oneUpColor;     }
    int GetGhostModeTimer()     { return ghostModeTimer; }

    void SetLevel(int y, int x, char c) { level[y][x] = c;    }
    void SetOneUpTimer(int t)           { oneUpTimer = t;     }
    void SetOneUpColor(int c)           { oneUpColor = c;     }
    void SetGhostModeTimer(int t)       { ghostModeTimer = t; }
};

#endif
