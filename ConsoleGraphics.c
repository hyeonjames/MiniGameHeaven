#include "ConsoleGraphics.h"

int Menu(int y,int x,int count,short set,short unset,short def,char* name,...)
{
	va_list vlist;
	int i,w=0,sel=0,ch=0,ww,bfsel=0,j=0,cy,cx;
	char** menuname;
	curs_set(0);	
	va_start(vlist,name);
	init_pair(MENU_SET,FRONTSH(set),BOTTOMSH(set));
	init_pair(MENU_UNSET,FRONTSH(unset),BOTTOMSH(unset));
	init_pair(MENU_DEFAULT,FRONTSH(def),BOTTOMSH(def));
	menuname = (char**)malloc(sizeof(char*)*count);
		
	for(i=0;i<count;i++)
	{
		Color(MENU_DEFAULT);
		move(y+1+i,x+2);
		if(i != sel) Color(MENU_UNSET);
		else Color(MENU_SET);
		menuname[i] = (char*)va_arg(vlist,char*);
		printw("%s",menuname[i]);
		getyx(stdscr,cy,cx);
		ww = cx - x - 2;
		if(ww > w) w = ww;
		if(i == sel) j = ww;
	}
	va_end(vlist);
	Color(MENU_DEFAULT);
	//Make Square
	move(y,x);
	printw("┏");
	printw("%s",name);
	getyx(stdscr,cy,cx);
	ww = cx-x-2;
	printw(" ");
	if(ww>w) w = ww;
	for(i=0;i<w-ww;i++) printw("━");
	
	printw("┓");
	for(i=0;i<count;i++)
	{
		move(y+i+1,x);
		printw("┃");
		move(y+i+1,x+w+3);
		printw("┃");
	}
	move(y+count+1,x);
	printw("┗");
	for(i=0;i<w+2;i++) printw("━");
	printw("┛");
	//select
	Color(MENU_SET);
	move(y+sel+1,x+2+j);
	for(i=0;i<w-j;i++) printw(" ");
	refresh();	__fpurge(stdin);

	while((ch = getch()) != 10)
	{
		if(ch==ERR) continue;
		bfsel = sel;
		switch(ch)
		{
			case KEY_UP:
				sel--;
				if(sel < 0) sel = count-1;
			break;
			case KEY_DOWN:
				sel++;
				if(sel >= count) sel = 0;
			break;
		}
		if(bfsel!=sel)
		{
			Color(MENU_SET);
			move(y+sel+1,x+2);
			printw("%s",menuname[sel]);
			getyx(stdscr,cy,cx);
			j = cx-x-2;
			for(i=0;i<w-j;i++) printw(" ");
			Color(MENU_UNSET);
			move(y+bfsel+1,x+2);
			printw("%s",menuname[bfsel]);
			getyx(stdscr,cy,cx);
			j = cx-x-2;
			for(i=0;i<w-j;i++) printw(" ");
		}
		
	}
	return sel;
}
void MkSquare(int y,int x,int w,int h)
{
	int i=0;
	move(y,x);
	printw("┏");
	for(i=0;i<w-2;i++) printw("━");
	printw("┓");
	for(i=0;i<h-2;i++)
	{
		move(y+i+1,x);
		printw("┃");
		move(y+i+1,x+w-1);
		printw("┃");
	}
	move(y+h-1,x);
	printw("┗");
	for(i=0;i<w-2;i++) printw("━");
	printw("┛");
	
}
void SetColor(int k,short b,short f)
{
	init_pair(k,f,b);
	attron(COLOR_PAIR(k));
}
void Erase(int y,int x,int w)
{
	int i;
	move(y,x);
	for(i=0;i<w;i++) printw(" ");
}
void EraseSq(int y,int x,int w,int h)
{
	int i;
	for(i=0;i<h;i++) Erase(y+i,x,w);
}
void nPuts(char* k,int n)
{
	char bef = k[n];
	k[n] = '\0';
	printw("%s",k);
	k[n] = bef;
}
void SetBoxUpdown(cbox* box,int min,int max,int def)
{
	int i;
	int *Data;
	if(!(box->type & BTYPE_UPDOWN)) return ;
	Data = (int*)malloc(sizeof(int)*3);	
	Data[0] = min;
	Data[1] = max;
	Data[2] = def;
	box->Data = Data;
}
int GetBoxIndex(cbox* box)
{
	int *Data;
	if(box->Data == NULL || !(box->type & BTYPE_UPDOWN)) return -1;
	Data=(int*)box->Data;	
	return Data[2];
}
void Updown(cbox *box)
{
	int ch;
	int *Data,change=0;	
	if(box->Data == NULL || !(box->type & BTYPE_UPDOWN)) return;
	Data = (int*)box->Data;
	__fpurge(stdin);

	while((ch=getch())!= 10)
	{
		change = 0;
		switch(ch)
		{
			case KEY_UP:
				if(Data[2]+1 <= Data[1])
				{
					Data[2]++;
					change = 1;
				}	
			break;
			case KEY_DOWN:
				if(Data[2]-1 >= Data[0])
				{
					Data[2]--;
					change = 1;
				}
			break;
		}
		if(change)
		{
			_PrintBox(box,box->posy,box->posx);
		}
	}
}
int InverseArrow(int c)
{
	if(c>=2) return c%2;
	else c+2;
}
cbox* MakeBox(int w,int h,int type,int cnt,...)
{
	char* m;
	int i;
	cbox* k;
	va_list vlist;
	va_start(vlist,cnt);
	
	k = (cbox*)malloc(sizeof(cbox));
	k->w = w;
	k->h = h;
	k->cnt = cnt;
	k->text = (char**)malloc(sizeof(char*)*(cnt));
	k->type = type;
	k->index = 0;
	for(i=0;i<cnt;i++)
	{
		m = (char*)va_arg(vlist,char*);
		if(m!=NULL)
		{
			k->text[i] = (char*)malloc(sizeof(char)*(strlen(m)+1));
			strcpy(k->text[i],m);
		}
	}
	return k;
}
void PrintBox(cbox* b,int y,int x,short squ,short def)
{
	int i=0,j=0,cy,cx;
	if(b==NULL) return;
	init_pair(PAIR_COLBOX_BOX,FRONTSH(squ),BOTTOMSH(squ));
	init_pair(PAIR_COLBOX_TEXT,FRONTSH(def),BOTTOMSH(def));
	Color(PAIR_COLBOX_BOX);
	MkSquare(y,x,b->w,b->h);
	Color(PAIR_COLBOX_TEXT);
	b->posx = x;
	b->posy = y;
	if(b->type == BTYPE_DEFAULT)
	{
		for(i=0;i<b->cnt;i++)
		{
			move(y  + i + 1,x + 1);
			if(b->text[i] != NULL) printw("%s",b->text[i]);
			getyx(stdscr,cy,cx);
			for(j=0;j<(x+b->w)-cx-1;j++) printw(" ");
		}
	}
	else
	{
		
		move(y + 1,x + 1);
		printw("%s%d",b->text[0],GetBoxIndex(b));
		getyx(stdscr,cy,cx);
		for(j=0;(x+j<b->w)-cx-1;j++) printw(" ");
	}

	refresh();
}

void _PrintBox(cbox* b,int y,int x)
{
	int i=0,j=0,cy,cx;
	if(b==NULL) return;
	MkSquare(y,x,b->w,b->h);
	b->posx = x;
	b->posy = y;

	if(b->type == BTYPE_DEFAULT)
	{
		for(i=0;i<b->cnt;i++)
		{
			move(y  + i + 1,x + 1);
			if(b->text[i] != NULL) printw("%s",b->text[i]);
			getyx(stdscr,cy,cx);
			for(j=0;j<(x+b->w)-cx-1;j++) printw(" ");
		}
	}
	if(b->type & BTYPE_UPDOWN)
	{
		move(y + 1,x + 1);
		printw("%s%d",b->text[0],GetBoxIndex(b));
		getyx(stdscr,cy,cx);
		for(j=0;j<(x+b->w)-cx-1;j++) printw(" ");
	}
	refresh();
}
int BoxMenu(int y,int x,char* text,short set,short unset,short def,int lineMax,int cnt,...)
{
	int i=0,w=2,len,h=3,maxw=2,maxh=0,sel=0,bfsel=0;
	int ch=0;
	va_list vlist;
	va_start(vlist,cnt);
	cbox** boxes = (cbox**) malloc(sizeof(cbox*)*cnt);
	move(y+1,x+1);
	Color(PAIR_COLBOX_DEFAULT);
	printw("%s",text);
	init_pair(PAIR_COLBOX_SET,FRONTSH(set),BOTTOMSH(set));
	init_pair(PAIR_COLBOX_UNSET,FRONTSH(unset),BOTTOMSH(unset));
	init_pair(PAIR_COLBOX_DEFAULT,FRONTSH(def),BOTTOMSH(def));
	curs_set(0);
	len = strlen(text)+4;
	for(i=0;i<cnt;i++){
		
		boxes[i] = (cbox*)va_arg(vlist,cbox*);	
		if(boxes[i] == NULL) continue;	
		maxw+=boxes[i]->w;
		if(sel == i) Color(PAIR_COLBOX_SET);
		else Color(PAIR_COLBOX_UNSET);
		_PrintBox(boxes[i],y+h,x+maxw-boxes[i]->w);
		maxw++;
		if(boxes[i]->h > maxh) maxh = boxes[i]->h;
		if((i+1)%lineMax == 0)
		{
			if(maxw > w) w = maxw;
			maxw = 2;
			h += maxh;
			maxh = 0;
		}
	}
	if(cnt%lineMax != 0) 
	{
		if(maxw > w) w = maxw;
		h += maxh;
	}
	w+=1;
	if(len > w)
	{
		w = len;
	}
	Color(PAIR_COLBOX_DEFAULT);
	MkSquare(y,x,w,h+1);	__fpurge(stdin);

	while(1)
	{
		ch = getch();
		if(ch==ERR) continue;
		switch(ch)
		{
			case KEY_LEFT:
				sel--;
				if(sel < 0) sel = 0;
			break;
			case KEY_RIGHT:
				sel++;
				if(sel >= cnt) sel--;
			break;
			case KEY_DOWN:
				if(sel+lineMax < cnt) sel += lineMax;
				else sel = cnt -1;
			break;
			case KEY_UP:
				if(sel-lineMax >= 0) sel -= lineMax;
				else sel = 0;
			break;
			case 10:
				if(boxes[sel]->type == BTYPE_DEFAULT)
				{
					va_end(vlist);
					return sel;
				}
				else
				{
					if(boxes[sel]->type & BTYPE_UPDOWN)
					{
						Updown(boxes[sel]);
					}
				}
			break;
		}
		if(bfsel!=sel)
		{
			Color(PAIR_COLBOX_UNSET);
			_PrintBox(boxes[bfsel],boxes[bfsel]->posy,boxes[bfsel]->posx);
			Color(PAIR_COLBOX_SET);
			_PrintBox(boxes[sel],boxes[sel]->posy,boxes[sel]->posx);
			bfsel = sel;
		}
	}
	va_end(vlist);
	return sel;
}
int YesNoMenu(int y,int x,short set,short def,char* ask)
{
	int sel=ID_YES,cx,cy,sx,ch=0;
	init_pair(MENU_SET,FRONTSH(set),BOTTOMSH(set));
	init_pair(MENU_DEFAULT,FRONTSH(def),BOTTOMSH(def));
	move(y,x);
	Color(MENU_DEFAULT);
	printw("%s",ask);
	getyx(stdscr,cy,cx);
	
	curs_set(0);
	move(y+cy-y+1,x);
	Color(MENU_SET);
	printw("1. Yes");
	Color(MENU_DEFAULT);
	printw("\t2. No");	__fpurge(stdin);

	while((ch=getch())!= 10)
	{
		if(ch==ERR) continue;
		switch(ch)
		{
			case KEY_RIGHT: // No
				if(sel == ID_YES)
				{
					move(y+cy-y+1,x);
					Color(MENU_DEFAULT);
					printw("1. Yes\t");
					Color(MENU_SET);
					printw("2. No");
				}
				sel = ID_NO;
			break;
			case KEY_LEFT://Yes
				if(sel == ID_NO)
				{
					move(y+cy-y+1,x);
					Color(MENU_SET);
					printw("1. Yes");
					Color(MENU_DEFAULT);
					printw("\t2. No");
				}
				sel = ID_YES;
			break;
		}
	}
	return sel;
}
