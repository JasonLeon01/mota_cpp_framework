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
    bool nextMove; // 事件执行到下一阶段的标志
    vector <string> addPower; // 记录商店增加能力的数值
    vector <string> initPrice; // 记录商店初始价格
    string rise; // 记录商店涨价幅度
    string transEventName; // 转换的事件名
    vector <tuple <int, string, string> > messageInfo; // 消息相关
    vector <Enemy> floorEnemies; // 记录当前楼层怪物

    // 初始化
    void init();
}; extern Temp motaTemp;
