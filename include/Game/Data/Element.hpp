#pragma once
#include <Game/stdafx.hpp>

// 属性名字和描述
class Element {
public:
    string name, description; // 属性名字，描述
    bool haveBuff; // 是否有buff
    // 反序列化至数据
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Element, name, description)
};
