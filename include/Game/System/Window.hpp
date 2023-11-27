#pragma once
#include <Game/stdafx.hpp>
#include <Game/System/KeyBoard.hpp>
#include <Game/System/System.hpp>

// 所有游戏窗口的基类，包括纯虚函数Refresh，通过此类可以获取窗口属性，绘制矩形以及在窗口内描绘文字和图像
class GameWindow {
public:
    float x, y; // 坐标
    int z, width, height, opacity; // 层级，宽高，不透明度
    bool haveFunction, visible; // 是否有功能，是否可见
    virtual void refresh() = 0; // 刷新(纯虚函数)

    // 构造函数，初始化对象
    GameWindow();
    GameWindow(IntRect rect, int opacity = motaSystem.windowOpacity);
    // 绘制矩形
    void drawRect(IntRect rect, int gtime);
    // 绘制窗口文字（xy为0时文字在窗口左上角）
    void drawWText(IntRect rect, const string& content, int pos = 0, LONG size = 20, bool bond = false, Color colour = Color::White) const;
    void drawWTextRotate(IntRect rect, const string& content, int pos = 0, LONG size = 20, bool bond = false, Color colour = Color::White, float angle = 0) const;
    void drawWTextSelect(IntRect rect, const string& content, int pos = 0, LONG size = 20, bool bond = false, Color colour = Color::White) const;
    // 绘制窗口图形（xy为0时文字在窗口左上角）
    void windowBitmap(const string& file, float x, float y, int opacity = 255) const;
    void windowBitmap(const string& file, float x, float y, IntRect rect, int opacity = 255) const;
    void windowBitmapSelect(const string& file, float x, float y, IntRect rect, int opacity = 255);
    // 绘制箭头
    void showArrow(IntRect rect, int now, int page, string pagestr = "");
    // 描绘背景色
    void showBackground(int x, int y, int width, int height);
};

// 处理带选择项窗口的类
class GameSelectWindow :public GameWindow {
public:
    int index, rectHeight;
    bool active;
    vector <string> items;

    // 构造函数，初始化对象
    GameSelectWindow();
    GameSelectWindow(int width, int rectheight, const vector <string>& item, bool visible = true);
    // 刷新
    void refresh() override;
    // 绘制项目
    void drawItem(int idx, Color colour);
};
