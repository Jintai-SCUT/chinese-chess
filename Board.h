#pragma once
#include"ChessPiece.h"
#include <vector>
#include<windows.h>
using namespace std;

class Board
{
public:
	int redColor, blackColor, bgColor;


	static constexpr int table[32] = { 1,1,2,2,3,3,4,4,5,5,5,5,5,6,6,7,-1,-1,-2,-2,-3,-3,-4,-4,-5,-5,-5,-5,-5,-6,-6,-7 };


	unsigned int round;//记录第几回合，奇数与偶数控制移动方，悔棋时减1.
	int board[10][9] = { 0 };
	//0是没有棋子，车1，马2，炮3，象4，兵5，士6，将7。belongs决定正负，正在下方。
	vector<int>record_step;
	//因为所有的棋子都没有额外拓展其他函数或者对象，所以直接使用基类指针数组其实可以实现访问所有需要访问的函数与对象。
	ChessPiece* pieces[32];
	King* king_pieces;


	Board(int red = 12, int black = 1, int bg = 7);//初始化指针的每一个数据以及棋盘
	void controls(const Position<int>& pos1, const Position<int>& pos2);//移动
	void regret();//悔棋
	void print(const Position<int>* highlight = nullptr) const;
	bool judge(int round1, int shifouxianshi = 1);//判断是否将军
	void record_games(string&);
	void setcolor(int)const;
	bool stalemate();
	void read_games(unsigned int towhichstep, vector<int>* record_round);//查看保存的对局
	HANDLE g_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	~Board();
};

