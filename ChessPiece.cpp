#include"ChessPiece.h"

ChessPiece::ChessPiece(int belong1, string name1, const Position<int>& pos1) :belongs(belong1), name(name1), pos(pos1)
{
	if_death = 0;
}//初始化存活局数

//注释：写的所有rules和move函数传的参数都是变量，不是目的地，要翻译来用.而control函数哪里我传的是始终，要注意，

//车Rook
Rook::Rook(int belong1, string name1, const Position<int>& pos1, int board[10][9]) :ChessPiece(belong1, name1, pos1)
{
	board[pos1.x][pos1.y] = 1 * belongs;
}//初始化归属方，名字，坐标。上方是-1，下方是1.名字上是小写，下是大写。

bool Rook::rules(const Position<int>& pos1, int board[10][9], int swichoff = 1) const//数组用来得到状态
{
	if (pos.x + pos1.x < 0 || pos.x + pos1.x > 9 || pos.y + pos1.y < 0 || pos.y + pos1.y> 8)
	{
		if (swichoff == 1)
		{
			cout << "越界了" << endl;
		}
		return false;
	}
	//在范围里面
	if (board[pos.x][pos.y] * board[pos.x + pos1.x][pos.y + pos1.y] > 0) {
		if (swichoff == 1) { cout << "己方棋子在目标点位" << endl; }
		return false;
	}
	//确保棋子能下那里}
	if (pos1.x * pos1.y == 0 && pos1.x + pos1.y != 0)//判断只有x或者y移动
	{
		if (pos1.x == 0)//横向移动的话
		{
			int step = (pos1.y > 0) ? 1 : -1;
			for (int i = step; i != pos1.y; i += step)
			{
				if (board[pos.x][pos.y + i] != 0)
				{
					if (swichoff == 1)
					{
						cout << "有阻挡" << endl;
					}
					return false;
				}

			}
			return true;
		}
		else if (pos1.y == 0)
		{
			int step = (pos1.x > 0) ? 1 : -1;
			for (int i = step; i != pos1.x; i += step)
			{
				if (board[pos.x + i][pos.y] != 0)
				{
					if (swichoff == 1)
					{
						cout << "有阻挡" << endl;
					}
					return false;
				}

			}
			return true;
		}
		return false;
	}
	else
	{
		if (swichoff == 1)
		{
			cout << "你违反了规则" << endl;
		}
		return false;

	}
}

void Rook::movement(const Position<int>& pos1, int board[10][9])
{
	board[pos.x][pos.y] = 0;
	pos.x += pos1.x;
	pos.y += pos1.y;
	board[pos.x][pos.y] = 1 * belongs;
}



//马
Knight::Knight(int belong1, string name1, const Position<int>& pos1, int board[10][9]) :ChessPiece(belong1, name1, pos1)
{
	board[pos1.x][pos1.y] = 2 * belongs;
}

bool Knight::rules(const Position<int>& pos1, int board[10][9], int swichoff = 1) const//数组用来得到状态
{
	if (pos1.x + pos.x < 0 || pos1.x + pos.x > 9 || pos1.y + pos.y < 0 || pos1.y + pos.y > 8)
	{
		if (swichoff == 1)
		{
			cout << "越界了" << endl;
		} return false;
	}
	//在范围里面
	if (board[pos.x][pos.y] * board[pos.x + pos1.x][pos.y + pos1.y] > 0)
	{
		if (swichoff == 1)
		{
			cout << "己方棋子在目标点位" << endl;
		}
		return false;
	}//确保棋子能下那里
	int absx = (pos1.x > 0) ? pos1.x : -pos1.x;
	int absy = (pos1.y > 0) ? pos1.y : -pos1.y;
	//
	if (absx == 1 && absy == 2)
	{
		if (board[pos.x][pos.y + (pos1.y / 2)] != 0)
		{
			if (swichoff == 1)
			{
				cout << "蹩马脚" << endl;
			}
			return false;
		}
	}
	else if (absx == 2 && absy == 1)
	{
		if (board[pos.x + (pos1.x / 2)][pos.y] != 0)
		{
			if (swichoff == 1)
			{
				cout << "蹩马脚" << endl;
			}
			return false;
		}
	}
	else
	{
		if (swichoff == 1) {
			cout << "移动不合规" << endl;
		}
		return false;
	}
	return true;
}

void Knight::movement(const Position<int>& pos1, int board[10][9])
{
	board[pos.x][pos.y] = 0;
	pos.x += pos1.x;
	pos.y += pos1.y;
	board[pos.x][pos.y] = 2 * belongs;
}




//炮
Cannon::Cannon(int belong1, string name1, const Position<int>& pos1, int board[10][9]) : ChessPiece(belong1, name1, pos1)
{
	board[pos1.x][pos1.y] = 3 * belongs;
}
//累加来表示有一个
bool Cannon::rules(const Position<int>& pos1, int board[10][9], int swichoff = 1) const//数组用来得到状态
{
	if (pos1.x + pos.x < 0 || pos1.x + pos.x > 9 || pos1.y + pos.y < 0 || pos1.y + pos.y > 8)
	{
		if (swichoff == 1)
		{
			cout << "越界了" << endl;
		}
		return false;
	}
	//在范围里面
	if (board[pos.x][pos.y] * board[pos.x + pos1.x][pos.y + pos1.y] > 0)
	{
		if (swichoff == 1)
		{
			cout << "目标点位有己方棋子" << endl;
		}
		return false;
	}//确保棋子能下那里
	if (board[pos.x][pos.y] * board[pos.x + pos1.x][pos.y + pos1.y] == 0)//不吃子移动
	{
		if (pos1.x * pos1.y == 0 && pos1.x + pos1.y != 0)//判断只有x或者y移动
		{
			if (pos1.x == 0)//横向移动的话
			{
				int step = (pos1.y > 0) ? 1 : -1;
				for (int i = step; i != pos1.y; i += step)
				{
					if (board[pos.x][pos.y + i] != 0)
					{
						if (swichoff == 1)
						{
							cout << "有阻挡" << endl;
						}
						return false;
					}

				}
				return true;
			}
			else if (pos1.y == 0)
			{
				int step = (pos1.x > 0) ? 1 : -1;
				for (int i = step; i != pos1.x; i += step)
				{
					if (board[pos.x + i][pos.y] != 0)
					{
						if (swichoff == 1)
						{
							cout << "有阻挡" << endl;
						}
						return false;
					}

				}
				return true;
			}
		}
	}
	if (pos1.x == 0 && pos1.y != 0)//吃子移动
	{
		int step = (pos1.y > 0) ? 1 : -1;
		int record = 0;
		for (int i = step; i != pos1.y; i += step)
		{
			if (board[pos.x][pos.y + i] != 0)
			{
				record += 1;
			}
		}
		if (record == 1)
		{
			return true;
		}
		else
		{
			if (swichoff == 1)
			{
				cout << "路径上棋子数不为1" << endl;
			}
			return false;
		}
	}
	else if (pos1.x != 0 && pos1.y == 0)
	{
		int step = (pos1.x > 0) ? 1 : -1;
		int record = 0;
		for (int i = step; i != pos1.x; i += step)
		{
			if (board[pos.x + i][pos.y] != 0)
			{
				record += 1;
			}
		}
		if (record == 1)
		{
			return true;
		}
		else
		{
			if (swichoff == 1)
			{
				cout << "路径上棋子数不为1" << endl;
			}
			return false;
		}
	}
	else
	{
		if (swichoff == 1)
		{
			cout << "移动不合规" << endl;
		} return false;
	}
	return true;
}

void Cannon::movement(const Position<int>& pos1, int board[10][9])
{
	board[pos.x][pos.y] = 0;
	pos.x += pos1.x;
	pos.y += pos1.y;
	board[pos.x][pos.y] = 3 * belongs;
}




//象
Bishop::Bishop(int belong1, string name1, const Position<int>& pos1, int board[10][9]) :ChessPiece(belong1, name1, pos1)
{
	board[pos1.x][pos1.y] = 4 * belongs;
}

bool Bishop::Move(int m, int n, const Position<int>& pos1, int board[10][9], int swichoff = 1)const//y是固定0到8的，m和n是x的范围
{
	if (pos1.x + pos.x < m || pos1.x + pos.x > n || pos1.y + pos.y < 0 || pos1.y + pos.y > 8)
	{
		if (swichoff == 1)
		{
			cout << "越界了" << endl;
		}
		return false;
	}
	//在范围里面
	if (board[pos.x][pos.y] * board[pos.x + pos1.x][pos.y + pos1.y] > 0) {
		if (swichoff == 1)
		{
			cout << "目标点位有己方棋子" << endl;
		}
		return false;
	}
	//确保棋子能下那里
	int absx = (pos1.x > 0) ? pos1.x : -pos1.x;
	int absy = (pos1.y > 0) ? pos1.y : -pos1.y;
	if (absx == 2 && absy == 2)
	{
		if (board[pos.x + (pos1.x / 2)][pos.y + (pos1.y / 2)] != 0)
		{
			if (swichoff == 1)
			{
				cout << "蹩象脚" << endl;
			}return false;
		}
	}
	else
	{
		if (swichoff == 1)
		{
			cout << "移动不合规" << endl;
		} return false;
	}
	return true;
}

bool Bishop::rules(const Position<int>& pos1, int board[10][9], int swichoff = 1)const
{
	if (belongs == -1)//上方棋子，从5到9;
	{
		return Move(5, 9, pos1, board, swichoff);
	}
	else if (belongs == 1)//下方棋子，从0到4;
	{
		return Move(0, 4, pos1, board, swichoff);
	}
}
void Bishop::movement(const Position<int>& pos1, int board[10][9])
{
	board[pos.x][pos.y] = 0;
	pos.x += pos1.x;
	pos.y += pos1.y;
	board[pos.x][pos.y] = 4 * belongs;
}


//兵
Pawn::Pawn(int belong1, string name1, const Position<int>& pos1, int board[10][9]) :ChessPiece(belong1, name1, pos1)
{
	board[pos1.x][pos1.y] = 5 * belongs;
}

bool Pawn::rules(const Position<int>& pos1, int board[10][9], int swichoff = 1)const//判断他的移动范围，以及判断他是否过河了
{
	//1、边界检查 2、目的地是否被堵住 3、是否过河 4、走法是否合规
	if (pos1.x + pos.x < 0 || pos1.x + pos.x> 9 || pos1.y + pos.y < 0 || pos1.y + pos.y > 8)
	{
		if (swichoff == 1)
		{
			cout << "越界了" << endl;
		}
		return false;
	}
	//在范围里面
	if (board[pos.x][pos.y] * board[pos.x + pos1.x][pos.y + pos1.y] > 0) {
		if (swichoff == 1)
		{
			cout << "目标点位有己方棋子" << endl;
		}
		return false;
	}
	//确保棋子能下那里
	//belongs=1是下方，belongs=-1是上方。
	if (belongs == 1 && pos.x >= 5 || belongs == -1 && pos.x <= 4)//过河了
	{
		//规定方向
		if (pos1.x == belongs && pos1.y == 0 || pos1.x == 0 && pos1.y == 1 || pos1.x == 0 && pos1.y == -1)
		{
			return true;
		}
		else
		{
			if (swichoff == 1)
			{
				cout << "走法不合规" << endl;
			}
			return false;
		}
	}
	else
	{
		if (pos1.x == belongs && pos1.y == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

void Pawn::movement(const Position<int>& pos1, int board[10][9])
{
	board[pos.x][pos.y] = 0;
	pos.x += pos1.x;
	pos.y += pos1.y;
	board[pos.x][pos.y] = 5 * belongs;
}


//士
Advisor::Advisor(int belong1, string name1, const Position<int>& pos1, int board[10][9]) :ChessPiece(belong1, name1, pos1)
{
	board[pos1.x][pos1.y] = 6 * belongs;
}
bool Advisor::rules(const Position<int>& pos1, int board[10][9], int swichoff = 1)const//判断他的移动范围，以及判断他是否过河了
{
	//先确保能下那里，再通过belongs得到边界并且判断边界，再判断走法
	if (pos1.x + pos.x < 0 || pos1.x + pos.x> 9 || pos1.y + pos.y < 0 || pos1.y + pos.y > 8)
	{
		if (swichoff == 1)
		{
			cout << "越界了" << endl;
		}
		return false;
	}//在范围里面
	if (board[pos.x][pos.y] * board[pos.x + pos1.x][pos.y + pos1.y] > 0)
	{
		if (swichoff == 1)
		{
			cout << "目标点位有己方棋子" << endl;
		}
		return false;
	}//确保棋子能下那里
	//belongs=1是下方，belongs=-1是上方。
	int x2 = pos.x + pos1.x;
	int y2 = pos.y + pos1.y;
	if (belongs == 1 && x2 >= 0 && x2 <= 2 && y2 >= 3 && y2 <= 5)
	{
		if (pos1.x * pos1.y == 1 || pos1.x * pos1.y == -1)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else if (belongs == -1 && x2 >= 7 && x2 <= 9 && y2 >= 3 && y2 <= 5)
	{
		if (pos1.x * pos1.y == 1 || pos1.x * pos1.y == -1)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		if (swichoff == 1)
		{
			cout << "越界了" << endl;
		}
		return false;
	}
}

void Advisor::movement(const Position<int>& pos1, int board[10][9])
{
	board[pos.x][pos.y] = 0;
	pos.x += pos1.x;
	pos.y += pos1.y;
	board[pos.x][pos.y] = 6 * belongs;
}



//将军
King::King(int belong1, string name1, const Position<int>& pos1, int board[10][9]) :ChessPiece(belong1, name1, pos1)
{
	board[pos1.x][pos1.y] = 7 * belongs;
}
bool King::rules(const Position<int>& pos1, int board[10][9], int swichoff = 1)const//判断他的移动范围，以及判断他是否过河了
{
	//先确保能下那里，再通过belongs得到边界并且判断边界，再判断走法
	if (pos1.x + pos.x < 0 || pos1.x + pos.x> 9 || pos1.y + pos.y < 0 || pos1.y + pos.y > 8)
	{
		if (swichoff == 1)
		{
			cout << "越界了" << endl;
		}
		return false;
	}//在范围里面
	if (board[pos.x][pos.y] * board[pos.x + pos1.x][pos.y + pos1.y] > 0)
	{
		if (swichoff == 1)
		{
			cout << "目标点位有己方棋子" << endl;
		}
		return false;
	}//确保棋子能下那里
	//belongs=1是下方，belongs=-1是上方。
	int x2 = pos.x + pos1.x;
	int y2 = pos.y + pos1.y;
	if (belongs == 1 && x2 >= 0 && x2 <= 2 && y2 >= 3 && y2 <= 5)
	{
		if (pos1.x * pos1.y == 0 && (pos1.x + pos1.y == -1 || pos1.x + pos1.y == 1))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else if (belongs == -1 && x2 >= 7 && x2 <= 9 && y2 >= 3 && y2 <= 5)
	{
		if (pos1.x * pos1.y == 0 && (pos1.x + pos1.y == -1 || pos1.x + pos1.y == 1))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		if (swichoff == 1)
		{
			cout << "越界了" << endl;
		}
		return false;
	}
}

void King::movement(const Position<int>& pos1, int board[10][9])
{
	board[pos.x][pos.y] = 0;
	pos.x += pos1.x;
	pos.y += pos1.y;
	board[pos.x][pos.y] = 7 * belongs;
}

bool King::iskingexpose(Position<int>& pos1, int board[10][9])
{
	for (int i = pos.x + belongs; i != pos1.x; i += belongs)
	{
		if (board[i][pos1.y] != 0)
		{
			return false;
		}
		if (i== pos1.x-belongs&&board[pos1.x][pos.y]+ board[pos.x][pos.y]==0)
		{
			return true;
		}
	}

	return false;

}