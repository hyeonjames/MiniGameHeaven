#include "luxlazer.h"
int nY,nX,nArr;
int sY,sX;
int isGameRunning;
int score,isGameOver,score_interval,key_interval;
int lazer_interval[4]={1000,1500,300,400}; // Ready for bomb min,max , bomb min,max
double score_rate;
int minMaking,maxMaking;
char LazerWState[LUXHEIGTH]={0};
char LazerHState[LUXWIDTH]={0};
timer LazerWStateT[LUXHEIGTH];
timer LazerHStateT[LUXWIDTH];
int LuxGame()
{
	double k;
	int i=0,j=0;
	int ch,bY,bX,rnd;
	sX = 2; sY = 1;
	minMaking = 1000;
	maxMaking = 1500;
	timer st,keyt,lazer_t;
	isGameRunning = 0;
	isGameOver = 0;

	for(i=0;i<LUXWIDTH;i++) CreateTimer(-1,&LazerHStateT[i]);
	for(i=0;i<LUXHEIGTH;i++) CreateTimer(-1,&LazerWStateT[i]);
		
	clear();
	noecho();
	init_pair(LUX_PAIR_DEFAULT,FRONTSH(LUX_COL_DEFAULT),BOTTOMSH(LUX_COL_DEFAULT));
	init_pair(LUX_PAIR_PLAYER_DEFAULT,FRONTSH(LUX_COL_PLAYER_DEFAULT),BOTTOMSH(LUX_COL_PLAYER_DEFAULT));
	init_pair(LUX_PAIR_READYDAMAGE,FRONTSH(LUX_COL_READYDAMAGE),BOTTOMSH(LUX_COL_READYDAMAGE));
	init_pair(LUX_PAIR_DAMAGE,FRONTSH(LUX_COL_DAMAGE),BOTTOMSH(LUX_COL_DAMAGE));
	Color(LUX_PAIR_DEFAULT);
	nY = LUXHEIGTH/2 - 1;
	nX = LUXWIDTH/2 -1;
		
	for(i=0;i<LUXHEIGTH;i++) LuxFreshLine(1,i);
	nArr = DOWN;
	isGameRunning = 1;
	score = 0;
	score_interval = 100;
	score_rate = 1;
	key_interval = KEY_DELAY;
	CreateTimer(((double)score_interval/score_rate),&st);
timer* t;
	CreateTimer(key_interval,&keyt);
	CreateTimer(RAND(minMaking,maxMaking),&lazer_t);
	while(1)
	{
		bY = nY;
		bX = nX;
		if(isGameOver)
		{
			clear();
			printw("GameOver\n Score : %d",score);
			nodelay(stdscr,0);
			getch();
			nodelay(stdscr,1);
			return score;
		}
		if(isGameRunning)
		{
			ch = getch();
			if(ch != ERR)
			{
		
				if(Check(&keyt))
				{  			
					switch(ch)
					{
					case KEY_LEFT:
						LuxMove(nY,nX-1,LEFT);
						break;
					case KEY_RIGHT:
						LuxMove(nY,nX+1,RIGHT);
						break;
					case KEY_UP:
						LuxMove(nY-1,nX,UP);
						break;
					case KEY_DOWN:
						LuxMove(nY+1,nX,DOWN);
						break;
					}
					if(bY != nY || bX != nX)
					{
						LuxFreshYX(bY,bX);
					}
				}
			}
			if(Check(&st))
			{
				score++;
				RefreshScore();
			}
			
			if(Check(&lazer_t))
			{
				rnd  = 0;
				while(!rnd || !SetLazerReady(rnd)) rnd = RAND(-LUXHEIGTH,LUXWIDTH);
				TimerRestart(&lazer_t);
				lazer_t.interval = RAND(minMaking,maxMaking);
			}
			for(i=0;i<LUXHEIGTH;i++)
			{
				if(Check(&LazerWStateT[i]))
				{
					if(LazerWState[i] == 1)
					{
						SetLazerDamage(1,i);
					}
					else if(LazerWState[i] == 2)
					{
						DelLazer(1,i);
					}	
				}
			}
			for(i=0;i<LUXWIDTH;i++)
			{
				if(Check(&LazerHStateT[i]))
				{
					if(LazerHState[i] == 1)
					{
						SetLazerDamage(0,i);
					}
					else if(LazerHState[i] == 2)
					{
						DelLazer(0,i);
					}	
				}
			}		
		}
	}
	return score;
}



int SetLazerDamage(int bW,int index)
{
	if(bW) // 가로
	{
		if(LazerWState[index] != 1) return -1;
		LazerWState[index] = 2;
		LazerWStateT[index].interval = RAND(lazer_interval[2],lazer_interval[3]);
		TimerRestart(&LazerWStateT[index]);

	}
	else // 세로
	{
		if(LazerHState[index] != 1) return -1;
		LazerHState[index] = 2;
		LazerHStateT[index].interval = RAND(lazer_interval[2],lazer_interval[3]);
		TimerRestart(&LazerHStateT[index]);
	}
	LuxFreshLine(bW,index);
	return 0;
}

int DelLazer(int bW,int index)
{
	if(bW) // 가로
	{
		if(LazerWState[index] != 2) return -1;
		LazerWState[index] = 0;
		LazerWStateT[index].interval = -1;
		move(sY+index,0);
		printw("  ");
	}
	else // 세로
	{
		if(LazerHState[index] != 2) return -1;
		LazerHState[index] = 0;
		LazerHStateT[index].interval = -1;
		move(0,sX+index*2);
		printw("  ");
	}
	LuxFreshLine(bW,index);
	return 0;
}
int SetLazerReady(int k)
{
	int index,i;
	Color(LUX_PAIR_READYDAMAGE);
	if(k>0) // 세로 LuxHState
	{
		index = k-1;
		if(LazerHState[index]) return 0;
		LazerHState[index] = 1;//Lazer Ready
		move(0,sX+index*2);
		printw("!!");
		LazerHStateT[index].interval = (RAND(lazer_interval[0],lazer_interval[1]));
		TimerRestart(&LazerHStateT[index]);
	}
	else if(k<0) // 가로 LuxWState
	{
		index = -k-1;
		if(LazerWState[index]) return 0;
		LazerWState[index] = 1;//Lazer Ready
		move(sY+index,0);
		printw("!!");
		LazerWStateT[index].interval = (RAND(lazer_interval[0],lazer_interval[1]));
		TimerRestart(&LazerWStateT[index]);
	}
	else return 0;
	return 1;
}

void RefreshScore()
{
	Color(LUX_PAIR_DEFAULT);
	move(sY,sX+2*LUXWIDTH+2);
	printw("Score:%d",score);
	
}
void LuxMove(int y,int x,int arr)
{
	if(y >= 0 && y<LUXHEIGTH) nY = y;
	else if(y >= 0) nY = LUXHEIGTH-1;
	else nY = 0;
	if(x >= 0 && x<LUXWIDTH) nX = x;
	else if(x >= 0) nX = LUXWIDTH-1;
	else nX = 0;
	nArr = arr;
	LuxFreshMe();
}
void LuxFreshMe()
{
	if(LazerWState[nY] == 2 || LazerHState[nX] == 2)
	{
		isGameOver = 1;
		return;
	}
	Color(LUX_PAIR_PLAYER_DEFAULT);
	move(nY+sY,sX+nX*2);
	
	switch(nArr)
	{
		case RIGHT:
			printw("▶");
		break;
		case LEFT:
			printw("◀");
		break;
		case UP:
			printw("▲");
		break;
		case DOWN:
			printw("▼");
		break;
			
	}
}

void LuxFreshLine(int bW,int idx)
{
	int i=0;
	if(bW) // 가로?
	{
		for(i=0;i<LUXWIDTH;i++) LuxFreshYX(idx,i);
	}
	else
	{
		for(i=0;i<LUXHEIGTH;i++) LuxFreshYX(i,idx);
	}
}

void LuxFreshYX(int y,int x)
{
	move(sY+y,sX+(x==0 ? 1 : x*2));
	if(LazerWState[y] == 2 || LazerHState[x]==2)
	{
		Color(LUX_PAIR_DAMAGE);
		printw("■");
	}
	else
	{
		Color(LUX_PAIR_DEFAULT);
	
		if(y == 0)
		{
			if(x==0) printw("┏");
			else if(x==LUXWIDTH-1) printw("━┓");
			else printw("━┳");
		}
		else if(y == LUXHEIGTH-1)
		{
			if(x==0) printw("┗");
			else if(x==LUXWIDTH-1) printw("━┛");
			else printw("━┻");
		}
		else
		{
			if(x==0) printw("┣");
			else if(x==LUXWIDTH-1) printw("━┫");
			else printw("━╋");
		}
	}
	if(nY == y && nX == x) LuxFreshMe();
}
