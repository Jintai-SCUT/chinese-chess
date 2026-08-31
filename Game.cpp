#include "Game.h"
using namespace std;
vector<string> splitLines(const string& str) {
	vector<string> lines;
	stringstream ss(str);
	string line;
	while (getline(ss, line, '\n')) {
		// 去掉末尾可能存在的 \r（Windows换行符）
		if (!line.empty() && line.back() == '\r') line.pop_back();
		if (!line.empty()) {
			lines.push_back(line);
		}
	}
	return lines;
}



Game::Game()
{
	hinput = GetStdHandle(STD_INPUT_HANDLE);
	houtput = GetStdHandle(STD_OUTPUT_HANDLE);
	player1 = "none";
	player2 = "none";
	timerRunning = false;
	remainingSeconds = timeLimit;
}
//已完结
Game::~Game()
{
	if (timerRunning) {
		timerRunning = false;
		if (timerThread.joinable()) timerThread.join();
	}
}
//已完结



//倒计时等等
void Game::timerLoop()
{
	auto start = chrono::steady_clock::now();
	while (timerRunning) {
		auto now = chrono::steady_clock::now();
		int elapsed = chrono::duration_cast<chrono::seconds>(now - start).count();
		int remain = timeLimit - elapsed;
		if (remain < 0) remain = 0;
		if (remain != remainingSeconds.load()) {
			remainingSeconds = remain;
			updateTimerDisplay();
		}
		if (remain <= 0) {
			timerRunning = false;
			break;
		}
		this_thread::sleep_for(chrono::milliseconds(200));
	}
}

void Game::updateTimerDisplay()
{
	gotoxy(0, 0);
	if (remainingSeconds >= 0) {
		cout << "倒计时: " << setw(2) << setfill('0') << remainingSeconds.load() << " 秒   ";
	}
	else {
		cout << "倒计时: 超时!      ";
	}
	cout << setfill(' ');
	cout << flush;
	gotoxy(1, 0);
}

//
void Game::recordResult(bool redWin)
{
	if (player1 == "none" && player2 == "none") return;

	struct PlayerRecord { string name; int wins, losses, net; };
	vector<PlayerRecord> records;
	ifstream inFile("rankings.txt");
	if (inFile.is_open()) {
		string name; int w, l, net;
		while (inFile >> name >> w >> l >> net)
			records.push_back({ name, w, l, net });
		inFile.close();
	}

	string winner = redWin ? player1 : player2;
	string loser = redWin ? player2 : player1;

	auto updatePlayer = [&](const string& name, bool isWin) {
		bool found = false;
		for (auto& rec : records) {
			if (rec.name == name) {
				if (isWin) rec.wins++; else rec.losses++;
				rec.net = rec.wins - rec.losses;
				found = true;
				break;
			}
		}
		if (!found && name != "none") {
			PlayerRecord newRec;
			newRec.name = name;
			newRec.wins = isWin ? 1 : 0;
			newRec.losses = isWin ? 0 : 1;
			newRec.net = newRec.wins - newRec.losses;
			records.push_back(newRec);
		}
		};

	if (winner != "none") updatePlayer(winner, true);
	if (loser != "none")  updatePlayer(loser, false);

	ofstream outFile("rankings.txt");
	if (!outFile.is_open()) {
		cerr << "无法写入排行榜文件！" << endl;
		return;
	}
	for (const auto& rec : records)
		outFile << rec.name << " " << rec.wins << " " << rec.losses << " " << rec.net << endl;
	outFile.close();
}




int Game::home()
{
	system("cls");
	//循环，按到什么就实现什么函数。
	home_print();
	Position<int> getposes;
	int numberofround = 1;
	int recordpasspos = 0;
	while (true)
	{
		setmouseinput();
		getposes = getpos();
		if (numberofround == 1)//设置高亮的,左键合法设置，不合法就无动于衷
		{
			if (getposes.y < 6 && getposes.y>1)
			{
				gotoxy(0, 0);
				home_print(getposes.y);
				recordpasspos = getposes.y;
				numberofround = 2;
			}
			else
			{
				continue;
			}
		}
		else//第二轮，左键同值有效，不同值可能不变可能转移高亮，右键取消高亮
		{
			if (getposes.x == -1 && getposes.y == -1)//右键重置
			{
				gotoxy(0, 0);
				home_print();
				recordpasspos = 0;
				numberofround = 1;
			}
			else if (getposes.y > 5 || getposes.y < 2)
			{
				continue;//不改变任何东西，就像没按一样
			}
			else if (getposes.y < 6 && getposes.y>1 && getposes.y != recordpasspos)
			{
				//跳过1
				gotoxy(0, 0);
				home_print(getposes.y);
				recordpasspos = getposes.y;
			}
			else if (getposes.y < 6 && getposes.y>1 && getposes.y == recordpasspos)
			{
				switch (getposes.y)
				{
				case 2:
					return 2;          // 进入对局，结束后返回 home
					break;
				case 3:
					iofiles(2);// 文件操作,打开
					system("cls");
					home_print();
					break;
				case 4:
					home_settings();
					system("cls");
					home_print();
					break;
				case 5:
					home_ranking();
					system("cls");
					home_print();
					break;
				default:
					break;
				}
				recordpasspos = 0;
				numberofround = 1;
			}
		}
		//循环设置
	}
	return 0;
}
//未完结


void Game::home_print(int highlight_home)
{
	int highlight = (highlight_home > 1 && highlight_home < 6) ? highlight_home - 2 : 6;
	const WORD COLOR_NORMAL = 0x07;
	const WORD MENU_COLOR_HIGHLIGHT = 0x70;
	SetConsoleTextAttribute(houtput, COLOR_NORMAL);
	cout << "====[中国象棋对决]====" << endl;
	cout << "=       -菜单-       =" << endl;
	vector<string> items = { "=    打开新的棋局    =", "=    打开历史对局    =", "=      更改设置      =", "=      查看排行      =" };
	for (int i = 0; i < 4; ++i) {
		if (i == highlight) {
			SetConsoleTextAttribute(houtput, MENU_COLOR_HIGHLIGHT);
		}
		else {
			SetConsoleTextAttribute(houtput, COLOR_NORMAL);
		}
		cout << items[i] << endl;
	}
	SetConsoleTextAttribute(houtput, COLOR_NORMAL);
	cout << "=                    =" << endl;
	cout << "======================" << endl;
}
//已完善

void Game::home_ranking()
{
	system("cls");
	setmouseinput();
	cout << "<--[return]" << endl;
	cout << "=== 排行榜（按净胜场） ===" << endl;
	ifstream rankFile("rankings.txt");
	if (!rankFile.is_open()) {
		cout << "暂无对局记录。" << endl;
	}
	else {
		struct PlayerRecord { string name; int wins, losses, net; };
		vector<PlayerRecord> records;
		string name; int w, l, net;
		while (rankFile >> name >> w >> l >> net)
			records.push_back({ name, w, l, net });
		rankFile.close();
		sort(records.begin(), records.end(),
			[](const PlayerRecord& a, const PlayerRecord& b) { return a.net > b.net; });
		cout << "排名\t选手\t胜\t负\t净胜" << endl;
		int rank = 1;
		for (const auto& rec : records)
			cout << rank++ << "\t" << rec.name << "\t" << rec.wins << "\t" << rec.losses << "\t" << rec.net << endl;
	}
	while (true) {
		Position<int> pos = getpos();
		if (pos.y == 0) return;
	}
}
//已完结



int Game::ingame()
{
	system("cls");
	Board newgames(redColor, blackColor, bgColor);

	// 初始启动计时器（如果有时间限制）
	if (timeLimit > 0) {
		remainingSeconds = timeLimit;
		timerRunning = true;
		if (timerThread.joinable()) timerThread.join();
		timerThread = thread(&Game::timerLoop, this);
		updateTimerDisplay();
	}

	int menuHighlight = 20;
	bool hasSelected = false;
	Position<int> from;
	int menuPhase = 1;
	int menuSelected = 0;
	vector<string> infoLines;

	auto refresh = [&]() {
		gotoxy(0, 0);
		updateTimerDisplay();           // 倒计时在 y=0
		gotoxy(0, 1);                   // 棋盘从 y=1 开始
		if (hasSelected) newgames.print(&from);
		else newgames.print();
		// 信息行 y=13~14
		gotoxy(0, 13);
		for (int i = 0; i < 2; ++i) {
			if (i < (int)infoLines.size())
				cout << left << setw(30) << infoLines[i] << endl;
			else
				cout << string(30, ' ') << endl;
		}
		// 菜单 y=15~17
		gotoxy(0, 15);
		printIngameMenu(menuHighlight);
		};

	refresh();

	while (true) {
		// 超时检测
		if (timerRunning && remainingSeconds <= 0) {
			timerRunning = false;
			if (timerThread.joinable()) timerThread.join();
			bool redWin = (newgames.round % 2 == 0);
			recordResult(redWin);
			infoLines.clear();
			infoLines.push_back(string("时间到！") + (redWin ? "红方" : "黑方") + "胜！");
			refresh();
			return 1;
		}

		setmouseinput();
		Position<int> pos = getpos();

		if (pos.x == -1 && pos.y == -1) {
			hasSelected = false;
			menuPhase = 1;
			menuSelected = 0;
			menuHighlight = 20;
			infoLines.clear();
			refresh();
			continue;
		}

		// ----- 棋盘区域（y=3~12）-----
		if (pos.x >= 1 && pos.x <= 18 && pos.y >= 3 && pos.y <= 12) {
			if (menuPhase == 2) continue;
			Position<int> boardPos = consoleToBoard(pos.x, pos.y);
			if (boardPos.x == -1) continue;
			if (!hasSelected) {
				int color = (newgames.round % 2 == 1) ? 1 : -1;
				if (newgames.board[boardPos.x][boardPos.y] * color > 0) {
					from = boardPos;
					hasSelected = true;
					refresh();
				}
				continue;
			}
			else {
				if (boardPos.x == from.x && boardPos.y == from.y) {
					hasSelected = false;
					refresh();
					continue;
				}
				else {
					// 记录走棋前步数
					size_t oldSize = newgames.record_step.size();

					stringstream buffer;
					auto old = cout.rdbuf(buffer.rdbuf());
					newgames.controls(from, boardPos);
					cout.rdbuf(old);
					hasSelected = false;

					string raw = buffer.str();
					vector<string> lines;
					if (!raw.empty()) {
						stringstream ss(raw);
						string line;
						while (getline(ss, line, '\n')) {
							if (!line.empty() && line.back() == '\r') line.pop_back();
							if (!line.empty()) lines.push_back(line);
						}
					}
					if (lines.size() > 2) lines.resize(2);
					infoLines.swap(lines);
					refresh();

					// 如果走棋成功（record_step 增加），重置倒计时
					if (newgames.record_step.size() > oldSize && timeLimit > 0) {
						timerRunning = false;
						if (timerThread.joinable()) timerThread.join();
						remainingSeconds = timeLimit;
						timerRunning = true;
						timerThread = thread(&Game::timerLoop, this);
						updateTimerDisplay();
						// 刷新显示倒计时
						refresh();
					}

					if (newgames.stalemate()) {
						// 停止计时器
						timerRunning = false;
						if (timerThread.joinable()) timerThread.join();

						bool redWin = (newgames.round % 2 == 0);
						recordResult(redWin);
						infoLines.clear();
						infoLines.push_back(string("绝杀！") + (redWin ? "红方" : "黑方") + "胜！");
						refresh();
					}
					continue;
				}
			}
		}

		// ----- 菜单区域（y=15~17）-----
		if (pos.y >= 16 && pos.y <= 18) {
			if (hasSelected) continue;
			if (menuPhase == 1) {
				menuSelected = pos.y;
				menuHighlight = pos.y;
				menuPhase = 2;
				refresh();
				continue;
			}
			else {
				if (pos.y == menuSelected) {
					menuPhase = 1;
					menuHighlight = 20;
					switch (pos.y) {
					case 16:
						newgames.regret();
						infoLines.clear();
						refresh();
						break;
					case 17:
						timerRunning = false;
						if (timerThread.joinable()) timerThread.join();
						iofiles(1, &newgames);
						return 1;
					case 18:
						timerRunning = false;
						if (timerThread.joinable()) timerThread.join();
						return 1;
					}
					infoLines.clear();
					refresh();
				}
				else {
					menuSelected = pos.y;
					menuHighlight = pos.y;
					refresh();
				}
				continue;
			}
		}
	}
	return 0;
}//已完善

void Game::printIngameMenu(int PIMhighlight)
{
	const WORD COLOR_NORMAL = 0x07;
	const WORD MENU_COLOR_HIGHLIGHT = 0x70;

	int index = -1;
	if (PIMhighlight >= 16 && PIMhighlight <= 18) {
		index = PIMhighlight - 16;
	}

	vector<string> items = {
		"       悔棋       ",
		"     保存退出     ",
		"     直接退出     "
	};

	SetConsoleTextAttribute(houtput, COLOR_NORMAL);
	cout << "───── 对局菜单 ─────" << endl;

	for (int i = 0; i < 3; ++i) {
		if (i == index) SetConsoleTextAttribute(houtput, MENU_COLOR_HIGHLIGHT);
		else SetConsoleTextAttribute(houtput, COLOR_NORMAL);
		cout << items[i] << endl;
	}

	SetConsoleTextAttribute(houtput, COLOR_NORMAL);
	cout << "───────────────────" << endl;
}







Position<int> Game::getpos()
{
	INPUT_RECORD mouseinput_record;
	DWORD events;

	while (true)
	{
		ReadConsoleInput(hinput, &mouseinput_record, 1, &events);

		if (events == 0)
			continue;
		if (mouseinput_record.EventType != MOUSE_EVENT)
			continue;
		MOUSE_EVENT_RECORD& mouse = mouseinput_record.Event.MouseEvent;
		//忽略鼠标移动
		if (mouse.dwEventFlags == MOUSE_MOVED)
			continue;
		//忽略鼠标释放
		if (mouse.dwButtonState == 0)
			continue;
		//右键取消
		if (mouse.dwButtonState & RIGHTMOST_BUTTON_PRESSED)
		{
			return Position<int>(-1, -1);
		}
		//左键点击
		if (mouse.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			int consoleX = mouse.dwMousePosition.X;
			int consoleY = mouse.dwMousePosition.Y;

			return Position<int>(consoleX, consoleY);
		}
	}
}
void Game::setmouseinput()
{
	//句柄、位掩码
	if (hinput == INVALID_HANDLE_VALUE)
	{
		cerr << "无法得到句柄" << endl;
		return;
	}
	DWORD mode;
	if (!GetConsoleMode(hinput, &mode))
	{
		cerr << "无法初始化位掩码" << endl;
		return;
	}
	mode |= ENABLE_MOUSE_INPUT;
	mode |= ENABLE_EXTENDED_FLAGS;

	// 关键：关闭快速编辑，否则鼠标点击会被控制台截走
	mode &= ~ENABLE_QUICK_EDIT_MODE;

	if (!SetConsoleMode(hinput, mode))
	{
		cerr << "无法设置鼠标输入模式" << endl;
		return;
	}
}
//已完结
void Game::gotoxy(int x, int y) {
	COORD coord = { (SHORT)x, (SHORT)y };
	SetConsoleCursorPosition(houtput, coord);
}
Position<int> Game::consoleToBoard(int consoleX, int consoleY) const
{
	
	int resultx = 12 - consoleY;    
	int resulty = (consoleX - 1) / 2;
	return Position<int>(resultx, resulty);
}

void Game::home_settings()
{
	system("cls");
	cout << "=== 设置 ===" << endl;
	cout << "1. 修改选手名称" << endl;
	cout << "2. 修改棋盘颜色" << endl;
	cout << "3. 修改倒计时时长" << endl;
	cout << "0. 返回" << endl;
	int choice;
	cin >> choice;
	switch (choice) {
	case 1: {
		cout << "当前红方选手: " << player1 << endl;
		cout << "输入新的红方选手名 (输入 none 表示匿名): ";
		string p1; cin >> p1;
		cout << "当前黑方选手: " << player2 << endl;
		cout << "输入新的黑方选手名: ";
		string p2; cin >> p2;
		setPlayerNames(p1, p2);
		cout << "设置成功！" << endl;
		system("pause");
		break;
	}
	case 2: {
		cout << "输入红方颜色码 (0-15, 默认12): ";
		int r; cin >> r;
		cout << "输入黑方颜色码 (0-15, 默认1): ";
		int b; cin >> b;
		cout << "输入背景颜色码 (0-15, 默认7): ";
		int bg; cin >> bg;
		setColors(r, b, bg);
		cout << "设置成功！" << endl;
		system("pause");
		break;
	}
	case 3: {
		cout << "当前每局时间限制: " << timeLimit << " 秒" << endl;
		cout << "输入新的时间（秒，0表示无限）: ";
		int t; cin >> t;
		setTimeLimit(t);
		cout << "设置成功！" << endl;
		system("pause");
		break;
	}
	default: break;
	}
}
//未完结



void Game::iofiles(int iomode, Board* board)
{
	const WORD COLOR_NORMAL = 0x07;
	const WORD MENU_COLOR_HIGHLIGHT = 0x70;
	if (iomode == 1)//输入模式，自定义输入名字
	{
		cout << "请输入文件名(仅支持英文）:";
		string cin_filename;
		cin >> cin_filename;
		board->record_games(cin_filename);
		return;
	}
	else if (iomode == 2)//输出模式，打开一个文件
	{
		system("cls");
		ifstream list_file("filename_records.txt");
		if (!list_file.is_open())
		{
			cout << "无法打开记录列表文件";
			return;
		}
		string line;
		vector<string> lines;//得到每一个名字
		cout << "<--[return]" << endl;
		while (getline(list_file, line))
		{
			lines.push_back(line);
			cout << line << endl;
		}
		int numberofround = 1;
		int recordposy = 0;
		Position<int> getposes;
		while (true)
		{
			setmouseinput();
			getposes = getpos();
			if (getposes.y == 0)
			{
				return;
			}
			else if (getposes.x == -1 && getposes.y == -1)//右键重置
			{
				gotoxy(0, 0);
				cout << "<--[return]" << endl;
				for (int i = 0; i < lines.size(); i++)
				{
					cout << lines[i] << endl;
				}
				recordposy = 0;
				numberofround = 1;
			}
			else if (getposes.y >= 1 && getposes.y <= lines.size() && numberofround == 1)
			{
				gotoxy(0, 0);
				cout << "<--[return]" << endl;
				for (int i = 0; i < lines.size(); i++)
				{
					if ((i + 1) == getposes.y)
					{
						SetConsoleTextAttribute(houtput, MENU_COLOR_HIGHLIGHT);
						cout << lines[i] << endl;
						SetConsoleTextAttribute(houtput, COLOR_NORMAL);
					}
					else
					{
						cout << lines[i] << endl;
					}
				}
				numberofround = 2;
				recordposy = getposes.y;
			}
			else if (getposes.y >= 1 && getposes.y <= lines.size() && numberofround == 2)
			{

				if (recordposy != getposes.y)
				{
					gotoxy(0, 0);
					cout << "<--[return]" << endl;
					for (int i = 0; i < lines.size(); i++)
					{
						if ((i + 1) == getposes.y)
						{
							SetConsoleTextAttribute(houtput, MENU_COLOR_HIGHLIGHT);
							cout << lines[i] << endl;
							SetConsoleTextAttribute(houtput, COLOR_NORMAL);
						}
						else
						{
							cout << lines[i] << endl;
						}
					}
					recordposy = getposes.y;
				}
				else if (recordposy == getposes.y)
				{
					string open_file = lines[getposes.y - 1];
					ifstream opengames(open_file);
					if (!opengames.is_open()) {
						cout << "文件已经损坏或丢失" << endl;
					}
					else
					{
						Board read;
						vector<int>read_step;
						string line;
						int lineCount = 0;         // 行计数器
						string firstLine;
						getline(opengames, firstLine);
						if (firstLine.length() >= 8) {
							string numStr = firstLine.substr(7);   // 第8个字符开始
							int rounds = stoi(numStr);             // 直接转换，无异常处理
							read_step.reserve(rounds * 4);         // 预留空间
						}
						// 逐行读取文件
						// 读取第一行（回合数）后，剩余的所有整数逐个读取
						for (int i = 0; i < 4; ++i) {
							string dummy;
							if (!getline(opengames, dummy)) break;   // 若提前结束则退出
						}
						string all;
						string otherline;
						while (getline(opengames, otherline)) {
							all += otherline;                // 拼接所有剩余行
						}

						// 去除所有非数字字符（如果混有空格或换行）
						string digits;
						for (char c : all) {
							if (isdigit(c)) digits.push_back(c);
						}
						for (char c : digits) {
							read_step.push_back(c - '0');  // 字符转数字
						}
						iofile_read(&read_step, &read);
						system("cls");
						cout << "<--[return]" << endl;
						for (const string& line : lines) {
							cout << line << endl;  // 逐行打印，不包含引号
						}


						//依赖于文件的行为链，目前只做读功能
					}
				}
				//elseif 对应的括号
			}


		}

	}

}


void Game::iofile_read(vector<int>* moveline, Board* boards)
{
	system("cls");
	cout << "<--[return] " << endl;          // 返回菜单提示
	boards->print();                    // 打印棋盘（假设 Board 有 print 方法）
	cout << "回退一步|前进一步" << endl;
	cout << "[跳转到]:       " << endl;
	while (true)
	{
		setmouseinput();                     // 转换输入模式
		Position<int> pos;
		pos = getpos();        // 获取鼠标坐标

		// y == 0 退出
		if (pos.y == 0)
			return;

		// 只有 y == 12 时处理鼠标点击
		if (pos.y == 13)
		{
			bool called = false;             // 标记是否调用了 read_games

			// 根据 x 坐标决定操作
			if (pos.x >= 0 && pos.x <= 9)    // 回退一步
			{
				int step = boards->round - 1;
				if (step >= 1)               // 确保步数有效
				{
					boards->read_games(static_cast<unsigned int>(step), moveline);
					called = true;
				}
			}
			else if (pos.x >= 10 && pos.x <= 18)   // 前进一步
			{
				int step = boards->round + 1;
				boards->read_games(static_cast<unsigned int>(step), moveline);
				called = true;
			}
			// 若成功调用了 read_games，则执行后续显示
			if (called)
			{
				gotoxy(0, 0);
				cout << "<--[return]" << endl;          // 返回菜单提示
				boards->print();                    // 打印棋盘（假设 Board 有 print 方法）
				cout << "回退一步|前进一步" << endl;
				cout << "[跳转到]:       " << endl;
			}
		}
		else if (pos.y == 14)  // 其他 x 范围 , 键盘输入跳转步数
		{
			cout << "请输入   " << endl;
			int input;
			cin >> input;
			if (input > 0)               // 过滤负数或0
			{
				boards->read_games(static_cast<unsigned int>(input), moveline);
				gotoxy(0, 0);
				cout << "<--[return] " << endl;          // 返回菜单提示
				boards->print();                    // 打印棋盘（假设 Board 有 print 方法）
				cout << "回退一步|前进一步" << endl;
				cout << "[跳转到]:       " << endl;

			}
			// 输入无效则忽略，不调用
		}

	}
}


void Game::setPlayerNames(const string& p1, const string& p2) { player1 = p1; player2 = p2; }
void Game::setTimeLimit(int seconds) { timeLimit = seconds; }
void Game::setColors(int red, int black, int bg) { redColor = red; blackColor = black; bgColor = bg; }

