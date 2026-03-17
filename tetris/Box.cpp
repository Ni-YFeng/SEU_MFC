#include "pch.h"
#include "Box.h"
#include <Windows.h>
#include <tchar.h>
#include <memory>  

Box::Box(int type)
{
	_type  = type;
	switch(_type)
	{
	case 1:Make_1();break;
	case 2:Make_2();break;
	case 3:Make_3();break;
	case 4:Make_4();break;
	case 5:Make_5();break;
	case 6:Make_6();break;
	case 7:Make_7();break;
	case 8:Make_8();break;
	case 9:Make_9(); break;
	case 10:Make_10(); break;
	}
}




//I
void Box::Make_1()
{
	_data[1][0]=_data[1][1]=_data[1][2]=_data[1][3] = 1; 
}

//O
void Box::Make_2()
{
	_data[1][1]=_data[1][2]=_data[2][1]=_data[2][2] = 1; 
}
// p
void Box::Make_3()
{
	_data[1][1]=_data[1][2]=_data[2][1]=_data[3][1] = 1;
}
// q
void Box::Make_4()
{
	_data[1][0]=_data[1][1]=_data[2][1]=_data[3][1] = 1;
}
// Z
void Box::Make_5()
{
	_data[1][0]=_data[1][1]=_data[2][1]=_data[2][2] = 1;
}
// S
void Box::Make_6()
{
	_data[2][0]=_data[2][1]=_data[1][1]=_data[1][2] = 1;
}
//T
void Box::Make_7()
{
	_data[2][0] = _data[1][1] = _data[2][1] = _data[3][1]=1;
}

void Box::Make_8()
{
	_data[0][0] = _data[0][1] = _data[0][2] = _data[1][0] = _data[2][0] = _data[2][1] = _data[2][2] = _data[1][2] = 1;
	
}
void Box::Make_9()
{
	_data[0][0] = _data[0][1] = _data[0][2] = _data[0][3] = _data[1][1] = _data[2][1] = _data[3][1] = 1;
}
void Box::Make_10()
{
	_data[0][0] = _data[0][3] = _data[1][1] = _data[1][2] = _data[2][1] = _data[2][2] = _data[3][0] = _data[3][3] = 1;
}
int Box::GetType()
{
	return _type;
}

int* Box::GetData()
{
	return &_data[0][0];
}

void Box::Roll()
{
	int temp[4][4];

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			temp[i][j] = _data[i][j];
		}
	}
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			_data[i][j] = temp[j][3 - i];
		}
	}
}

int* Box::GetRollData()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			_rolldata[i][j] = _data[j][3 - i];
		}
	}
	return &_rolldata[0][0];
}
