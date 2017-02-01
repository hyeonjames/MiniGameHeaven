#include "game.h"

void GameStart()
{
	gameinfo* ginfo;
	int c=0,sel,cnt,i;
	char id[ID_MAX+1];
	ginfo = (gameinfo*)malloc(sizeof(gameinfo));
	clear();
	c=LoadUser(ginfo,&cnt);
	while(1)
	{
		clear();
		curs_set(2);
		Color(PAIR_DEFAULT);
		MkSquare(0,2,37,3);
		move(1,4);
		printw("ID:");
		nodelay(stdscr,0);
		getstr(id);
		nodelay(stdscr,1);
		c = find(ginfo,id);
		if(c<0)
		{
			//Can't find id
			sel = YesNoMenu(3,0,MIXSH(COLOR_BLACK,COLOR_WHITE),MIXSH(COLOR_WHITE,COLOR_BLACK),"아이디가 존재하지 않습니다. \n새로 만들겠습니까?");
			if(sel == ID_YES)
			{
				ginfo->me = User(id);
				ginfo->usidx = -1;
				//Game Start	
				GameMenu(ginfo);
				break;
			}
		}
		else
		{
			ginfo->me = ginfo->users[c];
			ginfo->usidx = c;
			//Game Start
			GameMenu(ginfo);
			break;
		}
	}
	free(ginfo);
}
void GameMenu(gameinfo* ginfo)
{
	char text[51+ID_MAX];
	int c,yn,cy,cx,k,i,score;
	sprintf(text," %s   coin : %d",ginfo->me->id,ginfo->me->coin);
	while(1)
	{
		clear();
		c = BoxMenu(0,0,text,
				MIXSH(COLOR_YELLOW,COLOR_BLACK),
				MIXSH(COLOR_WHITE,COLOR_BLACK),
				MIXSH(COLOR_RED,COLOR_BLACK),
				4,GAME_COUNT+1,
				MakeBox(15,4,BTYPE_DEFAULT,2,"럭스의","레이저피하기"),
				MakeBox(15,4,BTYPE_DEFAULT,2,"카시오페아의","스네이크"),
				MakeBox(15,4,BTYPE_DEFAULT,2,"알리스타의","박스밀기"),
				MakeBox(15,4,BTYPE_DEFAULT,2,"티모의","버섯미로"),
				MakeBox(15,4,BTYPE_DEFAULT,1,"종료")
				);
		
		getyx(stdscr,cy,cx);
		cy += 5;	

		switch(c)
		{
			case 0:
				score=LuxGame();
				if(score > ginfo->me->scores[0]) ginfo->me->scores[0] = score;
				getch();
			break;
			case 1:
				score = SnakeGame();
			break;
			case 2:
			break;
			case 3:
				k=SaveUser(ginfo);
				
			break;
			case 4:
				//Ask
				yn = YesNoMenu(cy,0,MIXSH(COLOR_BLACK,COLOR_WHITE),MIXSH(COLOR_WHITE,COLOR_BLACK),"저장 하고 끝냄?");
				if(yn == ID_YES)
				{
					k=SaveUser(ginfo);
					
				}
				return ;
			break;
		}
	}
}

int find(gameinfo* ginfo,char* id)
{
	int i=0;
	int cnt ;
	if(ginfo->users == NULL) return -1;
	cnt = sizeof(ginfo->users)/sizeof(user*);
	for(i=0;i<cnt;i++)
	{
		if(strcmp(ginfo->users[i]->id,id) == 0)	return i;
	}
	return -1;
}

user* User(char* id)
{
	user* us = (user*)malloc(sizeof(user));
	strcpy(us->id,id);
	us->coin = START_COIN;
}

int LoadUser(gameinfo* ginfo,int* cnt)
{
	FILE* fw;
	int size,i;
	user* us;
	move(0,0);
	fw = fopen("./game.dat","rb");
	if(fw == 0) return ERR_NOUSRFILE;	
	fseek(fw,0,SEEK_END);
	size = ftell(fw);
	fseek(fw,0,SEEK_SET);
	*cnt = size/sizeof(user);
	if(*cnt <= 0) return ERR_LOAD_FAILED;
	ginfo->users = (user**)malloc(sizeof(user*)*(*cnt));
	for(i=0;i<*cnt;i++)
	{
		us = (user*)malloc(sizeof(user));
		fread(us,sizeof(user),1,fw);
		ginfo->users[i] = us;
	}
	fclose(fw);
	return LOAD_SUCCESS;
}

int SaveUser(gameinfo* ginfo)
{
	FILE* fw;
	int size,i;
	fw = fopen("./game.dat","ab");
	if(fw == NULL) return SAVE_FAIL;
	if(ginfo->usidx < 0)
		fseek(fw,0,SEEK_END);		
	else
		fseek(fw,0,sizeof(user)*ginfo->usidx);
	fwrite(ginfo->me,sizeof(user),1,fw);
	fclose(fw);
	return SAVE_SUCCESS;
}
/*int AddUser(user* usrs)
{
	FILE* fw;
	int size,i,cnt;
	fw = fopen("./users.usr","ab");	
	fwrite(usrs,sizeof(user),1,fw);
	fclose(fw);
	return SAVE_SUCCESS;
}*/
