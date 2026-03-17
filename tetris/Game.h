#pragma once
#include "Box.h"
#include <wtypes.h>
#include <memory>  
#include<iostream>
#include<fstream>
#define ulong ULONG
#define uint UINT
using namespace std;

enum GAME_STATE
{
	HALT,//暂停
	GO,  //工作
	STOP //停止
};                  //枚举变量，游戏状态参数
class Game
{
private:
	GAME_STATE state;        //游戏状态
	int*BigNet;              //Add_box前的游戏网格
	int*Pic_BigNet;          //Add_box后的游戏网格
 	int*SmallNet;            //下一块网格
	// 积木位置，以4x4方格的左上角定义位置
	int Loc_X;
	int Loc_Y;
public:	
	shared_ptr<Box>_Box;     //当前一块
	shared_ptr<Box>next_Box; //下一块
	int  Height;             //游戏网格高度
	int  Width;              //游戏网格宽度
	static const int NET_WIDTH = 9;   //默认游戏网格宽度为9格
	static const int NET_HEIGHT = 17; //默认游戏网格高度为20格
public:
    void AddBox(int *net);   // 将方块添加到堆中 _Box添加到net中
	void NextBox();          // 引出下一个积木
	bool IsDead();           //判断网格是否被填满，游戏结束
	bool CanMoveDown();      //判断当前块能否下降
	bool CanMoveLeft();      //判断当前块能否左移
	bool CanMoveRight();     //判断当前块能否右移
	bool CanRoll();          //判断当前块能否转动
	void Roll();             //当前块转动
	void MoveDown();         //当前块下移
 	void MoveLeft();         //当前块左移
	void MoveRight();        //当前块右移
	void RemoveLines();      //执行消行操作
	void RemoveLine(int index);//消除第index行
	bool CanRemoveLine(int index);//判断第index行能否消除
public:
	Game(int height,int width);  //构造网格高度为height格,宽度为width格的游戏
	~Game(void);                 //析构游戏
	int* GetBigNet();            //返回游戏网格不包含当前块的数据  
	int* GetSmallNet();          //返回下一块网格的数据
	int*GetPic_BigNet();         //返回游戏网格包含当前块的数据
    GAME_STATE GetState();       //返回游戏状态
	void Start();                //游戏开始
	bool Go();                   //游戏运行一步,游戏结束时返回false
    void HaltOrContinue();       //游戏暂停或继续
	void Input(UINT nChar);      //游戏对键盘输入的响应
	int score = 0;               //记录本局游戏分数
	int sta = 0;                 //判断是否要更新分数

public:
	//最优摆放算法
	int*best ;
	void  place();
	void  place_1();//I
	void  place_2();//O
	void  place_3();//J
	void  place_4();//L
	void  place_5();//Z
	void  place_6();//S
	void  place_7();//T
};

 