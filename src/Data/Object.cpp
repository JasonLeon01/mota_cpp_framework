#include <Game/Data/Object.hpp>

Object::Object() {
    // 初始化事件
    ID = 0;
    name = "";
    file = "";
    fill(pos, pos + 2, 0);
    x = 0;
    y = 0;
    fill(triggerCondition, triggerCondition + 3, 0);
    move = false;
    through = false;
    exist = true;
    toDispose = false;
}

Object::Object(string name, int x, int y) : Object() {
    // 初始化带坐标的事件
    this->name = std::move(name);
    this->x = x;
    this->y = y;
}

inline bool Object::operator == (const Object& ev) const {
    // 重载等于号
    return (x == ev.x && y == ev.y && exist);
}
