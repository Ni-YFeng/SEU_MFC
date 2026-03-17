#pragma once
#include"Game.h"
#include<mmsystem.h>
#pragma comment(lib,"winmm.lib")

// 构造
// CtetrisDlg 对话框
class CtetrisDlg : public CDialogEx
{
private:
	Game*game1 = new Game(17, 9); //游戏变量game1，单人模式和限时模式下使用，双人模式下1P使用
	Game*game2 = new Game(17, 9); //双人模式下2P使用
	int MODE=1;                   //游戏模式参数，单人模式为1，双人模式为2，限时模式为3
public:
	CtetrisDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CtetrisDlg();                      //对话框析构函数
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TETRIS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	HICON m_hIcon;                                     //程序图标

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();                       //对话框初始化
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);//
	afx_msg void OnPaint();                            //绘制界面
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public: 
	afx_msg void OnBnClickedButtonStart();             //开始/重新开始按钮响应
	afx_msg void OnBnClickedButtonHalt();              //暂停/继续按钮响应
	afx_msg void OnBnClickedModeSelect();              //模式选择按钮响应
	afx_msg void OnBnClickedMusic();                   //音乐播放按钮响应
	afx_msg void OnTimer(UINT_PTR nIDEvent);           //计时器函数
	virtual BOOL PreTranslateMessage(MSG* pMsg); 
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);//键盘输入响应
	
public:
	void DrawNet();                                    //绘制网格
	void DrawBigNet();                                 //绘制游戏状态网格
	void DrawSmallNet();                               //绘制下一块网格
	void DrawPlace();
	
public:
	CListBox m_list_score_1;                           //存储1P分数
	int score_1;                                       //1P分数 
	CListBox m_list_score_2;                           //存储2P分数 
	int score_2;                                       //2P分数
	CListBox m_list_highscore;                         //存储历史最高分数
	int highscore = 0;                                 //历史最高分数

	void UpdateScore();                                //更新分数
	void ResetScore();                                 //清空分数
	void UpdateScore_1();                              //更新1P分数
	void UpdateScore_2();                              //更新2P分数
	void UpdateHighScore();                            //更新历史最高分
	
	int musicstate = 0;                                //音乐播放状态
	static const int TIME_STEP = 500;                 //每步时长
	
	CString m_staticMode;                              //存储游戏模式
	void UpdateStaticMODE();                           //更新游戏模式
	void UpdateStaticTIME();                           //更新剩余时间

	void PlayMode_1();                                 //执行单人模式
	void PlayMode_2();                                 //执行双人模式
	void PlayMode_3();                                 //执行限时模式

	CString m_staticTime;                              //剩余时间
	int MODE_3_Time = 60000;                           //限时模式规定时长-60s

	void Rank();
	CString m_playerName;
	CListBox m_highScoreList;
	struct PlayerScore {
		CString playerName;
		int score;
	};

	// 排序用的比较函数
	bool CompareScores(const PlayerScore& a, const PlayerScore& b) {
		return a.score > b.score;  // 从大到小排序
	}
	
	afx_msg void OnBnClickedButton1();
};
 
