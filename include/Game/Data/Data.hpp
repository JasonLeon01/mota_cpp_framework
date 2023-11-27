#pragma once
#include <Game/Data/Animation.hpp>
#include <Game/Data/Enemy.hpp>
#include <Game/Data/Item.hpp>
#include <Game/Data/Map.hpp>
#include <Game/Data/NPC.hpp>

// 一切数据的存储器
class Data {
public:
    map <int, Actor> actors; // 角色
    map <int, Animation> animations; // 动画
    map <int, Element> elements; // 属性
    map <int, Enemy> enemies; // 敌人
    map <int, Item> items; // 道具
    map <int, Map> maps; // 地图
    map <int, NPC> npc; // npc
    map <int, string> motaName; // 魔塔名

    // 初始化
    void init();
    // 搜索地图
    int searchMap(const string& mapname);
}; extern Data motaData;
