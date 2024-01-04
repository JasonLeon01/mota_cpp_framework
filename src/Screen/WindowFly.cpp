#include <Game/Screen/WindowFly.hpp>

WindowFly::WindowFly(int x, int y) {
    // 初始化对象
    this->x = x;
    this->y = y;
    width = 64;
    height = 288;
    visible = false;
    haveFunction = true;
    viewFloorIndex = 0;
    motaGraphics.addWindow(this);
}

void WindowFly::refresh() {
    // 不可见时返回
    if (!visible) return;

    // 绘制箭头
    auto floors = motaVariables.floorRecord[motaVariables.variables[1]];
    showArrow(sf::IntRect(24, 16, 32, 256), viewFloorIndex, floors.size() - 1);
    drawWText(sf::IntRect(16, 64, 32, 160), std::format("{}\n{}\n{}", motaData.motaName[motaVariables.variables[1]], floors[viewFloorIndex], "层"), 1);
    showBackground(64, 0, 288, 288);
    auto gmap = motaData.maps[motaData.searchMap(std::format("{}:{}", motaVariables.variables[1], floors[viewFloorIndex]))];
    screenData.loadMap(gmap.mapID, &gmap);
    screenData.showMap(gmap, x + 76, y + 12, 0.75, false, false);

    // 按下上键时
    if (Input::repeat(Input::KeyUP) || Input::scrollUp()) {
        if (viewFloorIndex < floors.size() - 1) {
            playSE(motaSystem.cursorSE, motaSystem.SEVolume);
            ++viewFloorIndex;
        }
        return;
    }

    // 按下下键时
    if (Input::repeat(Input::KeyDOWN) || Input::scrollDown()) {
        if (viewFloorIndex > 0) {
            playSE(motaSystem.cursorSE, motaSystem.SEVolume);
            --viewFloorIndex;
        }
        return;
    }

    // 按下PgUp键时
    if (Input::repeat(Input::KeyPRIOR)) {
        if (viewFloorIndex < floors.size() - 1) {
            playSE(motaSystem.cursorSE, motaSystem.SEVolume);
            viewFloorIndex = std::min(viewFloorIndex + 5, (int)floors.size() - 1);
        }
        return;
    }

    // 按下PgDn键时
    if (Input::repeat(Input::KeyNEXT)) {
        if (viewFloorIndex > 0) {
            playSE(motaSystem.cursorSE, motaSystem.SEVolume);
            viewFloorIndex = std::max(viewFloorIndex - 5, 0);
        }
        return;
    }
}
