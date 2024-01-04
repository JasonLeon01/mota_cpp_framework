#pragma once
#include <Game/Data/Screen.hpp>

// 存读档窗口
class WindowLoadSave :public GameWindow {
public:
    int index, from; // 当前选择项、来源（地图或菜单）
    bool load; // 是否是读档

    // 构造函数，初始化对象
    WindowLoadSave(int x, int y);
    // 刷新
    void refresh() override;

private:
    sf::Texture savTexture; // 存储存档截图的纹理
    int hideLines;
};
