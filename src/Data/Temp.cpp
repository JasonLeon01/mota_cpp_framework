#include <Game/Data/Temp.hpp>

Temp motaTemp;

void Temp::init() {
    // 初始化数据
    battleEnemyID = -1;
    shopType = -1;
    functionEventID = -1;
    closeMS = false;
    directlyFunction = false;
    gameOver = false;
    ending = false;
    pause = false;
    initPrice.clear();
    rise = "";
    transEventName = "";
    messageInfo.clear();
    floorEnemies.clear();
}
