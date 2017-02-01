#include<stdio.h>

#define EMPTY 0
#define START 1
#define WALL 2
#define BOX 3
#define GOAL 4
#define MAXWH 40
void write(char* s,FILE *fs);
int main(void)
{
	int mlen,i;
	char smaps[MAXWH];
	FILE* fo = fopen("./maps.txt","r");
	FILE* fs = fopen("./maps.dat","wb");
	if(fo==NULL || fs== NULL)
	{
		printw("FAIL");
		return 0;
	}
	while(!feof(fo))
	{
		fscanf(fo,"%s",smaps);
		mlen = strlen(smaps);
		fwrite(mlen,2,1,fs);
		fwrite(mlen,2,1,fs);
		for(i=0;i<mlen-1;i++)
		{
			fscanf(fo,"%s",smaps);		
			write(smaps,fs);
		}
	}
}

void write(char* s,FILE *fs)
{
	int i=0;
	int k = strlen(s);
	char* data = malloc(sizeof(char)*s);
	for(i=0;i<k;i++)
	{
		switch(s[i])
		{
			case 's':
			case 'S':
				data[i] = START;
			break;
			case 'O': case 'o':
				data[i] = EMPTY;
			break;
			case 'X': case 'x':
				data[i] = WALL; break;
			case 'B': case 'b': data[i] = BOX; break;
			case 'G': case 'g': data[i] = GOAL; break;
		}
	}
	fwrite(data,strlen(data)*sizeof(char),1,fs);
}
