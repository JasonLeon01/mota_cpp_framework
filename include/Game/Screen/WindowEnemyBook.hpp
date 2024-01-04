#pragma once
#include <Game/Data/Screen.hpp>

// 怪物手册窗口
class WindowEnemyBook :public GameWindow {
public:
    int index, nowPage, from; // 当前选择项、当前页数、来源（地图或菜单）

    // 构造函数，初始化对象
    WindowEnemyBook(int x, int y);
    // 刷新
    void refresh() override;

private:
    int maxPage;
};
