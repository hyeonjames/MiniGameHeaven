#ifndef _ITEM_H
#define _ITEM_H
typedef struct GAMEITEM{
	char text[5];
	float percent;
	int iExist; // 아이템 지속시간
	int iEffect; // 효과 지속시간 
	void (*sfunc)(); // 먹었을 때
	void (*efunc)(); // 끝났을 때
	timer tEffect;
}gameitem;



#endif
