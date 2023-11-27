#pragma once
#include <Game/Data/Actor.hpp>
#include <Game/Data/Element.hpp>

// 敌人数据
class Enemy {
public:
    string name, file; // 敌人名字，文件名
    vector <int> element; // 敌人属性
    int ID, pos, hp, atk, def, conatk, exp, gold, animationID; // 敌人编号，位置，生命值，攻击力，防御力，连击攻击力，经验值，金钱，动画编号

    // 重载==号
    bool operator == (const Enemy& en) const;
    // 获取单回合伤害
    int getSingleDamage(int def);
    // 获取伤害
    int getDamage(Actor* actor, map <int, Element>* elements);
    // 获取真实防御
    int getDef(Actor* actor) const;
    // 是否拥有某属性
    [[nodiscard]] bool getP(int p) const;
    // 获取临界
    int getCrisis(Actor* actor);
    // 获取属性名字和描述
    pair <string, string> getElement(Element element, string buff = "");
    // 序列化和反序列化的宏
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Enemy, name, file, element, pos, hp, atk, def, conatk, exp, gold, animationID)
};
