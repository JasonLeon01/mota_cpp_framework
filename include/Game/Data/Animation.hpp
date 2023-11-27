#pragma once
#include <Game/stdafx.hpp>

// 动画数据
struct Animation {
    vector <string> patterns; // 动画帧
    string SEFile; // 音效文件名
    int SETime; // 音效播放的时间
    // 序列化和反序列化的宏
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Animation, patterns, SEFile, SETime)
};

// 动画播放器的数据
struct SpriteAnimation {
    int actX, actY, currentFrame; // 动画的x，y坐标，当前帧
    Animation type; // 动画类型
};
