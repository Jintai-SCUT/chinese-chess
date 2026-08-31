#include"Board.h"
#include<fstream>
#include <ctime>
//参数有round和board和belongs和if_death和name和x和y和piece指针
Board::Board(int red, int black, int bg)
	: round(1), redColor(red), blackColor(black), bgColor(bg)
{
	//初始化步数大小，可以150步，超了再扩容
	record_step.reserve(400);
	//下方棋子，红
	pieces[0] = new Rook(1, "r", Position<int>(0, 0), board);
	pieces[1] = new Rook(1, "r", Position<int>(0, 8), board);
	pieces[2] = new Knight(1, "k", Position<int>(0, 1), board);
	pieces[3] = new Knight(1, "k", Position<int>(0, 7), board);
	pieces[4] = new Cannon(1, "c", Position<int>(2, 1), board);
	pieces[5] = new Cannon(1, "c", Position<int>(2, 7), board);
	pieces[6] = new Bishop(1, "b", Position<int>(0, 2), board);
	pieces[7] = new Bishop(1, "b", Position<int>(0, 6), board);
	pieces[8] = new Pawn(1, "p", Position<int>(3, 0), board);
	pieces[9] = new Pawn(1, "p", Position<int>(3, 2), board);
	pieces[10] = new Pawn(1, "p", Position<int>(3, 4), board);
	pieces[11] = new Pawn(1, "p", Position<int>(3, 6), board);
	pieces[12] = new Pawn(1, "p", Position<int>(3, 8), board);
	pieces[13] = new Advisor(1, "a", Position<int>(0, 3), board);
	pieces[14] = new Advisor(1, "a", Position<int>(0, 5), board);
	pieces[15] = new King(1, "g", Position<int>(0, 4), board);
	//上方棋子，黑
	pieces[16] = new Rook(-1, "R", Position<int>(9, 0), board);
	pieces[17] = new Rook(-1, "R", Position<int>(9, 8), board);
	pieces[18] = new Knight(-1, "K", Position<int>(9, 1), board);
	pieces[19] = new Knight(-1, "K", Position<int>(9, 7), board);
	pieces[20] = new Cannon(-1, "C", Position<int>(7, 1), board);
	pieces[21] = new Cannon(-1, "C", Position<int>(7, 7), board);
	pieces[22] = new Bishop(-1, "B", Position<int>(9, 2), board);
	pieces[23] = new Bishop(-1, "B", Position<int>(9, 6), board);
	pieces[24] = new Pawn(-1, "P", Position<int>(6, 0), board);
	pieces[25] = new Pawn(-1, "P", Position<int>(6, 2), board);
	pieces[26] = new Pawn(-1, "P", Position<int>(6, 4), board);
	pieces[27] = new Pawn(-1, "P", Position<int>(6, 6), board);
	pieces[28] = new Pawn(-1, "P", Position<int>(6, 8), board);
	pieces[29] = new Advisor(-1, "A", Position<int>(9, 3), board);
	pieces[30] = new Advisor(-1, "A", Position<int>(9, 5), board);
	king_pieces = new King(-1, "G", Position<int>(9, 4), board);
	pieces[31] = king_pieces;
}



void Board::controls(const Position<int>& pos1, const Position<int>& pos2)
{
	//先处理指令
	int a = (round % 2 == 1) ? 1 : -1;//谁的回合
	if (board[pos1.x][pos1.y] * a <= 0)
	{
		cout << "选择的点位没有能走的棋子" << endl;
	}
	else//如果第一个if没有执行，那么就足够说明那个点就是自己能走的棋子
	{
		int x = 0;
		if (a == 1)//下方棋子先走
		{
			for (int i = 0; i <= 15; i++)
			{
				if (pieces[i]->pos == pos1 && pieces[i]->if_death == 0)
				{
					if (pieces[i]->rules(pos2 - pos1, board))//最终位置
					{
						for (int j = 16; j <= 31; j++)
						{
							if (pieces[j]->pos == pos2 && pieces[j]->if_death == 0)
							{
								pieces[j]->if_death = round;
							}
						}
						pieces[i]->movement(pos2 - pos1, board);
						round++; x++;
						record_step.insert(record_step.end(), { pos1.x,pos1.y,pos2.x,pos2.y });
					}
				}
				if (i == 15 && x == 0)
				{
					cout << "不能移动到那里" << endl;
				}
			}
		}
		else if (a == -1)
		{
			for (int i = 16; i <= 31; i++)
			{
				if (pieces[i]->pos == pos1 && pieces[i]->if_death == 0)
				{
					if (pieces[i]->rules(pos2 - pos1, board))//最终位置
					{
						for (int j = 0; j <= 15; j++)
						{
							if (pieces[j]->pos == pos2 && pieces[j]->if_death == 0)
							{
								pieces[j]->if_death = round;
							}
						}
						pieces[i]->movement(pos2 - pos1, board);
						round++; x++;
						record_step.insert(record_step.end(), { pos1.x,pos1.y,pos2.x,pos2.y });
					}
				}
				if (i == 31 && x == 0)
				{
					cout << "不能移动到那里" << endl;
				}
			}
		}
	}
	judge(round);
	if (stalemate())
	{
		cout << "       -绝杀-      " << endl;
	}
}
void Board::print(const Position<int>* highlight) const
{
	// 棋子符号数组
	const char red[8] = { ' ', 'r', 'k', 'c', 'b', 'p', 'a', 'g' };   // 8个元素，0不用
	const char black[8] = { ' ', 'R', 'K', 'C', 'B', 'P', 'A', 'G' }; // 同上
	const string lundaosheixia[2] = { "黑","红" };
	int shei = round % 2;
	// 打印列字母提示
	cout << "   第" << round << "回  " << lundaosheixia[shei] << endl;
	cout << "  a b c d e f g h i" << endl;

	for (int i = 0; i <= 9; i++) {
		cout << (9 - i) << " ";
		for (int j = 0; j <= 8; j++) {
			int val = board[9 - i][j];
			bool isHighlight = (highlight != nullptr && highlight->x == (9 - i) && highlight->y == j);


			if (val == 0) setcolor(bgColor);
			else if (val > 0) setcolor(redColor);
			else setcolor(blackColor);
			// 打印棋子
			if (val == 0) {
				cout << ". ";
			}
			else if (val > 0) {
				if (isHighlight) {
					setcolor(0x70);   // 灰色背景，黑色文字（高亮）
				}
				cout << red[val];
				if (isHighlight) {
					setcolor(0x07);   // 灰色背景，黑色文字（高亮）
				}
				cout << " ";
			}
			else {
				if (isHighlight) {
					setcolor(0x70);   // 灰色背景，黑色文字（高亮）
				}
				cout << black[-val];
				if (isHighlight) {
					setcolor(0x07);   // 灰色背景，黑色文字（高亮）
				}
				cout << " ";
			}
			setcolor(7);  // 恢复默认
		}
		cout << endl;
	}
}


Board::~Board() {
	for (int i = 0; i < 32; ++i)
	{
		delete pieces[i];
	}
}
void Board::record_games(string& chess_name)//记录对局数据的
{
	time_t now = time(nullptr);
	string filename = chess_name + "_" + to_string(now) + ".txt";
	ofstream outfile(filename);
	if (!outfile.is_open()) {
		std::cerr << "无法打开文件" << std::endl;
		return;
	}

	outfile << "||Round" << round << endl << "Member" << endl;
	for (int i = 0; i < 16; i++)
	{
		outfile << pieces[i]->if_death << pieces[i]->pos.x << pieces[i]->pos.y << " ";
	}
	outfile << endl;
	for (int i = 16; i < 32; i++)
	{
		outfile << pieces[i]->if_death << pieces[i]->pos.x << pieces[i]->pos.y << " ";
	}
	outfile << endl << "Move" << endl;
	for (size_t i = 0; i < record_step.size(); i += 1) {
		outfile << record_step[i];
	}
	outfile.close();
	ofstream outfile2("filename_records.txt", ios::app);
	if (!outfile2.is_open()) {
		std::cerr << "无法打开文件" << std::endl;
		return;
	}
	outfile2 << filename << endl;//存文件名
	outfile2.close();
	//存入file里面，具体存每个棋子坐标与死亡局数与游戏局数与vector数组数据与游戏名称（根据游戏名称可以直接回溯对局），相比存快照，这样更省内存，而且存快照也需要从头推导一次再存

}

void Board::regret()
{
	// 防止vector越界
	if (record_step.size() < 4)
	{
		return;
	}

	size_t i = record_step.size();

	Position<int> from(record_step[i - 2], record_step[i - 1]);
	Position<int> to(record_step[i - 4] - record_step[i - 2],
		record_step[i - 3] - record_step[i - 1]);


	// 找移动的棋子
	for (int j = 0; j <= 31; j++)
	{
		if (pieces[j]->pos == from && pieces[j]->if_death == 0)
		{
			//反向移动
			pieces[j]->movement(to, board);

			round--;
			record_step.erase(record_step.end() - 4, record_step.end());
			break;
		}
	}


	//恢复被吃棋子
	for (int j = 0; j <= 31; j++)
	{
		if (pieces[j]->if_death == round)
		{
			pieces[j]->if_death = 0;

			board[pieces[j]->pos.x][pieces[j]->pos.y] =
				table[j];

			break;
		}
	}
}

bool Board::judge(int round1, int shifouxianshi)
{
	//判断这一步是否会导致自己被将军


	if (round1 % 2 == 1)
	{
		//黑方走完，现在判断红方是否能攻击黑将
		const Position<int> judge1(
			pieces[31]->pos.x,
			pieces[31]->pos.y
		);


		for (int i = 0; i <= 14; i++)
		{
			if (pieces[i]->if_death == 0 &&
				pieces[i]->rules(judge1 - pieces[i]->pos, board, 0))
			{
				if (shifouxianshi == 1)
				{
					cout << "不能走这一步，否则会被对面将军" << endl;
					regret();
				}

				return false;
			}
		}
	}


	else if (round1 % 2 == 0)
	{
		//红方走完，判断黑方是否攻击红将

		const Position<int> judge1(
			pieces[15]->pos.x,
			pieces[15]->pos.y
		);


		for (int i = 16; i <= 30; i++)
		{
			if (pieces[i]->if_death == 0 &&
				pieces[i]->rules(
					judge1 - pieces[i]->pos,
					board,
					0))
			{

				if (shifouxianshi == 1)
				{
					cout << "不能走这一步，否则会被对面将军" << endl;
					regret();
				}

				return false;
			}
		}
	}



	//判断飞将
	if (king_pieces->iskingexpose(
		pieces[15]->pos,
		board))
	{
		if (shifouxianshi == 1)
		{
			cout << "不能走这一步，否则会被对面飞将" << endl;
			regret();
		}

		return false;
	}



	return true;
}


bool Board::stalemate()
{
	int whoseround = (round % 2 == 1) ? 0 : 16;


	//保存当前状态
	int saveBoard[10][9];

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			saveBoard[i][j] = board[i][j];
		}
	}


	Position<int> savePos[32];
	int saveDeath[32];


	for (int i = 0; i < 32; i++)
	{
		savePos[i] = pieces[i]->pos;
		saveDeath[i] = pieces[i]->if_death;
	}


	unsigned int saveRound = round;



	//尝试一个棋子的所有走法
	for (int idx = whoseround; idx < whoseround + 16; idx++)
	{
		if (pieces[idx]->if_death != 0)
			continue;


		for (int x = -9; x <= 9; x++)
		{
			for (int y = -9; y <= 9; y++)
			{

				if (x == 0 && y == 0)
					continue;


				Position<int> delta(x, y);



				//规则不允许
				if (!pieces[idx]->rules(delta, board, 0))
					continue;



				int tx = pieces[idx]->pos.x + x;
				int ty = pieces[idx]->pos.y + y;



				//模拟吃子
				for (int j = 0; j < 32; j++)
				{
					if (pieces[j]->pos.x == tx &&
						pieces[j]->pos.y == ty &&
						pieces[j]->if_death == 0 &&
						pieces[j]->belongs != pieces[idx]->belongs)
					{
						pieces[j]->if_death = round;
						break;
					}
				}



				//模拟移动
				pieces[idx]->movement(delta, board);
				round++;



				//如果有一步能走
				if (judge(round, 0))
				{
					//恢复
					for (int i = 0; i < 10; i++)
						for (int j = 0; j < 9; j++)
							board[i][j] = saveBoard[i][j];


					for (int i = 0; i < 32; i++)
					{
						pieces[i]->pos = savePos[i];
						pieces[i]->if_death = saveDeath[i];
					}

					round = saveRound;


					return false;
				}




				//恢复
				for (int i = 0; i < 10; i++)
					for (int j = 0; j < 9; j++)
						board[i][j] = saveBoard[i][j];


				for (int i = 0; i < 32; i++)
				{
					pieces[i]->pos = savePos[i];
					pieces[i]->if_death = saveDeath[i];
				}

				round = saveRound;

			}
		}
	}


	return true;
}

void Board::setcolor(int colorcode) const
{
	SetConsoleTextAttribute(g_hConsole, colorcode);
}
//要写一个纯移动函数，读取file并且直接执行移动,与悔棋函数不冲突
void Board::read_games(unsigned int towhichstep, vector<int>* record_round)
{
	if (towhichstep > round && towhichstep >= 1 && (4 * towhichstep - 4) <= record_round->size())
	{
		for (int x = round; x < towhichstep; x++)
		{
			Position<int> from((*record_round)[4 * x - 4], (*record_round)[4 * x - 3]);
			Position<int> to((*record_round)[4 * x - 2], (*record_round)[4 * x - 1]);
			for (int i = 0; i <= 31; i++)
			{
				if (pieces[i]->pos == from && pieces[i]->if_death == 0)
				{
					for (int j = 0; j <= 31; j++)
					{
						if (pieces[j]->pos == to && pieces[j]->if_death == 0)
						{
							pieces[j]->if_death = round;
							break;
						}
					}
					pieces[i]->movement(to - from, board);
					round++;
					record_step.insert(record_step.end(), { from.x,from.y,to.x,to.y });
					break;
				}
			}
		}
	}
	else if (towhichstep < round&& towhichstep >= 1)
	{
		for (int x = round; x > towhichstep; x--)
		{
			regret();
		}
	}
}
//一个game类，记得存名字，设循环g_hConsole
