// tetrisDlg.cpp: 实现文件
#include "pch.h"
#include "framework.h"
#include "tetris.h"
#include "tetrisDlg.h"
#include "afxdialogex.h"
#include<vector>
#include <algorithm>
#include<map>
#include<sstream>
#include "Game.h"
#include <windows.h> 
#include<iostream>
#include<fstream>
using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////////////
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};
CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}
//消息响应列表
BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// CtetrisDlg 对话框

CtetrisDlg::CtetrisDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TETRIS_DIALOG, pParent), score_1(0), score_2(0),  m_playerName(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}
CtetrisDlg::~CtetrisDlg()
{
	delete game1;
	delete game2;
}
void CtetrisDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_LBIndex(pDX, IDC_LIST_SCORE, score_1);
	DDX_Control(pDX, IDC_LIST_SCORE, m_list_score_1);
	DDX_Control(pDX, IDC_HIGHSCORE, m_list_highscore);

	//  DDX_Control(pDX, IDC_LIST_SCORE1, m_list_score1);
	DDX_Control(pDX, IDC_LIST_SCORE1, m_list_score_1);
	DDX_Control(pDX, IDC_LIST_SCORE2, m_list_score_2);
	DDX_Text(pDX, IDC_PLAYER_NAME, m_playerName);
	DDX_Control(pDX, IDC_HIGHSCORE_LIST, m_highScoreList);
}


//消息响应列表
BEGIN_MESSAGE_MAP(CtetrisDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START, &CtetrisDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_HALT, &CtetrisDlg::OnBnClickedButtonHalt)
	ON_WM_TIMER()
	ON_WM_KEYDOWN()
	ON_BN_CLICKED(IDC_MUSIC, &CtetrisDlg::OnBnClickedMusic)

	ON_BN_CLICKED(IDC_MODE_SELECT, &CtetrisDlg::OnBnClickedModeSelect)
	ON_BN_CLICKED(IDC_BUTTON1, &CtetrisDlg::OnBnClickedButton1)
	 
END_MESSAGE_MAP()


// CtetrisDlg 消息处理程序

//界面初始化
BOOL CtetrisDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	Rank();
	GetDlgItem(IDC_STATIC_MODE)->SetWindowText(_T("无尽模式"));
	GetDlgItem(IDC_STATIC_TIME)->SetWindowText(_T("-1"));
	UpdateHighScore();
	UpdateScore_1();
	UpdateScore_2();
	SetWindowText(_T("TETRIS"));
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}
void CtetrisDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CtetrisDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
		DrawNet();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CtetrisDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//设置游戏状态
void CtetrisDlg::OnBnClickedButtonStart()
{
	// TODO: 在此添加控件通知处理程序代码
	//UpdateStaticMODE();
	game1->Start();
	if (MODE == 1)Rank();
	if (MODE == 2)
	{
		game2->Start();
	}

	ResetScore();
	PlaySound(_T("D:\\Project\\C++\\C++作业\\MFC\\tetris\\tetris\\res\\begin.wav"), NULL, SND_FILENAME | SND_NOSTOP);
	if (musicstate == 0)OnBnClickedMusic();

	SetTimer(1, TIME_STEP, NULL);
}
void CtetrisDlg::OnBnClickedButtonHalt()
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedMusic();//修改
	game1->HaltOrContinue();
	if (MODE == 2)
	{
		game2->HaltOrContinue();
	}

	if (game1->GetState() == HALT) KillTimer(1);
	if (game1->GetState() == GO)SetTimer(1, TIME_STEP, NULL);
}

//播放音乐
void CtetrisDlg::OnBnClickedMusic()
{
	// TODO: 在此添加控件通知处理程序代码
	if (musicstate == 0) {
		PlaySound(_T("D:\\Project\\C++\\C++作业\\MFC\\tetris\\tetris\\res\\startmusic.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		musicstate = 1;
	}
	else {
		PlaySound(NULL, NULL, SND_FILENAME | SND_ASYNC);
		musicstate = 0;
	}
}

//切换游戏模式
void CtetrisDlg::OnBnClickedModeSelect()
{
	// TODO: 在此添加控件通知处理程序代码
	//模式代码1，2，3
	MODE += 1;
	if (MODE > 3)MODE = 1;
	UpdateStaticMODE();

	OnBnClickedButtonStart();
}
//绘制显示
void CtetrisDlg::DrawNet()
{
	DrawSmallNet();
	DrawBigNet();
	UpdateScore();
}
void CtetrisDlg::DrawBigNet()
{
	CRect rect;
	CWnd *wnd = GetDlgItem(IDC_PIC_BIG);
	CPaintDC dc(wnd);

	CBrush brush1(RGB(255, 255, 0)); // 黄色  
	CBrush* pOldBrush1 = dc.SelectObject(&brush1); // 选择画刷
	wnd->GetClientRect(&rect);
	if (game1->GetPic_BigNet())
	{
		for (int i = 0; i < game1->NET_HEIGHT; i++)
		{
			for (int j = 0; j < game1->NET_WIDTH; j++)
				if (game1->GetPic_BigNet()[i*(game1->NET_WIDTH) + j] == 1)
				{
					dc.Rectangle(
						j*rect.Width() / game1->NET_WIDTH,
						i*rect.Height() / game1->NET_HEIGHT,
						(j + 1)*rect.Width() / game1->NET_WIDTH,
						(i + 1)*rect.Height() / game1->NET_HEIGHT);
				}
		}
	}

	//DrawPlace();

	if (MODE == 2)
	{
		CRect rect;
		CWnd *wnd = GetDlgItem(IDC_PIC_BIG2);
		CPaintDC dc(wnd);

		CBrush brush1(RGB(255, 255, 0)); // 黄色  
		CBrush* pOldBrush1 = dc.SelectObject(&brush1); // 选择画刷
		wnd->GetClientRect(&rect);
		if (game2->GetPic_BigNet())
		{
			for (int i = 0; i < game2->NET_HEIGHT; i++)
			{
				for (int j = 0; j < game2->NET_WIDTH; j++)
					if (game2->GetPic_BigNet()[i*(game1->NET_WIDTH) + j] == 1)
					{
						dc.Rectangle(
							j*rect.Width() / game2->NET_WIDTH,
							i*rect.Height() / game2->NET_HEIGHT,
							(j + 1)*rect.Width() / game2->NET_WIDTH,
							(i + 1)*rect.Height() / game2->NET_HEIGHT);
					}
			}
		}

	}

	dc.SelectObject(pOldBrush1); // 恢复原来的画笔
	//修改
	//可增加随机颜色功能
	wnd->RedrawWindow();
	
}
void CtetrisDlg::DrawPlace()
{
	CRect rect;
	CWnd *wnd = GetDlgItem(IDC_PIC_BIG);
	CPaintDC dc(wnd);

	CBrush brush2(RGB(0, 0, 255)); // 蓝色
	CBrush* pOldBrush2 = dc.SelectObject(&brush2); // 选择画刷
	wnd->GetClientRect(&rect);

	//game1->place();
	for (int i = 0; i < game1->NET_HEIGHT; i++)
	{
		for (int j = 0; j < game1->NET_WIDTH; j++)
		{
			if (game1->best[i*(game1->NET_WIDTH) + j] == 1 && game1->GetBigNet()[i*(game1->NET_WIDTH) + j] == 0)
			{
				dc.Rectangle(
					j*rect.Width() / game1->NET_WIDTH,
					i*rect.Height() / game1->NET_HEIGHT,
					(j + 1)*rect.Width() / game1->NET_WIDTH,
					(i + 1)*rect.Height() / game1->NET_HEIGHT);
			}
		}
	}

	dc.SelectObject(pOldBrush2); // 恢复原来的画笔
}
void CtetrisDlg::DrawSmallNet()
{
	CRect rect;
	CWnd *wnd = GetDlgItem(IDC_PIC_SMALL);
	CPaintDC dc(wnd);
	CBrush brush2(RGB(255, 255, 0)); // 
	CBrush* pOldBrush2 = dc.SelectObject(&brush2); // 选择画刷
	//增加随机颜色功能
	wnd->GetClientRect(&rect);

	if (game1->GetSmallNet())
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
				if (game1->GetSmallNet()[i * 4 + j] == 1)
				{
					dc.Rectangle(
						j*rect.Width() / 4,
						i*rect.Height() / 4,
						(j + 1)*rect.Width() / 4,
						(i + 1)*rect.Height() / 4);
				}
		}
	}
	if (MODE == 2)
	{
		CRect rect;
		CWnd *wnd = GetDlgItem(IDC_PIC_SMALL2);
		CPaintDC dc(wnd);
		CBrush brush2(RGB(255, 255, 0)); // 
		CBrush* pOldBrush2 = dc.SelectObject(&brush2); // 选择画刷
		//增加随机颜色功能
		wnd->GetClientRect(&rect);

		if (game2->GetSmallNet())
		{
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
					if (game2->GetSmallNet()[i * 4 + j] == 1)
					{
						dc.Rectangle(
							j*rect.Width() / 4,
							i*rect.Height() / 4,
							(j + 1)*rect.Width() / 4,
							(i + 1)*rect.Height() / 4);
					}
			}
		}
	}
	wnd->RedrawWindow();
}

//计时器游戏进程
void CtetrisDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	switch (MODE)
	{
	case 1:PlayMode_1(); break;
	case 2:PlayMode_2(); break;
	case 3:

		MODE_3_Time -= TIME_STEP;
		PlayMode_3();
		break;
	}
	UpdateStaticTIME();
	
	Invalidate(true);// 重绘画面
	
	CDialogEx::OnTimer(nIDEvent);
}

//游戏模式
void  CtetrisDlg::UpdateStaticMODE()
{
	switch (MODE)
	{
	case 1:m_staticMode = "无尽模式"; break;
	case 2:m_staticMode = "双人模式"; break;
	case 3:m_staticMode = "限时模式"; break;
	}
	GetDlgItem(IDC_STATIC_MODE)->SetWindowText(m_staticMode);
}
void CtetrisDlg::UpdateStaticTIME()
{
	switch (MODE)
	{
	case 1:case 2:m_staticTime = "-1"; break;
	case 3:m_staticTime.Format(_T("%d"), (MODE_3_Time + 1000) / 1000); break;
	}
	GetDlgItem(IDC_STATIC_TIME)->SetWindowText(m_staticTime);
}
void CtetrisDlg::PlayMode_1()
{
	TCHAR *msg1 = _T("Over!One more try?");
	TCHAR *msg2 = _T("Goodbye.See you next time!");
	bool state1 = game1->Go();
	if (!state1)
	{
		if (game1->sta)UpdateScore_1();

		KillTimer(1);
		if (MessageBox(msg1, _T("游戏结束"), MB_YESNO | MB_ICONQUESTION) == IDYES)
		{
			// 重置游戏状态并开始新游戏
			game1->Start();
			Rank();
			ResetScore();
			SetTimer(1, TIME_STEP, NULL); // 以1秒的间隔重新启动定时器
		}
		else
		{
			// 可以选择处理退出或其他操作
			MessageBox(msg2);
		}
	}
}
void CtetrisDlg::PlayMode_2()
{
	TCHAR *msg1 = _T("Over!One more try?");
	TCHAR *msg2 = _T("Goodbye.See you next time!");
	CString msg3;
	msg3.Format(_T("Winner:1P! Score:%d Once again?"), game1->score);
	CString msg4;
	msg4.Format(_T("Winner:2P! Score:%d Once again?"), game2->score);
	CString msg5 = (_T("Draw game!Once again?"));

	bool state1 = game1->Go();
	bool state2 = game2->Go();

	if (game1->sta)
	{
		UpdateScore_1();
		game2->Input(VK_NUMPAD0);
		game2->next_Box= std::make_shared<Box>(rand() % 3 + 8);
	}
	if (game2->sta)
	{
		UpdateScore_2();
		game1->Input(71);
		game1->next_Box = std::make_shared<Box>(rand() % 3 + 8);
	}
	game1->sta = game2->sta = 0;
	if ((!state1)||(!state2))
	{
		KillTimer(1);
		if (state1)
		{
			msg5 = msg3;
		}
		else 
		{
			msg5 = msg4;
		}

		if (MessageBox(msg5, _T("游戏结束"), MB_YESNO | MB_ICONQUESTION) == IDYES)
		{
			// 重置游戏状态并开始新游戏					
			game1->Start();
			game2->Start();

			ResetScore();
			SetTimer(1, TIME_STEP, NULL); // 重新启动定时器
		}
		else
		{
			// 可以选择处理退出或其他操作
			MessageBox(msg2);
		}
	}
}
void CtetrisDlg::PlayMode_3()
{
	CString msg1;
	msg1.Format(_T("Game over! Score:%d Once again?"), game1->score);
	TCHAR *msg2 = _T("Goodbye.See you next time!");
	CString msg3;
	msg3.Format(_T("Time over! Score:%d Once again?"), game1->score);

	int lastscore = game1->score;
	bool state1 = game1->Go();
	MODE_3_Time += 0.1*(game1->score - lastscore) * 3000;

	if (!state1)
	{
		if (game1->sta)UpdateScore_1();

		KillTimer(1);
		if (MessageBox(msg1, _T("游戏结束"), MB_YESNO | MB_ICONQUESTION) == IDYES)
		{
			// 重置游戏状态并开始新游戏
			MODE_3_Time = 60000;
			game1->Start();
			ResetScore();
			SetTimer(1, TIME_STEP, NULL); // 以TIME_STEP（单位ms）为时间间隔重新启动定时器
		}
		else
		{
			// 可以选择处理退出或其他操作
			MessageBox(msg2);
		}
	}
	if (MODE_3_Time == 0)
	{
		if (game1->sta)UpdateScore_1();

		KillTimer(1);
		if (MessageBox(msg3, _T("游戏结束"), MB_YESNO | MB_ICONQUESTION) == IDYES)
		{
			// 重置游戏状态并开始新游戏
			MODE_3_Time = 60000;
			game1->Start();
			ResetScore();
			SetTimer(1, TIME_STEP, NULL); // 以TIME_STEP（单位ms）为时间间隔重新启动定时器
		}
		else
		{
			// 可以选择处理退出或其他操作
			MessageBox(msg2);
		}
	}
}

//键盘输入响应
BOOL CtetrisDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)
		OnKeyDown((UINT)pMsg->wParam, 1, 1);
	return CDialogEx::PreTranslateMessage(pMsg);
}
void CtetrisDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (MODE == 1)
	{
		game1->Input(nChar);
	}
	if (MODE == 2)
	{
		switch (nChar)
		{
		case 65:case 68:case 87:case 83:case 71:game1->Input(nChar); break;
		case 100:case 102:case 104:case 101:case 96:game2->Input(nChar); break;
		}
	}
	if (MODE == 3)
	{
		if (nChar == 83 || nChar == 101)MODE_3_Time += 1000;
		game1->Input(nChar);
	}
	Invalidate(true);// 重绘画面
	CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
}

//更新游戏分数
void CtetrisDlg::UpdateScore()
{
	if (game1->sta)UpdateScore_1();
	if (MODE == 2)
	{
		if (game2->sta)UpdateScore_2();
	}
	UpdateHighScore();
}
void CtetrisDlg::UpdateScore_1()
{

	CWnd *pWndScore = (CEdit*)GetDlgItem(IDC_LIST_SCORE1);
	CString str;
	str.Format(TEXT("%d 分"), game1->score);
	int count = m_list_score_1.GetCount();
	bool found = false;
	for (int i = 0; i < count; ++i)
	{
		CString itemText;
		m_list_score_1.GetText(i, itemText); // 获取列表中指定索引的项
		if (itemText == str)
		{
			found = true;
			break;
		}
	}
	// 如果分数不在列表中，则添加到列表
	if (!found)
	{
		int index = m_list_score_1.GetCount(); // 获取当前列表的长度
		m_list_score_1.InsertString(0, str); // 在列表开头插入新分数
		m_list_score_1.SetItemData(0, game1->score); // 设置新分数的ID
		m_list_score_1.SetTopIndex(0); // 将列表滚动到新插入的分数
	}
}
void CtetrisDlg::UpdateScore_2()
{
	CWnd *pWndScore = (CEdit*)GetDlgItem(IDC_LIST_SCORE2);
	CString str;
	str.Format(TEXT("%d 分"), game2->score);
	int count = m_list_score_2.GetCount();
	bool found = false;
	for (int i = 0; i < count; ++i)
	{
		CString itemText;
		m_list_score_2.GetText(i, itemText); // 获取列表中指定索引的项
		if (itemText == str)
		{
			found = true;
			break;
		}
	}
	// 如果分数不在列表中，则添加到列表
	if (!found)
	{
		int index = m_list_score_2.GetCount(); // 获取当前列表的长度
		m_list_score_2.InsertString(0, str); // 在列表开头插入新分数
		m_list_score_2.SetItemData(0, game1->score); // 设置新分数的ID
		m_list_score_2.SetTopIndex(0); // 将列表滚动到新插入的分数
	}
}
void CtetrisDlg::ResetScore()
{
	// 清空CListBox控件中的所有项目
	m_list_score_1.ResetContent();

	// 添加新的初始分数（例如0或默认值）
	game1->score = 0;
	CString str_1;
	str_1.Format(_T("%d 分"), game1->score);
	m_list_score_1.InsertString(0, str_1); // 在列表开头插入新的分数
	m_list_score_1.SetItemData(0, game1->score); // 设置新分数的数据
	m_list_score_1.SetTopIndex(0); // 将列表滚动到新插入的分数

	if (MODE == 2)
	{
		// 清空CListBox控件中的所有项目
		m_list_score_2.ResetContent();

		// 添加新的初始分数（例如0或默认值）
		game2->score = 0;
		CString str_2;
		str_2.Format(_T("%d 分"), game2->score);
		m_list_score_2.InsertString(0, str_2); // 在列表开头插入新的分数
		m_list_score_2.SetItemData(0, game2->score); // 设置新分数的数据
		m_list_score_2.SetTopIndex(0); // 将列表滚动到新插入的分数
	}
}

void CtetrisDlg::UpdateHighScore()
{
	if (MODE == 1 || MODE == 2)
	{
		CString playerName;
		GetDlgItemText(IDC_PLAYER_NAME, playerName);  // 从编辑框中获取玩家名称

		int score_now = (game1->score > game2->score ? game1->score : game2->score);

		PlayerScore currentScore;
		currentScore.playerName = playerName;
		currentScore.score = score_now;// 定义一个数组用于存放分数
		std::vector<PlayerScore> scores;

		fstream infile;
		infile.open("highscore.txt");
		if (!infile)
		{
			// 如果文件不存在，创建一个新文件
			fstream out;
			out.open("highscore.txt", ios::out | ios::trunc);
			out << score_now;
			out.close();
		}
		else {
			// 如果文件存在，读取文件中的最高分
			infile >> highscore;
			// 更新历史最高分
			if (score_now > highscore) {
				highscore = score_now;
				fstream out;
				out.open("highscore.txt", ios::out | ios::trunc);
				out << highscore;
				out.close();
			}
		}
		infile.close();
	}
	if (MODE == 3)
	{
		int score_now = (game1->score > game2->score ? game1->score : game2->score);
		fstream infile;
		infile.open("highscore1.txt");
		if (!infile)
		{
			// 如果文件不存在，创建一个新文件
			fstream out;
			out.open("highscore1.txt", ios::out | ios::trunc);
			out << score_now;
			out.close();
		}
		else {
			// 如果文件存在，读取文件中的最高分
			infile >> highscore;
			// 更新历史最高分
			if (score_now > highscore) {
				highscore = score_now;
				fstream out;
				out.open("highscore1.txt", ios::out | ios::trunc);
				out << highscore;
				out.close();
			}
		}
		infile.close();
	}
	CWnd *phighScore = GetDlgItem(IDC_HIGHSCORE);
	CString stri;
	stri.Format(L"%d 分", highscore);
	m_list_highscore.InsertString(0, stri);
}

void CtetrisDlg::Rank() {// 获取玩家名称
	CString playerName;
	GetDlgItemText(IDC_PLAYER_NAME, playerName);  // 从编辑框中获取玩家名称

	// 获取当前分数
	int score_now = (game1->score > game2->score ? game1->score : game2->score);

	// 创建 PlayerScore 结构保存玩家名称和分数
	PlayerScore currentScore;
	currentScore.playerName = playerName;
	currentScore.score = score_now;

	// 定义一个数组用于存放分数
	vector<PlayerScore> scores;

	// 打开文件并读取历史分数
	ifstream infile;
	infile.open("rankhighscore.txt");

	if (!infile) {
		// 文件不存在时，创建新文件并写入当前玩家分数
		std::ofstream out("rankhighscore.txt", std::ios::out | std::ios::trunc);
		if (out.is_open()) {
			out << std::string(CT2A(playerName)) << " " << score_now << std::endl;
			scores.push_back(currentScore);
		}
	}
	else {
		// 文件存在，读取文件内容
		string line;
		while (std::getline(infile, line)) {
			istringstream iss(line);
			string playerNameStd;
			int score;

			if (iss >> playerNameStd >> score) {
				PlayerScore ps;
				ps.playerName = CString(playerNameStd.c_str());  // 将 std::string 转换为 CString
				ps.score = score;
				scores.push_back(ps);
			}
		}

		infile.close();

		// 合并同一玩家的分数，保留最高分
		map<CString, int> playerScoreMap;
		for (const auto& s : scores) {
			auto it = playerScoreMap.find(s.playerName);
			if (it != playerScoreMap.end()) {
				if (s.score > it->second) {
					it->second = s.score;
				}
			}
			else {
				playerScoreMap[s.playerName] = s.score;
			}
		}

		// 清空 scores 向量并重新填充
		scores.clear();
		for (const auto& entry : playerScoreMap) {
			PlayerScore ps;
			ps.playerName = entry.first;
			ps.score = entry.second;
			scores.push_back(ps);
		}

		// 添加当前分数并排序
		bool playerExists = false;
		for (auto& s : scores) {
			if (s.playerName == currentScore.playerName) {
				if (s.score < currentScore.score) {
					s.score = currentScore.score;  // 更新为更高的分数
				}
				playerExists = true;
				break;
			}
		}

		if (!playerExists) {
			scores.push_back(currentScore);
		}

		//排序
		for (int i = 0; i < scores.size(); i++)
		{
			for (int j = scores.size()-1; j > i; j--)
			{
				if (scores[j].score > scores[j - 1].score) 
				{
					std::swap(scores[j], scores[j - 1]);
				}
			}
		}
		// 保留前五名
		if (scores.size() > 5) {
			scores.resize(5);
		}
		
		// 将前五名分数写回文件
		ofstream out("rankhighscore.txt", std::ios::out | std::ios::trunc);
		if (out.is_open()) {
			for (const auto& s : scores) {
				out << std::string(CT2A(s.playerName)) << " " << s.score << std::endl;
			}
			out.close();
		}
	}
	// 清空 List Box
	m_highScoreList.ResetContent();

	
	// 将前五名玩家的名称和分数格式化并显示在 List Box 中
	for (int i = 0; i < scores.size(); ++i) {
		CString formattedScore;
		formattedScore.Format(_T("%d. %s: %d"), i + 1, scores[i].playerName, scores[i].score);
		m_highScoreList.InsertString(i, formattedScore);
	}
}

void CtetrisDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CAboutDlg dlg;
	dlg.DoModal();
}
