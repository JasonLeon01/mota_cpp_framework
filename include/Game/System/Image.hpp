#pragma once
#include <Game/stdafx.hpp>
#include <Game/System/System.hpp>

// Image类，用于填充图像
class GameImage {
public:
    float x, y, origin_x, origin_y, scale_x, scale_y, angle; // 坐标，原点，缩放，角度
    int z, width, height, sx, sy, opacity; // 层级，宽高，源坐标，不透明度
    bool visible; // 是否可见

    // 构造函数，初始化对象
    GameImage();
    GameImage(std::string file, float x = 0, float y = 0, int width = 0, int height = 0, int sx = 0, int sy = 0);
    // 设置图像信息
    void setSprite(std::string file, float x = 0, float y = 0, int width = 0, int height = 0, int sx = 0, int sy = 0);
    // 获取精灵
    void show();
private:
    std::string imgFile;
};
