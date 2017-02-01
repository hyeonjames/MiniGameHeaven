#include<stdio.h>

int main(void)
{
	system("gcc -o game game.c main.c ConsoleGraphics.c timer.c snake.c luxlazer.c -lncursesw -lpthread");
	return 0;
}
