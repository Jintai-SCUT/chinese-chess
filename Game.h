#pragma once
#include"Board.h"
#include "Position.h"
#include<iostream>
#include<windows.h>
#include<string>
#include <vector>
#include <sstream>
#include <iomanip>
#include<fstream>
#include <chrono>
#include <thread>
#include <atomic>
#include <algorithm>
class Game
{
private:
	Position<int> consoleToBoard(int consoleX, int consoleY) const;
	HANDLE hinput;
	HANDLE houtput;

	// 选手名称
	string player1;
	string player2;

	// 颜色
	int redColor = 12;
	int blackColor = 1;
	int bgColor = 7;

	// 倒计时
	atomic<bool> timerRunning;
	atomic<int> remainingSeconds;
	thread timerThread;
	int timeLimit = 30;
	void timerLoop();
	void updateTimerDisplay();

	// 排行榜记录
	void recordResult(bool redWin);
public:
	Game();
	~Game();
	int home();//没进游戏的函数的所有功能
	void home_print(int highlight_home = 0);
	void home_ranking();//打印排行榜
	int  ingame();//进了游戏的所有功能
	void home_settings();
	void printIngameMenu(int PIMhighlight = 19);
	void iofiles(int iomode, Board* board = nullptr);//存入文件与打开文件,1是输出文件out，2是输入流in
	void iofile_read(vector<int>* lines, Board* boards);


	Position<int> getpos();//鼠标输入转化为坐标
	void setmouseinput();//转化输入模式
	void gotoxy(int x, int y);
	void setPlayerNames(const string& p1, const string& p2);
	void setTimeLimit(int seconds);
	void setColors(int red, int black, int bg = 7);
};
//棋盘的round和棋子的ifdeath的回合都是指发生在第几第几回的事，用的时候很容易就串了，board和piece容易不统一也是因为这个，在第几回合结束不是执行了几回合，会比执行的多一回合！！！





