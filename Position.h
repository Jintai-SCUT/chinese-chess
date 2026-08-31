#pragma once
#include <iostream>

template<typename T>
class Position {
public:
    T x, y;
    //默认构造
    Position() : x(0), y(0) {}
    // 构造函数
    Position(T x0, T y0) : x(x0), y(y0) {}
    Position& operator=(const Position& other) {
        if (this != &other) {          // 自赋值检查
            x = other.x;
            y = other.y;
        }
        return *this;                  // 返回当前对象的引用
    }

    // 算术运算符+重载
    Position operator+(const Position& other) const
    {
        return Position(x + other.x, y + other.y);
    }
    Position operator-(const Position& other) const
    {
        return Position(x - other.x, y - other.y);
    }
    friend std::ostream& operator<<(std::ostream& os, const Position& pos) {
        os << pos.x << " " << pos.y;
        return os;
    }

    // 条件运算符 == !=
    bool operator==(const Position& other) const
    {
        return x == other.x && y == other.y;
    }
    bool operator!=(const Position& other) const
    {
        return !(*this == other);
    }
};

