#pragma once
#include <Game/Data/Screen.hpp>

// 怪物百科窗口
class WindowEnemyEncyclopedia :public GameWindow {
public:
    Enemy mon; // 怪物信息

    // 构造函数，初始化对象
    WindowEnemyEncyclopedia(int x, int y);
    // 刷新
    void refresh() override;
};
