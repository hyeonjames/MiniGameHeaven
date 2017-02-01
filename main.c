#include "game.h"

/*
	main function
	
*/
void Init();
int main(void)
{	
	int c,d;
	char name[] = "메뉴를 선택하세요.";
	cbox* BTest;	
	setlocale(LC_ALL,"ko_KR.utf8");
	setlocale(LC_CTYPE,"ko_KR.utf8");
	Init();
	while(c != 2)
	{
		clear();
		c=Menu(6,3,3,
			MIXSH(COLOR_BLACK,COLOR_WHITE),
			MIXSH(COLOR_WHITE,COLOR_BLACK),
			MIXSH(COLOR_RED,COLOR_BLACK),
			name,"Game Start","Game Option","Exit");
		switch(c)
		{
			case 0:
				clear();
				GameStart();
				break;
			case 1:
				clear();
				BTest = MakeBox(11,3,BTYPE_UPDOWN,1,"Test : ");
				SetBoxUpdown(BTest,0,10,5);
				d=BoxMenu(0,0,"TESTTEST",
					MIXSH(COLOR_BLACK,COLOR_WHITE),
					MIXSH(COLOR_WHITE,COLOR_BLACK),
					MIXSH(COLOR_RED,COLOR_BLACK),
					1,2,
					BTest,
					MakeBox(11,3,BTYPE_DEFAULT,1,"Back"));
				if(d == 1) break;
				break;
		}
	}
	endwin();
}

void Init()
{
	initscr();
	//raw();
	
	has_colors();
	start_color();
	keypad(stdscr,TRUE);
	nodelay(stdscr, 1);
	init_pair(PAIR_DEFAULT,FRONTSH(color_default),BOTTOMSH(color_default));	
	srand(time(NULL));
	clear();
}
