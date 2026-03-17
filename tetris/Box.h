#pragma once
#include <memory>  
//块类
class Box
{
private:
	int _data[4][4] = {0};
	int _rolldata[4][4] = {0};          //////////////////
	int _type;

private:
	//七种块，记为
	void Make_1();//---
	void Make_2();//[ ]
	void Make_3();// p
	void Make_4();// q
	void Make_5();// Z
	void Make_6();// s
	void Make_7();//-|
	//双人模式道具
	void Make_8();
	void Make_9();
	void Make_10();
public:
	Box(int type);
	~Box(void) {};

	int GetType();
	int *GetData();

	void Roll();//块转动
	int* GetRollData();

};

