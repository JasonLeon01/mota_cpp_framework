#pragma once
#include <Game/Data/Screen.hpp>

// 物品窗口
class WindowItem :public GameWindow {
public:
    int index; // 当前选择项
    std::vector <Item> myItems; // 物品列表

    // 构造函数，初始化对象
    WindowItem(int x, int y);
    // 刷新
    void refresh() override;

private:
    int hideLines;
};
