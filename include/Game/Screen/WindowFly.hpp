#pragma once
#include <Game/Data/Screen.hpp>

// 楼层传送器窗口
class WindowFly :public GameWindow {
public:
    int viewFloorIndex; // 当前查看的楼层ID

    // 构造函数，初始化对象
    WindowFly(int x, int y);
    // 刷新
    void refresh() override;
};
