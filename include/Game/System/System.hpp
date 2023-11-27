#pragma once
#include <Game/stdafx.hpp>

// 所有场景类的基类，仅包含纯虚函数
class SceneBase {
public:
    virtual void main() = 0;
    virtual void update() = 0;
};

//系统类，用来储存各种参数
class System {
public:
    RenderWindow window; // 游戏窗口
    Font font; // 游戏字体
    Music bgm; // 游戏音乐
    SceneBase* scene; // 所有场景的基类
    unsigned int gameTime; // 游戏时间
    float resolutionRatio; // 分辨率
    short frameRate; // 刷新频率
    short windowOpacity; // 窗口不透明度
    float BGMVolume; // BGM音量
    float SEVolume; // SE音量
    string windowskinName; // 窗口文件名
    string titleName; // 标题名
    string fontName; // 字体名
    string titleBGM; // 标题背景音乐
    string cursorSE; // 选择SE
    string decisionSE; // 确认SE
    string cancelSE; // 取消SE
    string buzzerSE; // 警告SE
    string shopSE; // 商店SE
    string saveSE; // 存档SE
    string loadSE; // 读档SE
    string gateSE; // 开门SE
    string stairSE; // 上下楼SE
    string getSE; // 获取道具SE
    map <string, Texture> textureCache; // 纹理缓存

    // 初始化函数
    void init();
    // 更改游戏bgm
    void bgmSwitch(const string& file);
}; extern System motaSystem;