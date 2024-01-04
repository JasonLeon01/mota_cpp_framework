#include <Game/Data/Map.hpp>

inline bool Map::operator == (const Map &mp) const {
    // 重载等于号，地图名相同即可
    return mapName == mp.mapName;
}

bool Map::haveAnEvent(int x, int y) {
    return std::ranges::any_of(mapEvents, [&](auto ev){
        return (ev.x == x && ev.y == y);
    });
}

Object* Map::EcheckEvent(int x, int y) {
    for (auto& ev : mapEvents) {
        if (ev.x == x && ev.y == y) {
            return &ev;
        }
    }
    return new Object();
}

bool Map::passible(int x, int y) {
    // 地图外的坐标不可通行
    if (x > 10 || x < 0 || y > 10 || y < 0) return false;

    // 有不可穿透的事件时也不可通行
    if (haveAnEvent(x, y)) {
        if (auto ev = EcheckEvent(x, y); ev->exist && !ev->through) {
            // 如果还没出现就可以通行
            if (ev->triggerCondition[0] == 1 && motaVariables.variables[ev->triggerCondition[1]] != ev->triggerCondition[2]) return true;
            return false;
        }
    }
    return true;
}

std::vector <Object*> Map::getLineEvents(std::pair<int, int> a, std::pair<int, int> b) {
    if (a.first != b.first && a.second != b.second) return {};
    std::vector <Object*> result;
    if (a.first == b.first) {
        for (int i = std::min(a.second, b.second); i <= std::max(a.second, b.second); ++i) {
            if (haveAnEvent(a.first, i)) {
                result.push_back(EcheckEvent(a.first, i));
            }
        }
    }
    else {
        for (int i = std::min(a.first, b.first); i <= std::max (a.first, b.first); ++i) {
            if (haveAnEvent(i, a.second)) {
                result.push_back(EcheckEvent(i, a.second));
            }
        }
    }
    return result;
}
