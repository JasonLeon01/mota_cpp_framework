#pragma once
#include <Game/stdafx.hpp>
#include <Game/System/Input.hpp>
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
    GameWindow(sf::IntRect rect, int opacity = motaSystem.windowOpacity);
    // 绘制矩形
    void drawRect(sf::IntRect rect);
    // 绘制窗口文字（xy为0时文字在窗口左上角）
    void drawWText(sf::IntRect rect, const std::string& content, int pos = 0, LONG size = 20, bool bond = false, sf::Color colour = sf::Color::White) const;
    void drawWTextRotate(sf::IntRect rect, const std::string& content, int pos = 0, LONG size = 20, bool bond = false, sf::Color colour = sf::Color::White, float angle = 0) const;
    void drawWTextSelect(sf::IntRect rect, const std::string& content, int pos = 0, LONG size = 20, bool bond = false, sf::Color colour = sf::Color::White) const;
    // 绘制窗口图形（xy为0时文字在窗口左上角）
    void windowBitmap(const std::string& file, float x, float y, int opacity = 255) const;
    void windowBitmap(const std::string& file, float x, float y, sf::IntRect rect, int opacity = 255) const;
    void windowBitmapSelect(const std::string& file, float x, float y, sf::IntRect rect, int opacity = 255);
    // 绘制箭头
    void showArrow(sf::IntRect rect, int now, int page, std::string pagestr = "");
    // 描绘背景色
    void showBackground(int x, int y, int width, int height);
    // 判定范围内
    static bool inRange(std::pair <int, int> pos, std::tuple <int, int, int, int> range);
};

// 处理带选择项窗口的类
class GameSelectWindow :public GameWindow {
public:
    int index, rectHeight;
    bool active;
    std::vector <std::string> items;

    // 构造函数，初始化对象
    GameSelectWindow();
    GameSelectWindow(const std::vector <std::string>& item, int rectheight, int width, int height = 0, bool visible = true);
    // 刷新
    void refresh() override;

private:
    int hideLines;
};
