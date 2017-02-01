#ifndef GAME_H
#define GAME_H
#include "usr.h"
#include "luxlazer.h"
#include "snake.h"
#include "sokoban.h"
typedef struct __GINFO
{
	user** users;
	int usidx;
	user* me;
}gameinfo;

void GameStart();
void MemoryFree();
void GameMenu();
int LoadUser(gameinfo*,int* cnt);
int find(gameinfo*,char* id);
int SaveUser(gameinfo* ginfo);
#endif
