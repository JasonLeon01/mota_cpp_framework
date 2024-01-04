#pragma once
#include <Game/System/Graphics.hpp>
#include <Game/Data/Temp.hpp>
#include <Game/Data/NPC.hpp>

// 地图对象
class Object {
public:
    std::string name, file; // 事件名字，文件名
    int ID, pos[2], x, y, triggerCondition[3]; // 事件编号，位置，出现触发条件
    bool move, through, exist, toDispose; // 是否可移动，是否可穿透，是否存在，是否消除
    void* refer; // 引用

    // 构造函数，初始化对象
    Object();
    Object(std::string name, int x = 0, int y = 0);
    // 比较，用于find等函数
    inline bool operator == (const Object& ev) const;
    // 序列化和反序列化的宏
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Object, ID, name, file, pos, x, y, triggerCondition, move, through)
};
