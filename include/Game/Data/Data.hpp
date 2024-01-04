#pragma once
#include <Game/Data/Animation.hpp>
#include <Game/Data/Enemy.hpp>
#include <Game/Data/Item.hpp>
#include <Game/Data/Map.hpp>
#include <Game/Data/NPC.hpp>

// 一切数据的存储器
class Data {
public:
    std::map <int, Actor> actors; // 角色
    std::map <int, Animation> animations; // 动画
    std::map <int, Element> elements; // 属性
    std::map <int, Enemy> enemies; // 敌人
    std::map <int, Item> items; // 道具
    std::map <int, Map> maps; // 地图
    std::map <int, NPC> npc; // npc
    std::map <int, std::string> motaName; // 魔塔名

    // 初始化
    void init();
    // 搜索地图
    int searchMap(const std::string& mapname);
}; extern Data motaData;
