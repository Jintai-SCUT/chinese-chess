#include"Board.h"
#include"Game.h"
#include<iostream>
#include <windows.h>
using namespace std;

int main() {


    Game game;

    int state = 1;  // 1=主菜单，2=对局

    while (true) {
        if (state == 1) {
            state = game.home();
        }
        else if (state == 2) {
            state = game.ingame();
        }
        else {
            break;
        }
    }

    return 0;
}//创建一个game函数，智能识别指令（数字输入或者字母加数字或者单个字母，然后分别代表