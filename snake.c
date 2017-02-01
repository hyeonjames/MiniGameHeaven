#include "snake.h"
#define SNAKE_PAIR_SNAKE 24
#define SNAKE_PAIR_DEFAULT 25
#define INIT_SNAKE_HP 500
#define INIT_SPEED 750
#define MAX_SNAKE 35
#define DRAW(y,x) printw("%s",sitems[screen[y][x]].text)
#define INIT_SNAKE 5 // 초기 갯수
#define CNT_SITEM 7
#define SNAKE_COL_SNAKE MIXSH(COLOR_WHITE,COLOR_BLACK)
#define SNAKE_COL_DEFAULT MIXSH(COLOR_RED,COLOR_BLACK)
int isSnakeGameOver = 0,score,isRunning;
double speedrate,scorerate,hungryrate;
int cntSnake=0;
int sX,sY,rolKey,nArr;
char screen[SNAKEHEIGTH][SNAKEWIDTH]={0};
timer tExistItem[SNAKEHEIGTH][SNAKEWIDTH],tEffect[CNT_SITEM-2];
int bSlow=0;
snake snakes[MAX_SNAKE];
int bReverse=0,hp,maxhp;
int score_interval,hungry_interval,snake_interval,grow_interval;
int idropmin,idropmax;
timer tSnakeGo,tScore,tHungry,tGrow,tDrop;
void* scoreup();
void* slow();
void* food();
void* GameOver();
void* slow();
void* end_slow();
void* freeze();
void* end_freeze();
void* scoreup();
void* reverse();
void* end_reverse();
gameitem sitems[CNT_SITEM]={
	{"  ",0,-1,-1,NULL,NULL},
	{"○ ",0,-1,-1,GameOver,NULL},
	{"＊",0.8,15000,-1,food,NULL},
	{"ⓢ ",0.05,6000,10000,slow,end_slow},
	{"ⓕ ",0.025,4000,3000,freeze,end_freeze},
	{"☆ ",0.1,10000,-1,scoreup,NULL},
	{"¿  ",0.01,8000,2500,reverse,end_reverse},
};

void* reverse()
{
	rolKey = RAND(0,3);
}
void* end_reverse()
{
	rolKey = 0;
}
int use_sitem(int y,int x)
{
	int idx = screen[y][x];
	if(idx >= 2)
	{
		sitems[idx].sfunc();
		tEffect[idx-2].interval = sitems[idx].iEffect;
		TimerRestart(&tEffect[idx-2]);
		return 1;
	}
	return 0;
}
void* scoreup()
{
	score+=100;
	RefreshScore2();
}
void* slow()
{
	if(bSlow) return ;
	speedrate -= 0.5;
	hungryrate -= 0.5;
	tSnakeGo.interval = snake_interval/speedrate;
	tGrow.interval = grow_interval/speedrate;
	tHungry.interval = hungry_interval/hungryrate;
	bSlow = 1;
}
void* end_slow()
{
	speedrate += 0.5;
	hungryrate += 0.5;
	tSnakeGo.interval = snake_interval/speedrate;
	tGrow.interval = grow_interval/speedrate;
	tHungry.interval = hungry_interval/hungryrate;
	bSlow = 0;
}
void* freeze()
{
	tSnakeGo.interval = -1;
	tGrow.interval = -1;
	tHungry.interval = -1;
}
void* end_freeze()
{
	tSnakeGo.interval = snake_interval/speedrate;
	tGrow.interval = grow_interval/speedrate;
	tHungry.interval = hungry_interval/hungryrate;
}

void* food()
{
	hp += 250;
	if(hp > maxhp) hp = maxhp;
	RefreshHungry();
	return NULL;
}
void* GameOver()
{
	isSnakeGameOver = 1;
	return NULL;
}

int SnakeGame()
{
	int i,j,ch,bArr;
	snake* s,*temp;
	pthread_t tchk_t;
	clear();
	// game init
	COLSET(SNAKE_PAIR_SNAKE,SNAKE_COL_SNAKE);
	COLSET(SNAKE_PAIR_DEFAULT,SNAKE_COL_DEFAULT);
	hp = maxhp = INIT_SNAKE_HP;
	speedrate = 1;
	scorerate = 1;
	sX = 2; sY = 1;
	rolKey = 0;
	score=0;
	hungry_interval = 100;
	score_interval = 1000;
	snake_interval = 1000;
	grow_interval = 10000;
	idropmin = 1500;
	idropmax = 3000;
	hungryrate = 1;
	isRunning = 1;
	nArr = RIGHT;
	move(0,0);
	isSnakeGameOver = 0;
	// draw game
	Color(SNAKE_PAIR_DEFAULT);
	
	for(i=0;i<SNAKEWIDTH+2;i++) printw("■ ");
	
	for(i=0;i<SNAKEHEIGTH;i++)
	{
		move(i+1,0);
		printw("■ ");
		for(j=0;j<SNAKEWIDTH;j++)
		{
			printw("  ");
			screen[i][j]=0;
		}
		printw("■ ");
		
	}
	for(i=0;i<INIT_SNAKE;i++)
	{
		add(SNAKEHEIGTH/2,(SNAKEWIDTH/2)+(INIT_SNAKE/2)-i-1,RIGHT);
	}
	move(SNAKEHEIGTH+1,0);
	for(i=0;i<SNAKEWIDTH+2;i++) printw("■ ");
	// game start
	GoSnake();	
	CreateTimer(INIT_SPEED/speedrate,&tSnakeGo);
	CreateTimer(grow_interval,&tGrow);
	CreateTimer(score_interval,&tScore);
	CreateTimer(hungry_interval/hungryrate,&tHungry);
	CreateTimer(RAND(idropmin,idropmax),&tDrop);	
	//tchk_t = Thread(TimeCheck,NULL);
	while(!isSnakeGameOver)
	{	
		if(Check(&tDrop))
		{
			tDrop.interval = RAND(idropmin,idropmax);
			Drop();
		}
		for(i=0;i<SNAKEHEIGTH;i++)
		{
			for(j=0;j<SNAKEWIDTH;j++)
			{
				if(tExistItem[i][j].interval > 0 && Check(&tExistItem[i][j]))
				{
					tExistItem[i][j].interval = -1;
					move(sY+i,sX+j*2);
					screen[i][j] = 0;
					DRAW(i,j);
				}
			}
		}
		for(i=0;i<CNT_SITEM-2;i++)
		{
			if(tEffect[i].interval > 0 && Check(&tEffect[i]))
			{
				tEffect[i].interval = -1;
				sitems[i+2].efunc();
			}
		}
		if(Check(&tHungry))
		{
			hp--;
			if(hp <= 0)
			{	
				isSnakeGameOver = 1;
				break;
			}
			RefreshHungry();
		}
		if(Check(&tGrow))
		{
			switch(snakes[cntSnake-1].direction)
			{
				case DOWN:
				add(snakes[cntSnake-1].y-1,snakes[cntSnake-1].x,snakes[cntSnake-1].direction);
				break;
				case UP:
				add(snakes[cntSnake-1].y+1,snakes[cntSnake-1].x,snakes[cntSnake-1].direction);
				break;
				case RIGHT:
				add(snakes[cntSnake-1].y,snakes[cntSnake-1].x-1,snakes[cntSnake-1].direction);
				break;
				case LEFT:
				add(snakes[cntSnake-1].y,snakes[cntSnake-1].x+1,snakes[cntSnake-1].direction);
				break;
			}
			scorerate += 0.1 - (1/(MAX_SNAKE-cntSnake));
			
			tScore.interval = score_interval/scorerate;
			tSnakeGo.interval /= 1-(1/cntSnake);
			snake_interval /= 1-(1/cntSnake);
		}
		if(Check(&tScore))
		{
			score++;
			RefreshScore2();
		}
		if(Check(&tSnakeGo))
		{	
			if(GoSnake())
			{
				isSnakeGameOver = 1;
				break;
			}
		}
		ch = getch();
		bArr = nArr;
		switch(ch)
		{
			case KEY_LEFT:
				if(nArr != RIGHT || !bReverse) nArr = snakes[0].direction = (LEFT+rolKey)%4;
			break;
			case KEY_UP:
				if(nArr != DOWN || !bReverse) nArr = snakes[0].direction = (UP+rolKey)%4;
			break;
			case KEY_DOWN:
				if(nArr != UP || !bReverse) nArr = snakes[0].direction = (DOWN+rolKey)%4;
			break;
			case KEY_RIGHT:
				if(nArr != LEFT || !bReverse) nArr = snakes[0].direction = (RIGHT+rolKey)%4;
			break;
		}	
	}
	cntSnake = 0;
	if(isSnakeGameOver) return score;
	else return -1;
}

void Drop()
{
	int i;
	int x=RAND(0,SNAKEWIDTH-1),y=RAND(0,SNAKEHEIGTH-1);
	int rnd;
	for(i=2;i<CNT_SITEM;i++)
	{
		rnd = RAND(1,10000);
		if(rnd > (sitems[i].percent*10000)) continue;
		while(screen[y][x])
		{	
			x=RAND(0,SNAKEWIDTH-1);
			y=RAND(0,SNAKEHEIGTH-1);
		}
		screen[y][x] = RAND(2,CNT_SITEM-1);
		move(sY+y,sX+x*2);
		DRAW(y,x);
		CreateTimer(sitems[screen[y][x]].iExist,&tExistItem[y][x]);
	}
}
void add(int y,int x,int dr)
{
	snakes[cntSnake].x = x; snakes[cntSnake].y = y; snakes[cntSnake].direction = dr;
	if(dr<0) snakes[cntSnake].direction = snakes[cntSnake-1].direction;
	if(isPassable(y,x)) screen[y][x] = 1;
	cntSnake++;
}

void RefreshHungry()
{
	move(2,SNAKEWIDTH*2+4);
	printw("HP : %d / %d",hp,maxhp);
}

void RefreshScore2()
{
	move(0,SNAKEWIDTH*2+4);
	printw("score:");
	move(1,SNAKEWIDTH*2+4);
	printw("%d",score);
	refresh();
}


int isPassable(int y,int x)
{
	if(y >= SNAKEHEIGTH || y < 0) return 0;
	if(x >= SNAKEWIDTH || x < 0) return 0;
	return screen[y][x]!=1;
}

int GoSnake()
{
	int i,prev,temp,bX,bY,item;
	prev = snakes[0].direction;
	bReverse = 1;
	for(i=0;i<cntSnake;i++)
	{
		bX = snakes[i].x;
		bY = snakes[i].y;
		switch(snakes[i].direction)
		{
			case RIGHT:
				snakes[i].x++;
			break;
			case LEFT:
				snakes[i].x--;
			break;
			case UP:
				snakes[i].y--;
			break;
			case DOWN:
				snakes[i].y++;
			break;
		}
		if(!isPassable(snakes[i].y,snakes[i].x))		
			return 1;
		temp = snakes[i].direction;
		snakes[i].direction = prev;
		item = screen[snakes[i].y][snakes[i].x];
		use_sitem(snakes[i].y,snakes[i].x);	
		screen[snakes[i].y][snakes[i].x] = 1;
		if(bY>=0 && bX>=0 && bY < SNAKEHEIGTH && bX < SNAKEWIDTH) screen[bY][bX] = 0;
		move(sY+bY,sX+bX*2);
		Color(SNAKE_PAIR_DEFAULT);
		DRAW(bY,bX);

		move(sY+snakes[i].y,sX+snakes[i].x*2);
		Color(SNAKE_PAIR_SNAKE);
		DRAW(snakes[i].y,snakes[i].x);
		prev = temp;
	}
	refresh();
	return 0;
}

