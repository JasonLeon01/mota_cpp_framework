#pragma once
#include <Game/Data/Variables.hpp>

// 游戏角色
class Actor {
public:
    string name, file; // 角色名字，文件名
    set <int> status; // 角色状态
    map <int, int> item; // 道具
    int ID, level, hp, atk, def, mdef, exp, gold, mapID, x, y, animationID; // 角色编号，等级，生命值，攻击力，防御力，魔防，经验值，金钱，所在地图，坐标，动画编号

    // 构造函数，初始化对象
    Actor();
    // 获取实际攻防（算上衰弱效果）
    [[nodiscard]] int getAtk() const;
    [[nodiscard]] int getDef() const;
    // 序列化和反序列化的宏
    NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Actor, name, file, status, level, hp, atk, def, mdef, exp, gold, item, mapID, x, y, animationID)
};
