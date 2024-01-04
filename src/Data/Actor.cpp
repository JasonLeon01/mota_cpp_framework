#include <Game/Data/Actor.hpp>

Actor::Actor() {
    // 初始化
    name = file = "";
    status.clear();
    level = hp = atk = def = mdef = exp = gold = mapID = x = y = animationID = 0;
    item.clear();
}

int Actor::getAtk() const {
    // 加以衰弱影响
    return std::max(0, atk - motaVariables.variables[6]);
}

int Actor::getDef() const {
    // 加以衰弱影响
    return std::max(0, def - motaVariables.variables[6]);
}
