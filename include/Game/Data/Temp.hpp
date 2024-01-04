#pragma once
#include <Game/Data/Enemy.hpp>

// 临时变量的存储
class Temp {
public:
    int battleEnemyID; // 战斗的敌人ID
    int shopType; // 商店类型
    int functionEventID; // 正在处理的事件ID
    bool closeMS; // 关闭状态栏
    bool directlyFunction; // 转换后直接触发
    bool gameOver; // 游戏结束
    bool ending; // 结局
    bool pause; // 事件指令集执行过程的暂停
    bool autoWalk; // 自动行走
    sf::SoundBuffer voiceBuffer; // 语音buffer
    sf::Sound voice; // 语音
    std::vector <std::string> addPower; // 记录商店增加能力的数值
    std::vector <std::string> initPrice; // 记录商店初始价格
    std::string rise; // 记录商店涨价幅度
    std::string transEventName; // 转换的事件名
    std::vector <std::tuple <int, std::string, std::string> > messageInfo; // 消息相关
    std::vector <Enemy> floorEnemies; // 记录当前楼层怪物
    std::pair <int, int> voiceInfo;

    // 初始化
    void init();
}; extern Temp motaTemp;
