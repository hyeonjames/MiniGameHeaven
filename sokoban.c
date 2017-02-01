#include "sokoban.h"
#define EMPTY 0
#define START 1
#define WALL 2
#define BOX 3
#define GOAL 4
#define isBox(y,x) map[y][x]==BOX
#define isPassable(y,x) (map[y][x] != WALL && map[y][x] != BOX)
char **map;
int width,heigth;
long time,timelimit;
int score,maxscore;
int cnt_map,isGameClear,nY,nX,sY,sX,cBox,cntmove;
FILE* fo;
int LoadNextMap()
{
	int j;
	if(fo==NULL) fopen("./maps.dat","rb");
	if(fo==NULL) return 0;	
	if(map!=NULL) free(map);
	fread(&width,2,fo);
	if(feof(fo)) return 0;
	fread(&height,2,fo);
	if(feof(fo)) return 0;
	//fread(&timelimit,4,fo);
	//if(feof(fo)) return 0;	
	//fread(&maxscore,4,fo);
	//if(feof(fo)) return 0;	
	maxscore = 1000;
	timelimit = 100000;
	map = malloc(sizeof(char*)*height);
	for(j=0;j<height;j++) map[j] = malloc(sizeof(char)*width);
	fread(map,w*h,fo);
	return 1;
}
int bmove(int y,int x,int cy,int cx)
{
	if(!isBox(y,x) || !isPassable(cy,cx)) return 1;
	map[y][x] = EMPTY;
	move(sY+y,sX+x*2); DRAW(y,x);
	if(map[cy][cx] != GOAL)
	{
		map[cy][cx] = BOX;
		move(sY+cy,sX+cx*2); DRAW(cy,cx);
	}
	else cBox--;
	return 0;
}

void PrintMap()
{	
	int i,j;
	cBox=0;
	for(i=0;i<heigth;i++)
	{
		for(j=0;j<width;j++)
		{
			if(map[i][j] == START)
			{
				nY = i;
				nX = j;
				map[i][j] = EMPTY;
			}
			move(sY+i,sX+j*2);
			DRAW(i,j);
			if(isBox(i,j)) cBox++;
		}
	}
}
void StageClear(int score)
{
	int i,cx,cy;
	printw("Stage Clear!\n Score + ");
	getyx(cy,cx);
	for(i=0;i<score;i++)
	{
		move(cy,cx);
		printw("%d",i);
		usleep(10000*10);
	}
	nodelay(stdscr,0);
	getch();
	nodelay(stdscr,1);
}
long SokobanGame()
{
	int i,j, ch,bY,bX,bScore;
	struct timeval before,now;
	clear();
	fo = fopen("./maps.dat","rb");
	if(!LoadNextMap())
	{
		//FAIL Load Map		
		printw("Load map file error. please check have 'maps.dat'\n");
		nodelay(stdscr,0);
		getch();
		nodelay(stdscr,1);
		return -1;
	}
	time=0; score = 0;
	sX = 0;
	sY = 0;
	PrintMap();
	gettimeofday(&before,NULL);
	while(!isGameClear)
	{
		if(cBox == 0)
		{
			bScore = score;
			gettimeofday(&now,NULL);
			time += now.tv_sec - before.tv_sec;
			if(timelimit > time)
				score += maxscore;
			else
				score += maxscore - maxscore*(time - timelimit)/timelimit;
			if(bScore > score) score = bScore;
			StageClear(score - bScore);
			if(!LoadNextMap())
			{
				//GameClear
				isGameClear = 1;
				break;
			}
			
			PrintMap();
			gettimeofday(&before,NULL);			
		}
		bY = nY;
		bX = nX;
		ch = getch();
		switch(ch)
		{
			case DOWN:
				if(isBox(nY+1,nX))
				{
					nY++;
					bmove(nY,nX,nY+1,nX);
				}
				else if(isPassable(nY+1,nX)) nY++;
			break;
			case UP:
				if(isBox(nY-1,nX))
				{
					nY--;
					bmove(nY,nX,nY-1,nX);
				}
				else if(isPassable(nY+1,nX)) nY--;
			break;
			case RIGHT:
				if(isBox(nY,nX+1))
				{
					nX++;
					bmove(nY,nX,nY,nX+1);
				}
				else if(isPassable(nY,nX+1)) nX++;
			break;
			case LEFT:
				if(isBox(nY,nX-1))
				{
					nX--;
					bmove(nY,nX,nY,nX-1);
				}
				else if(isPassable(nY+1,nX)) nX--;
			break;
		}
		if(bY!=nY || bX!=nX)
		{
			move(sY+nY,sX+nX*2);
			DRAW(nY,nX);
			move(sY+bY,sX+bX*2);
			DRAW(nY,nX);
			cntmove++;
		}
	}
	fclose(fo);
	return score;
}

