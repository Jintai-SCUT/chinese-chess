#pragma once
#include<string>
#include<iostream>
using namespace std;
#include"Position.h"

//棋子每个作为独立个体,只完成棋子基本功能
class ChessPiece
{
public:
	int belongs;//  归属，-1或1，不可改变
	int if_death;//是否存活，0或者死亡回合数x
	string name;//名字
	Position<int> pos;//坐标

	ChessPiece(int, string, const Position<int>& pos1);//赋值的构造函数，创建棋盘时候棋盘完成
	virtual bool rules(const Position<int>& pos1, int board[10][9], int swichoff = 1)const = 0;
	/*移动是否合法？先判断是否在范围内，然后判断移动方式，最后通过棋盘找路径上有没有遮挡，以及目标位置是否有自己方的棋子*/

	virtual void movement(const Position<int>& pos1, int board[10][9]) = 0;//移动函数
	virtual ~ChessPiece() {};//必须用虚函数
};
//==============车===============
class Rook :public ChessPiece
{
public:
	Rook(int, string, const Position<int>& pos1, int board[10][9]);
	bool rules(const Position<int>& pos1, int board[10][9], int swichoff) const override;
	void movement(const Position<int>& pos1, int board[10][9])override;
};
//==============马===============
class Knight :public ChessPiece
{
public:
	Knight(int, string, const Position<int>& pos1, int board[10][9]);
	bool rules(const Position<int>& pos1, int board[10][9], int swichoff) const override;
	void movement(const Position<int>& pos1, int board[10][9])override;
};
//==============炮===============
class Cannon :public ChessPiece
{
public:
	Cannon(int, string, const Position<int>& pos1, int board[10][9]);
	bool rules(const Position<int>& pos1, int board[10][9], int swichoff) const override;
	void movement(const Position<int>& pos1, int board[10][9])override;
};
//==============象===============
class Bishop :public ChessPiece
{
public:
	Bishop(int, string, const Position<int>& pos1, int board[10][9]);
	bool Move(int m, int n, const Position<int>& pos1, int board[10][9], int swichoff) const;//与rules搭配用，因为红和黑的边界不同
	bool rules(const Position<int>& pos1, int board[10][9], int swichoff) const override;
	void movement(const Position<int>& pos1, int board[10][9])override;
};
//==============兵===============
class Pawn :public ChessPiece
{
public:
	Pawn(int, string, const Position<int>& pos1, int board[10][9]);
	bool rules(const Position<int>& pos1, int board[10][9], int swichoff) const override;
	void movement(const Position<int>& pos1, int board[10][9])override;
};
//==============士===============
class Advisor :public ChessPiece
{
public:
	Advisor(int, string, const Position<int>& pos1, int board[10][9]);
	bool rules(const Position<int>& pos1, int board[10][9], int swichoff) const override;
	void movement(const Position<int>& pos1, int board[10][9])override;
};
//==============将===============
class King :public ChessPiece
{
public:
	King(int, string, const Position<int>& pos1, int board[10][9]);
	bool rules(const Position<int>& pos1, int board[10][9], int swichoff) const override;
	void movement(const Position<int>& pos1, int board[10][9])override;
	bool iskingexpose(Position<int>& pos1, int board[10][9]);
};
