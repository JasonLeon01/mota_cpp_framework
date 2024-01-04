#pragma once
#include <Game/Data/Screen.hpp>

// 对话信息窗口
class WindowMessage :public GameWindow {
public:
    bool haveIndex; // 是否有选项
    int index; // 有选项对话的选项
    std::map <int, std::vector<std::string> > order; // 指令

    // 构造函数，初始化对象
    WindowMessage(int x, int y);
    // 刷新
    void refresh() override;

private:
    int maxIndex;
};
