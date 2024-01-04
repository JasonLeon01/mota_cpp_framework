#pragma once
#include <Game/Data/Screen.hpp>

// 快捷键提示窗口
class WindowHint :public GameWindow {
public:
    int nowPage; // 当前页数

    // 构造函数，初始化对象
    WindowHint(int x, int y);
    // 刷新
    void refresh() override;

private:
    std::vector <std::string> keys; // 存储快捷键描述的变量
    int maxPage; // 最大页数
};
