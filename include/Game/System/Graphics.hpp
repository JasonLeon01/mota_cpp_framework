#pragma once
#include <Game/stdafx.hpp>
#include <Game/System/Input.hpp>
#include <Game/System/Image.hpp>
#include <Game/System/Text.hpp>
#include <Game/System/Window.hpp>

// 游戏主画面相关的类
class GameGraphics {
public:
    // 刷新画面
    void update(bool clear_device = true);
    // 插入图片
    void addImage(GameImage* obj);
    // 消除图片
    void eraseImage(GameImage* obj);
    // 插入窗口
    void addWindow(GameWindow* obj);
    // 消除窗口
    void eraseWindow(GameWindow* obj);
    // 插入文字
    void addText(GameText* obj);
    // 消除文字
    void eraseText(GameText* obj);
    // 消除画面
    void dispose();
private:
    std::vector <GameImage*> patterns;
    std::vector <GameWindow*> windows;
    std::vector <GameText*> texts;
}; extern GameGraphics motaGraphics;
