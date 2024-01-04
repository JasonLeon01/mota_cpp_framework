#pragma once
#include <Game/stdafx.hpp>

// 物品数据
class Item {
public:
    std::string name, description, file; // 物品名字，描述，文件名
    int ID, pos[2], price; // 物品编号，位置，价格
    bool usable, cost; // 是否可用，是否消耗
    // 序列化和反序列化的宏
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Item, name, description, file, pos, price, usable, cost)
};
