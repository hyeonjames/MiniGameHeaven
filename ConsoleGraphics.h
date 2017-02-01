#ifndef GRAPHICS_H
#define GRAPHICS_H

#include<stdio.h>
#include<stdarg.h>
#include<ncurses.h>
#include<stdlib.h>
#include<locale.h>
#include<string.h>
#include <unistd.h>
#define Color(k) attron(COLOR_PAIR(k))
#define MENU_DEFAULT 1
#define MENU_UNSET 2
#define MENU_SET 3
#define PAIR_DEFAULT 4
#define PAIR_COLBOX_BOX 5
#define PAIR_COLBOX_TEXT 6
#define PAIR_COLBOX_SET 7
#define PAIR_COLBOX_UNSET 8
#define PAIR_COLBOX_DEFAULT 9
#define MIXSH(a,b) (((a&0x00FF) << 8) | (b&0x00FF))
#define FRONTSH(a) ((a&0xFF00) >> 8) // get front bits ( font)
#define BOTTOMSH(a) (a&0x00FF) // get bottom bits ( background)
#define ID_YES 1
#define ID_NO 2
#define COLSET(a,b) init_pair(a,FRONTSH(b),BOTTOMSH(b))
#define RAND(a,b) (rand()%(b+1-a)+a)
#define BTYPE_UPDOWN 1
#define BTYPE_LEFTRIGHT 2
#define BTYPE_DEFAULT 0
#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3
#define color_default MIXSH(COLOR_WHITE,COLOR_BLACK)
typedef struct __BOX
{
	int posx,posy,h,w,cnt,type;
	char** text;
	int index;
	void* Data;
}cbox;
int InverseArrow(int);
int Menu(int,int,int,short,short,short,char*,...);
void SetBoxUpdown(cbox* box,int min,int max,int def);
int GetBoxIndex(cbox* box);
void Updown(cbox *box);

int YesNoMenu(int,int,short,short,char*);
void SetColor(int,short,short);
void MkSquare(int,int,int,int);
void Erase(int,int,int);
void EraseSq(int,int,int,int);
cbox* MakeBox(int,int,int,int,...);

void PrintBox(cbox*,int,int,short,short);
void _PrintBox(cbox*,int,int);
int BoxMenu(int y,int x,char* text,short set,short unset,short def,int lineMax,int cnt,...);
#endif
