#pragma once
#include <Game/stdafx.hpp>

// npc信息
class NPC {
public:
    std::vector <std::tuple <int, std::string, std::string> > npcInfo; // npc信息
    std::string transName; // 转换后的名字
    bool fade, directlyFunction; // 是否淡入淡出，转换后直接触发
};
