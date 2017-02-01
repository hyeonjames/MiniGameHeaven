#ifndef __SNAKE_H_
#define __SNAKE_H_
#include "ConsoleGraphics.h"
#include "timer.h"
#include "item.h"
#define SNAKEWIDTH	20
#define SNAKEHEIGTH 	20
int SnakeGame();

typedef struct SNAKE{
	int x,y;
	int direction;
}snake;




void RefreshScore2();
int isPassable(int y,int x);
int GoSnake();
void add(int,int,int);
#endif
