#ifndef _GAME_LUX
#define _GAME_LUX
#include "ConsoleGraphics.h"
#include "timer.h"
#define LUXWIDTH 20
#define LUXHEIGTH 20
#define LUX_PAIR_DEFAULT 20
#define LUX_PAIR_PLAYER_DEFAULT 21
#define LUX_PAIR_READYDAMAGE 22
#define LUX_PAIR_DAMAGE 23
#define KEY_DELAY 50 // milisecond
#define LUX_COL_DEFAULT MIXSH(COLOR_GREEN,COLOR_BLACK)
#define LUX_COL_PLAYER_DEFAULT MIXSH(COLOR_WHITE,COLOR_BLACK)
#define LUX_COL_READYDAMAGE MIXSH(COLOR_YELLOW,COLOR_BLACK)
#define LUX_COL_DAMAGE MIXSH(COLOR_RED,COLOR_BLACK)
void RefreshScore();
int LuxGame();
void LuxFreshYX(int y,int x);
void LuxFreshLine(int bW,int idx);
void LuxMove(int y,int x,int arr);
int SetLazerDamage(int bW,int index);
int DelLazer(int bW,int index);
int SetLazerReady(int k);
#endif
