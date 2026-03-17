#include "pch.h"
#include "Game.h"
#include"Box.h"
#include"tetrisDlg.h"
#include <memory> 
using namespace std;
extern int score = 0;
extern int calculate = 0;

Game::Game(int height,int width)
{
	srand((unsigned int)time(0));
	Width    = width;
	Height   = height;
	BigNet   = new int[Width*Height];
	SmallNet = new int [16];
	Pic_BigNet = new int[Width*Height];
	best = new int[Width*Height];
	for (int i=0; i < Width*Height; i++)
	{
		BigNet[i] = 0; Pic_BigNet[i] = 0; best[i] = 0;
	}
	for (int i = 0; i < 16; i++)
	{
		SmallNet[i] = 0;
	}
	_Box     = NULL;
	next_Box = NULL;
	state = STOP;
}
Game::~Game(void){}
int* Game::GetPic_BigNet()
{
	for (int i = 0; i < Width*Height; i++) Pic_BigNet[i] =BigNet[i];
	AddBox(Pic_BigNet);
	return Pic_BigNet;
}
int* Game::GetBigNet()
{
	return this->BigNet;
}
int* Game::GetSmallNet()
{
	return this->SmallNet;
} 
GAME_STATE Game::GetState()
{
	return state;
}

// 开始 
void Game::Start()
{
	_Box = NULL;
	next_Box = NULL;
	state = GO;
	for (int i = 0; i < Height*Width; i++) BigNet[i] = 0;
	srand((unsigned int)time(0));
	NextBox();
	NextBox();
}
// 暂停或继续
void Game::HaltOrContinue()
{
	if (state == HALT)
	{
		state = GO;
	}
	else if (state == GO)
	{
		state = HALT;
	}
}
// 运行一步
bool Game::Go()
{
	if(CanMoveDown())
	{
		MoveDown();
		return true;
	}
	else
	{
		AddBox(this->BigNet);
		RemoveLines();
		if(IsDead())
		{
			state = STOP;
			return false;
		}
		else
		{
			NextBox();
			return true;
		}
	}	
}
void Game::Input(UINT nChar)
{
	if (state == STOP) return;
	switch (nChar)
	{
	case 65:case 100:if (CanMoveLeft())MoveLeft(); break;
	case 68:case 102:if (CanMoveRight()) MoveRight(); break;
	case 87:case 104:if (CanRoll())     Roll(); break;
	case 83:case 101:;if (CanMoveDown())MoveDown(); break;
	case 71:case 96:while (CanMoveDown()) MoveDown(); break;
	}
}
void Game::AddBox(int *net)
{
	if(net!=NULL && _Box!=NULL && BigNet!=NULL)
	{
		if(net!=BigNet)
			for (int i = 0; i < Height*Width; i++)
			{
				net[i] = BigNet[i];
			}
		for(int i=0;i<4;i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (i + Loc_Y >= 0 &&
					i + Loc_Y < Height  &&
					j + Loc_X >= 0 &&
					j + Loc_X < Width   &&
					_Box->GetData()[i * 4 + j])
					net[(i + Loc_Y)*Width + j + Loc_X] = 1;
			}
		}
	}
}
void Game::NextBox()
{
	// 小窗口积木出场，更换新的积木，设置出场位置
	Loc_Y = 0;
	Loc_X = (Width - 4) / 2;
	_Box = std::move(next_Box);
	
	//更新小窗口信息
	next_Box = std::make_shared<Box>(rand() % 7 + 1);

	SmallNet = next_Box->GetData();
}
bool Game::IsDead()
{
	int count = 0;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (next_Box->GetData()[i * 4 + j])
			{
				count++;
			}
		}
	}
	int cnt1 = count,cnt2=0,x = (Width-4)/2,y = 0;
	int *temp = new int[Height*Width];

	// 复制一个副本，统计原有方块数+Box中的块数
	for(int i=0;i<Height*Width;i++)
	{
		temp[i] = BigNet[i];
		cnt1 += temp[i];
	}
	// 假设发生变换
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (i + y >= 0 &&
				i + y < Height  &&
				j + x >= 0 &&
				j + x < Width   &&
				next_Box->GetData()[i * 4 + j])
				temp[(i + y)*Width + j + x] = 1;
		}
	}
	// 统计变换后方块数
	for (int i = 0; i < Height*Width; i++)
	{
		cnt2 += temp[i];
	}
	delete[]temp;
	return cnt2!=cnt1;
}
bool Game::CanMoveDown()
{
	int count = 0;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (_Box->GetData()[i * 4 + j])
			{
				count++;
			}
		}
	}
	int cnt1 = count,cnt2=0,x = Loc_X,y = Loc_Y+1;
	int *temp = new int[Height*Width];

	// 复制一个副本，统计原有方块数+Box中的块数
	for (int i = 0; i < Height*Width; i++)
	{
		temp[i] = BigNet[i];
		cnt1 += temp[i];
	}
	// 假设发生变换
	for(int i=0;i<4;i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (i + y >= 0 &&
				i + y < Height  &&
				j + x >= 0 &&
				j + x < Width   &&
				_Box->GetData()[i * 4 + j])
				temp[(i + y)*Width + j + x] = 1;
		}
	}

	// 统计变换后方块数
	for (int i = 0; i < Height*Width; i++)
	{
		cnt2 += temp[i];
	}
	delete[]temp;
	return cnt2==cnt1;
}
bool Game::CanMoveLeft()
{
	int count = 0;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (_Box->GetData()[i * 4 + j])
			{
				count++;
			}
		}
	}
	int cnt1 = count,cnt2=0,x = Loc_X-1,y = Loc_Y;
	int *temp = new int[Height*Width];

	// 复制一个副本，统计原有方块数+Box中的块数
	for (int i = 0; i < Height*Width; i++)
	{
		temp[i] = BigNet[i];
		cnt1 += temp[i];
	}
	// 假设发生变换
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (i + y >= 0 &&
				i + y < Height  &&
				j + x >= 0 &&
				j + x < Width   &&
				_Box->GetData()[i * 4 + j])
				temp[(i + y)*Width + j + x] = 1;
		}
	}
	// 统计变换后方块数
	for (int i = 0; i < Height*Width; i++)
	{
		cnt2 += temp[i];
	}
	delete[]temp;
	return cnt2==cnt1;
}
bool Game::CanMoveRight()
{
	int count = 0;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (_Box->GetData()[i * 4 + j])
			{
				count++;
			}
		}
	}
	int cnt1 = count,cnt2=0,x = Loc_X+1,y = Loc_Y;
	int *temp = new int[Height*Width];

	// 复制一个副本，统计原有方块数+Box中的块数
	for (int i = 0; i < Height*Width; i++)
	{
		temp[i] = BigNet[i];
		cnt1 += temp[i];
	}
	// 假设发生变换
	for(int i=0;i<4;i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (i + y >= 0 &&
				i + y < Height  &&
				j + x >= 0 &&
				j + x < Width   &&
				_Box->GetData()[i * 4 + j])
				temp[(i + y)*Width + j + x] = 1;
		}
	}
	// 统计变换后方块数
	for (int i = 0; i < Height*Width; i++)
	{
		cnt2 += temp[i];
	}
	delete[]temp;
	return cnt2==cnt1;
}
bool Game::CanRoll()
{
	int count = 0;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (_Box->GetData()[i * 4 + j])
			{
				count++;
			}
		}
	}
	int cnt1 = count,cnt2=0,x = Loc_X,y = Loc_Y;
	int *temp = new int[Height*Width];
	int *rollData = _Box->GetRollData();

	// 复制一个副本，统计原有方块数+Box中的块数
	for(int i=0;i<Height*Width;i++)
	{
		temp[i] = BigNet[i];
		cnt1 += temp[i];
	}

	// 假设发生变换
	for(int i=0;i<4;i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (i + y >= 0 &&
				i + y < Height  &&
				j + x >= 0 &&
				j + x < Width   &&
				rollData[i * 4 + j])
				temp[(i + y)*Width + j + x] = 1;
		}
	}

	// 统计变换后方块数
	for (int i = 0; i < Height*Width; i++)
	{
		cnt2 += temp[i];
	}
	delete[]temp;
	return cnt2==cnt1;
}
void Game::RemoveLines()
 {
	int j = 0;
	for (int i = Height - 1; i > 0; i--)
	{
		while (CanRemoveLine(i))
		{
			RemoveLine(i); 
			j++;
			sta = 1;
			
		}
	}
	if (j > 0) sta = 1;
	score += 10 * j *j;
	
}
void Game::RemoveLine(int index)
{
	for (int i = index; i > 0; i--)
	{
		for (int j = 0; j < Width; j++)
			BigNet[i*Width + j] = BigNet[(i - 1)*Width + j];
	}
	for (int j = 0; j < Width; j++)
	{
		BigNet[j] = 0;
	}
}
bool Game::CanRemoveLine(int index)
{
	int count = 0;
	for (int j = 0; j < Width; j++)
	{
		if (BigNet[index*Width + j] == 1)
			count++;
	}
	return count==Width;
}
void Game::Roll()
{
	if(_Box)
		_Box->Roll();
}
void Game::MoveDown()
{
	Loc_Y++;
}
void Game::MoveLeft()
{
	Loc_X--;
}
void Game::MoveRight()
{
	Loc_X++;
}

//拓展功能
double Score(int*net, int Width, int Height)
{
	int height = 0;               //最大高度
	int height_diff_sum = 0;      //高度差
	int space_num = 0;            //空格数
	int complete = 0;             //消除行数
	int space_row = 0;
	if (net == NULL) return 0;

	//height
	int sum_1 = 0;
	for (int i = Height - 1;i>=0; i--)
	{
		if (i == 0)height = Height;
		for (int j = 0; j < Width; j++)
		{
			sum_1 += net[i*Width + j];
		}
		if (sum_1 == 0)
		{
			height = Height - 1 - i; break;
		}
		sum_1 = 0;
	}
	//space_num
	for (int i = Height * Width-1;i>=(Height-height)*Width-1;i--)
	{
		if (net[i] == 0)space_num++;
	}
	//height_diff_sum
	int*num_2 = new int[Width];
	for (int j = 0; j < Width; j++)
	{
		int i = 0;
		while (net[i*Width + j] == 0)
		{
			i++;
			if (i == Height) break;
		}
		num_2[j] = i;
	}
	for (int j = 0; j < Width - 1; j++)
	{
		height_diff_sum += num_2[j + 1] - num_2[j];
	}
	delete[]num_2;
	//space_row
	for (int j = 0; j < Width; j++)
	{
		int i = 0;
		while (net[i*Width + j] == 0)
		{
			if (i == Height - 1) break;
			i++;
		}
		i++;
		for (; i < Height; i++)
		{
			if (net[i*Width + j] == 0) space_num++;
		}
	}
	for (int i = Height-1; i >= Height-height; i--)
	{
		int sum = 0;
		for (int j = 0; j < Width; j++)
		{
			sum += net[i*Width + j];
		}
		if (Width - sum > 1) space_row++;
	}
	//complete
	for (int i = 0; i < Height; i++)
	{
		int sum = 0;
		for (int j = 0; j < Width; j++)
		{
			sum += net[i*Width + j];
		}
		if (sum == Width) { complete++; }
	}
	return (double)complete + 2.0 / (1.0 + (double)height * (double)height_diff_sum*(double)space_num*(double)space_row);
}
void Game::place()
{
	for (int i = 0; i < Width*Height; i++)
	{
		best[i] = GetBigNet()[i];
	}
	if (_Box == NULL) {
		return;
	}  
	int type = _Box->GetType();
	switch (type)
	{
		case 1: place_1(); break;
		case 2: place_2(); break;
		case 3: place_3(); break;
		case 4: place_4(); break;
		case 5: place_5(); break;
		case 6: place_6(); break;
		case 7: place_7(); break;
	}
}
void Game::place_1()//---
{
	double max_score = 0;
	int*temp = new int[Width*Height];
	for (int i = 0; i < Width*Height; i++)
	{
		temp[i] = BigNet[i];
	}
	//0'
	for (int i = 0; i < Width - 3; i++)
	{
		for (int j = 0; j < Height; j++)
		{
			int order = j * Width + i; //点位
			if (temp[order] == 1 || temp[order+1] == 1 || temp[order+2] == 1 || temp[order+3] == 1)break;
			temp[order] = temp[order + 1] =temp[order + 2 ] = temp[order + 3 ] = 1;
			/*
			if (temp[order + Width] == 0 && temp[order + 1 + Width] == 0 && temp[order + 2 + Width] == 0 && temp[order + 3 + Width] == 0)
			{
				temp[order] = temp[order + 1] = temp[order + 2] = temp[order + 3] = 0;
				break;
			}
			*/
			
			if (Score(temp, Width, Height) > max_score)
			{
				max_score = Score(temp, Width, Height);
				for (int i = 0; i <Width*Height; i++) best[i] = temp[i];
			}
			temp[order] = temp[order + 1] = temp[order + 2] = temp[order + 3] = 0;
		}
	}
	//+90'
	for (int i = 0; i < Width; i++)
	{
		for (int j = 0; j < Height-3; j++)
		{
			int order = j * Width + i; //点位
			if (temp[order] == 1 || temp[order + Width] == 1 ||temp[order + 2 * Width] == 1 || temp[order + 3 * Width] == 1)break;
			temp[order] = temp[order + Width] =temp[order + 2 * Width] = temp[order + 3 * Width] = 1;
			/*
			if (temp[order + Width] == 0 && temp[order + 2 * Width] == 0 && temp[order + 3 * Width] == 0 && temp[order + 4 * Width] == 0)
			{
				temp[order] = temp[order + Width] = temp[order + 2 * Width] = temp[order + 3 * Width] = 0;
				break;
			}
			*/
			
			if (Score(temp, Width, Height) > max_score)
			{
				max_score = Score(temp, Width, Height);
				for (int i = 0; i < Width*Height; i++) best[i] = temp[i];
			}
			temp[order] = temp[order + Width] = temp[order + 2 * Width] = temp[order + 3 * Width] = 0;
		}
	}
	delete[]temp;
}
void Game::place_2()//[]
{
	double max_score = 0;
	int*temp = new int[Width*Height];
	for (int i = 0; i < Width*Height; i++)
	{
		temp[i] = BigNet[i];
	}
	for (int i = 0; i < Width - 1; i++)
	{
		for (int j = 0; j < Height-1; j++)
		{
			int order = j * Width + i; //点位
			
			if (temp[order] == 1 || temp[order + 1] == 1 || temp[order+Width] == 1 || temp[order+Width+1] == 1)break;
			temp[order] = temp[order + 1] = temp[order + Width] = temp[order + Width + 1] = 1;
			/*
			if (temp[order + Width] == 0 && temp[order + 1 + Width] == 0 && temp[order + 2 * Width] == 0 && temp[order + 1 + 2 * Width] == 0)
			{
				temp[order] = temp[order + 1] = temp[order + Width] = temp[order + Width + 1] = 0;
				break;
			}
			*/
			if (Score(temp, Width, Height) > max_score)
			{
				max_score = Score(temp, Width, Height);
				for (int i = 0; i < Width*Height; i++) best[i] = temp[i];
			}
			temp[order] = temp[order + 1] = temp[order + Width] = temp[order + Width + 1] = 0;
		}
	}
	delete[]temp;
}
void Game::place_3()//p
{
	double max_score=0;
	int*temp = new int[Width*Height];
	for (int i = 0; i < Width*Height; i++)
	{
		temp[i] = BigNet[i];
	}
	//0'
	for (int i = 0; i < Width - 1; i++)
	{
		for (int j = 0; j < Height-2; j++)
		{
			int order = j * Width + i; //点位
			if (temp[order] == 1 || temp[order + 1] == 1 || temp[order + Width] == 1 || temp[order + 2*Width] == 1)break;
			temp[order] = temp[order + 1] = temp[order + Width] = temp[order + 2 * Width] = 1;
			/*
			if (temp[order + Width] == 0 && temp[order + 1 + Width] == 0 && temp[order + 2 * Width] == 0 && temp[order + 3 * Width] == 0)
			{
				temp[order] = temp[order + 1] = temp[order + Width] = temp[order + 2*Width ] = 0;
				break;
			}
			*/
			
			if (Score(temp, Width, Height) > max_score)
			{
				max_score = Score(temp, Width, Height);
				for (int i = 0; i < Width*Height; i++) best[i] = temp[i];
			}
			temp[order] = temp[order + 1] = temp[order + Width] = temp[order + 2 * Width] = 0;
		}
	}
	//+90'
	for (int i = 0; i < Width-2; i++)
	{
		for (int j = 0; j < Height - 1; j++)
		{
			int order = j * Width + i; //点位
			if (temp[order] == 1 || temp[order + Width] == 1 || temp[order + 1+ Width] == 1 || temp[order + 2+ Width] == 1)break;
			temp[order] = temp[order + Width] = temp[order + 1 + Width] = temp[order + 2 + Width] = 1;
			/*
			if (temp[order + Width] == 0 && temp[order + 2* Width] == 0 && temp[order + 1+2 * Width] == 0 && temp[order + 2+2* Width] == 0)
			{
				temp[order] = temp[order + Width] = temp[order + 1 + Width] = temp[order + 2 + Width] = 0;
				break;
			}
			*/
			if (Score(temp, Width, Height) > max_score)
			{
				max_score = Score(temp, Width, Height);
				for (int i = 0; i < Width*Height; i++) best[i] = temp[i];
			}
			temp[order] = temp[order + Width] = temp[order + 1 + Width] = temp[order + 2 + Width] = 0;
		}
	}
	//+180'
	for (int i = 0; i < Width-1; i++)
	{
		for (int j = 0; j < Height - 2; j++)
		{
			int order = j * Width + i; //点位
			if (temp[order+1] == 1 || temp[order+1+Width] == 1 || temp[order+1+2*Width] == 1|| temp[order+2*Width] == 1 )break;
			temp[order+1] = temp[order + 1 + Width] = temp[order + 1 + 2 * Width] = temp[order + 2 * Width] = 1;
			/*
			if (temp[order +1+ Width] == 0 && temp[order + 1+2 * Width] == 0 && temp[order + 1 + 3 * Width] == 0 && temp[order + 3 * Width] == 0)
			{
				temp[order + 1] = temp[order + 1 + Width] = temp[order + 1 + 2 * Width] = temp[order + 2 * Width] = 0;
				break;
			}
			*/
			
			if (Score(temp, Width, Height) > max_score)
			{
				max_score = Score(temp, Width, Height);
				for (int i = 0; i < Width*Height; i++) best[i] = temp[i];
			}
			temp[order + 1] = temp[order + 1 + Width] = temp[order + 1 + 2 * Width] = temp[order + 2 * Width] = 0;
		}
	}
	//+270'
	for (int i = 0; i < Width-2; i++)
	{
		for (int j = 0; j < Height - 1; j++)
		{
			int order = j * Width + i; //点位
	
			if (temp[order]==1||temp[order+1]==1||temp[order+2]==1||temp[order+2+Width] == 1)break;
			 temp[order] = temp[order+1] = temp[order+2] = temp[order+2+Width] = 1;
			 /*
			  if (temp[order +  Width] == 0 && temp[order + 1 +  Width] == 0 && temp[order +2+ Width] == 0 && temp[order +2+ 2 * Width] == 0)
			 {
				 temp[order] = temp[order + 1] = temp[order + 2] = temp[order + 2 + Width] = 0;
				 break;
			 }
			 */
			
			if (Score(temp, Width, Height) > max_score)
			{
				max_score = Score(temp, Width, Height);
				for (int i = 0; i < Width*Height; i++) best[i] = temp[i];
			}
			temp[order] = temp[order + 1] = temp[order + 2] = temp[order + 2 + Width] = 0;
		}
	}
	delete[]temp;
}
void Game::place_4()//q
{
	double max_score=0;
	int*temp = new int[Width*Height];
	for (int i = 0; i < Width*Height; i++)
	{
		temp[i] = BigNet[i];
	}
	//0'
	for (int i = 0; i < Width - 1; i++)
	{
		for (int j = 0; j < Height - 2; j++)
		{
			int order = j * Width + i; //点位
			
			if (temp[order] == 1 || temp[order + 1] == 1 || temp[order +1+ Width] == 1 || temp[order +1+ 2 * Width] == 1)break;
			temp[order] = temp[order + 1] = temp[order +1+ Width] = temp[order +1+ 2 * Width] = 1;
			/*
				if (temp[order + Width] == 0 && temp[order + 1 + Width] == 0 && temp[order + 1+ 2* Width] == 0 && temp[order + 1 + 3 * Width] == 0)
			{
				temp[order] = temp[order + 1] = temp[order + 1 + Width] = temp[order + 1 + 2 * Width] = 0;
				break;
			}
			*/
		
			if (Score(temp, Width, Height) > max_score)
			{
				max_score = Score(temp, Width, Height);
				for (int i = 0; i < Width*Height; i++) best[i] = temp[i];
			}
			temp[order] = temp[order + 1] = temp[order + 1 + Width] = temp[order + 1 + 2 * Width] = 0;
		}
	}
	//+90'
	for (int i = 0; i < Width-2; i++)
	{
		for (int j = 0; j < Height - 1; j++)
		{
			int order = j * Width + i; //点位
			if (temp[order] == 1 || temp[order + Width] == 1 || temp[order + 1] == 1 || temp[order + 2 ] == 1)break;
			temp[order] = temp[order + Width] = temp[order + 1] = temp[order + 2 ] = 1;
			/*
				if (temp[order + Width] == 0 && temp[order + 2* Width] == 0 && temp[order + 1 +  Width] == 0 && temp[order + 2+ Width] == 0)
			{
				temp[order] = temp[order + Width] = temp[order + 1] = temp[order + 2] = 0;
				break;
			}
			*/
		
			if (Score(temp, Width, Height) > max_score)
			{
				max_score = Score(temp, Width, Height);
				for (int i = 0; i < Width*Height; i++) best[i] = temp[i];
			}
			temp[order] = temp[order + Width] = temp[order + 1] = temp[order + 2] = 0;
		}
	}
	//+180'
	for (int i = 0; i < Width - 1; i++)
	{
		for (int j = 0; j < Height - 2; j++)
		{
			int order = j * Width + i; //点位
			if (temp[order] == 1 || temp[order  + Width] == 1 || temp[order  + 2 * Width] == 1 || temp[order + 2 * Width+1] == 1)break;
			temp[order] = temp[order + Width] = temp[order + 2 * Width] = temp[order + 2 * Width+1] = 1;
			/*
				if (temp[order + Width] == 0 && temp[order + 2 * Width] == 0 && temp[order + 3* Width] == 0 && temp[order +3* Width+1] == 0)
			{
				temp[order] = temp[order + Width] = temp[order + 2 * Width] = temp[order + 2 * Width + 1] = 0;
				break;
			}
			*/
		
			if (Score(temp, Width, Height) > max_score)
			{
				max_score = Score(temp, Width, Height);
				for (int i = 0; i < Width*Height; i++) best[i] = temp[i];
			}
			temp[order] = temp[order + Width] = temp[order + 2 * Width] = temp[order + 2 * Width + 1] = 0;
		}
	}
	//+270'
	for (int i = 0; i < Width - 2; i++)
	{
		for (int j = 0; j < Height - 1; j++)
		{
			int order = j * Width + i; //点位
			if (temp[order+2] == 1 || temp[order + Width] == 1 || temp[order+ Width+1] == 1 || temp[order + 2 + Width] == 1)break;
			temp[order+2] = temp[order +Width] = temp[order +Width+1] = temp[order + 2 + Width] = 1;
			/*
			if (temp[order + 2+Width] == 0 && temp[order + 2 * Width] == 0 && temp[order + 2 * Width+1] == 0 && temp[order + 2+2* Width] == 0)
			{
				temp[order + 2] = temp[order + Width] = temp[order + Width + 1] = temp[order + 2 + Width] = 0;
				break;
			}
			*/
			
			if (Score(temp, Width, Height) > max_score)
			{
				max_score = Score(temp, Width, Height);
				for (int i = 0; i < Width*Height; i++) best[i] = temp[i];
			}
			temp[order + 2] = temp[order + Width] = temp[order + Width + 1] = temp[order + 2 + Width] = 0;
		}
	}
	delete[]temp;
}
void Game::place_5()// Z
{
	double max_score=0;
	int*temp = new int[Width*Height];
	for (int i = 0; i < Width*Height; i++)
	{
		temp[i] = BigNet[i];
	}
	//0'
	for (int i = 0; i < Width - 2; i++)
	{
		for (int j = 0; j < Height-1; j++)
		{
			int order = j * Width + i; //点位
			
			if (temp[order] == 1 || temp[order + 1] == 1 || temp[order+1+Width] == 1 || temp[order+2+Width] == 1)break;
			temp[order] = temp[order + 1] = temp[order + 1 + Width] = temp[order + 2 + Width] = 1;
			/*
			if (temp[order + Width] == 0 && temp[order + 1+ Width] == 0 && temp[order + 2 * Width + 1] == 0 && temp[order + 2 + 2 * Width] == 0)
			{
				temp[order] = temp[order + 1] = temp[order + 1 + Width] = temp[order + 2 + Width] = 0;
				break;
			}
			*/
			
			if (Score(temp, Width, Height) > max_score)
			{
				max_score = Score(temp, Width, Height);
				for (int i = 0; i < Width*Height; i++) best[i] = temp[i];
			}
			temp[order] = temp[order + 1] = temp[order + 1 + Width] = temp[order + 2 + Width] = 0;
		}
	}
	//+90'
	for (int i = 0; i < Width-1; i++)
	{
		for (int j = 0; j < Height - 2; j++)
		{
			int order = j * Width + i; //点位
			if (temp[order+1] == 1 || temp[order +1+ Width] == 1 || temp[order + 2* Width] == 1 || temp[order + Width] == 1)break;
			temp[order+1] = temp[order +1+ Width] = temp[order + 2 * Width] = temp[order +  Width] = 1;
			/*
			if (temp[order + 1+Width] == 0 && temp[order + 1 + 2*Width] == 0 && temp[order + 3 * Width] == 0 && temp[order +  2 * Width] == 0)
			{
				temp[order + 1] = temp[order + 1 + Width] = temp[order + 2 * Width] = temp[order + Width] = 0;
				break;
			}
			*/
			
			if (Score(temp, Width, Height) > max_score)
			{
				max_score = Score(temp, Width, Height);
				for (int i = 0; i < Width*Height; i++) best[i] = temp[i];
			}
			temp[order + 1] = temp[order + 1 + Width] = temp[order + 2 * Width] = temp[order + Width] = 0;
		}
	}
	delete[]temp;
}
void Game::place_6()// s
{
	int*best = new int[Width*Height];
	double max_score=0;
	int*temp = new int[Width*Height];
	for (int i = 0; i < Width*Height; i++)
	{
		temp[i] = BigNet[i];
	}
	//0'
	for (int i = 0; i < Width - 2; i++)
	{
		for (int j = 0; j < Height - 1; j++)
		{
			int order = j * Width + i; //点位
			
			if (temp[order+1] == 1 || temp[order +2] == 1 || temp[order + Width] == 1 || temp[order + 1 + Width] == 1)break;
			temp[order+1] = temp[order + 2] = temp[order + Width] = temp[order + 1 + Width] = 1;
			/*
			if (temp[order + 1 + Width] == 0 && temp[order +  2 + Width] == 0 && temp[order + 2 * Width] == 0 && temp[order +1+ 2 * Width] == 0)
			{
				temp[order + 1] = temp[order + 2] = temp[order + Width] = temp[order + 1 + Width] = 0;
				break;
			}
			*/
			
			if (Score(temp, Width, Height) > max_score)
			{
				max_score = Score(temp, Width, Height);
				for (int i = 0; i < Width*Height; i++) best[i] = temp[i];
			}
			temp[order + 1] = temp[order + 2] = temp[order + Width] = temp[order + 1 + Width] = 0;
		}
	}
	//+90'
	for (int i = 0; i < Width - 1; i++)
	{
		for (int j = 0; j < Height - 2; j++)
		{
			int order = j * Width + i; //点位
			
			if (temp[order] == 1 || temp[order + Width] == 1 || temp[order+1+Width] == 1 || temp[order +1+2* Width] == 1)break;
			temp[order] = temp[order+ Width] = temp[order+1+Width] = temp[order+1+2*Width] = 1;
			/*
			if (temp[order +  Width] == 0 && temp[order + 2 *Width] == 0 && temp[order + 1+2 * Width] == 0 && temp[order + 1 + 3 * Width] == 0)
			{
				temp[order] = temp[order + Width] = temp[order + 1 + Width] = temp[order + 1 + 2 * Width] = 0;
				break;
			}
			*/
			if (Score(temp, Width, Height) > max_score)
			{
				max_score = Score(temp, Width, Height);
				for (int i = 0; i < Width*Height; i++) best[i] = temp[i];
			}
			temp[order] = temp[order + Width] = temp[order + 1 + Width] = temp[order + 1 + 2 * Width] = 0;
			
		}
	}
	delete[]temp;
}
void Game::place_7()//-|
{
	double max_score=0;
	int*temp = new int[Width*Height];
	for (int i = 0; i < Width*Height; i++)
	{
		temp[i] = BigNet[i];
	}
	//0'
	for (int i = 0; i < Width - 1; i++)
	{
		for (int j = 0; j < Height - 2; j++)
		{
			int order = j * Width + i; //点位
			if (temp[order+1] == 1 || temp[order+Width] == 1 || temp[order+Width+1] == 1 || temp[order +1+ 2 * Width] == 1)break;
			temp[order+1] = temp[order +Width] = temp[order +1+ Width] = temp[order +1+ 2 * Width] = 1;
			/*
				if (temp[order + 1+Width] == 0 && temp[order + 2 * Width] == 0 && temp[order + 1 + 2 * Width] == 0 && temp[order + 1 + 3 * Width] == 0)
			{
				temp[order + 1] = temp[order + Width] = temp[order + 1 + Width] = temp[order + 1 + 2 * Width] = 0;
				break;
			}
			*/
			if (Score(temp, Width, Height) > max_score)
			{
				max_score = Score(temp, Width, Height);
				for (int i = 0; i < Width*Height; i++) best[i] = temp[i];
			}
			temp[order + 1] = temp[order + Width] = temp[order + 1 + Width] = temp[order + 1 + 2 * Width] = 0;
		}
	}
	//+90'
	for (int i = 0; i < Width - 2; i++)
	{
		for (int j = 0; j < Height - 1; j++)
		{
			int order = j * Width + i; //点位
			if (temp[order] == 1 || temp[order +1] == 1 || temp[order +2] == 1 || temp[order+1+Width] == 1)break;
			temp[order] = temp[order+1] = temp[order +2] = temp[order +1+ Width] = 1;
			/*
			if (temp[order + Width] == 0 && temp[order + 1+Width] == 0 && temp[order + 2 + Width] == 0 && temp[order + 1 + 2 * Width] == 0)
			{
				temp[order] = temp[order + 1] = temp[order + 2] = temp[order + 1 + Width] = 0;
				break;
			}
			*/
			if (Score(temp, Width, Height) > max_score)
			{
				max_score = Score(temp, Width, Height);
				for (int i = 0; i < Width*Height; i++) best[i] = temp[i];
			}
			temp[order] = temp[order + 1] = temp[order + 2] = temp[order + 1 + Width] = 0;
		}
	}
	//+180'
	for (int i = 0; i < Width - 1; i++)
	{
		for (int j = 0; j < Height - 2; j++)
		{
			int order = j * Width + i; //点位
			if (temp[order] == 1 || temp[order+ Width] == 1 || temp[order+2* Width] == 1 || temp[order+Width+1] == 1)break;
			temp[order] = temp[order+Width] = temp[order +2 * Width] = temp[order+1+Width] = 1;
			/*
			if (temp[order + Width] == 0 && temp[order + 2* Width] == 0 && temp[order + 3* Width] == 0 && temp[order + 1 + 2 * Width] == 0)
			{
				temp[order] = temp[order + Width] = temp[order + 2 * Width] = temp[order + 1 + Width] = 0;
				break;
			}
			*/
			
			if (Score(temp, Width, Height) > max_score)
			{
				max_score = Score(temp, Width, Height);
				for (int i = 0; i < Width*Height; i++) best[i] = temp[i];
			}
			temp[order] = temp[order + Width] = temp[order + 2 * Width] = temp[order + 1 + Width] = 0;
		}
	}
	//+270'
	for (int i = 0; i < Width - 2; i++)
	{
		for (int j = 0; j < Height - 1; j++)
		{
			int order = j * Width + i; //点位
			if (temp[order+1] == 1 || temp[order +Width] == 1 || temp[order +Width+1] == 1 || temp[order + 2 + Width] == 1)break;
			temp[order+1] = temp[order +Width] = temp[order+Width+1] = temp[order + 2 + Width] = 1;
			/*
			if (temp[order + 1+Width] == 0 && temp[order + 2 * Width] == 0 && temp[order + 2* Width+1] == 0 && temp[order + 2+2 * Width] == 0)
			{
				temp[order + 1] = temp[order + Width] = temp[order + Width + 1] = temp[order + 2 + Width] = 0;
				break;
			}
			*/
			if (Score(temp, Width, Height) > max_score)
			{
				max_score = Score(temp, Width, Height);
				for (int i = 0; i < Width*Height; i++) best[i] = temp[i];
			}
			temp[order + 1] = temp[order + Width] = temp[order + Width + 1] = temp[order + 2 + Width] = 0;
		}
	}
	delete[]temp;
}

