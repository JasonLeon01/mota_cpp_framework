#pragma once
#include <Game/stdafx.hpp>

// 游戏变量的存储
class Variables {
public:
    map <int, int> variables; // 变量
    map <int, vector <int> > floorRecord; // 楼层记录
    map <int, vector <int> > eventRecord; // 记录处理过的事件
    map <int, map <int, string> > transRecord; // 记录变换过名字的事件
    map <int, bool> itemRecord; // 记录获得过的道具

    // 初始化
    void init();
    // 将形如[x]的字符串替换为其对应序号变量
    string replaceToVar(const string& source);
    // 初始化对话字符串，按对话框里面的全部替换
    string initDialogue(const string& source);
    // 序列化和反序列化的宏
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Variables, variables, floorRecord, eventRecord, transRecord, itemRecord)

}; extern Variables motaVariables;
