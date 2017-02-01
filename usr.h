#ifndef USER_H
#define USER_H

#include "ConsoleGraphics.h"
#define ERR_LOAD_FAILED 20002
#define ERR_NOUSRFILE 20001
#define LOAD_SUCCESS 20000
#define SAVE_SUCCESS 30000
#define SAVE_FAIL	30001
#define ID_MAX 30
#define START_COIN	0
#define SETCOIN 1
#define MIX(a,b) (a<<8)&b
#define GAME_COUNT 4
typedef struct __USR
{
	int coin;
	char id[ID_MAX+1];
	int scores[GAME_COUNT];
}user;
user* User();
#endif
