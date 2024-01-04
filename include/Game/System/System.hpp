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
    sf::RenderWindow window; // 游戏窗口
    sf::Font font; // 游戏字体
    sf::Music bgm; // 游戏音乐
    SceneBase* scene; // 所有场景的基类
    unsigned int gameTime; // 游戏时间
    float resolutionRatio; // 分辨率
    short frameRate; // 帧率
    short windowOpacity; // 窗口不透明度
    float BGMVolume; // BGM音量
    float SEVolume; // SE音量
    bool verticalSync; // 垂直同步
    bool smooth; // 柔化
    std::string windowskinName; // 窗口文件名
    std::string titleName; // 标题名
    std::string fontName; // 字体名
    std::string titleBGM; // 标题背景音乐
    std::string cursorSE; // 选择SE
    std::string decisionSE; // 确认SE
    std::string cancelSE; // 取消SE
    std::string buzzerSE; // 警告SE
    std::string shopSE; // 商店SE
    std::string saveSE; // 存档SE
    std::string loadSE; // 读档SE
    std::string gateSE; // 开门SE
    std::string stairSE; // 上下楼SE
    std::string getSE; // 获取道具SE
    std::map <std::string, sf::Texture> textureCache; // 纹理缓存

    // 初始化函数
    void init();
    // 更改游戏bgm
    void bgmSwitch(const std::string& file);
}; extern System motaSystem;