#pragma once
#include <Game/Data/Object.hpp>

// 地图
class Map {
public:
    int mapID; // 地图编号
    string mapName, bgmFile; // 地图名字，BGM文件名
    vector <Object> mapEvents; // 地图事件

    // 判断两地图是否相等
    inline bool operator == (const Map& mp) const;
    // 在(x,y)是否有事件
    bool haveAnEvent(int x, int y);
    // 返回在(x,y)处的事件指针
    Object* EcheckEvent(int x, int y);
    // 判断在(x,y)处是否可通行
    bool passible(int x, int y);
    // 获取在两个同x或同y坐标之间的事件列表
    vector <Object*> getLineEvents(pair <int, int> a, pair <int, int> b);
};
