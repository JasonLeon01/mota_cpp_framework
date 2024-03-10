#include <Game/Screen/WindowShop.hpp>

WindowShop::WindowShop(int x, int y) {
    // 初始化对象
    this->x = x;
    this->y = y;
    width = 256;
    height = 288;
    visible = false;
    haveFunction = true;
    index = 0;
    pos = 0;
    motaGraphics.addWindow(this);
}

void WindowShop::refresh() {
    // 不可见时返回
    if (!visible) return;

    // 描绘行走图
    windowBitmap("character\\" + file, 16, 16, sf::IntRect(32 * ((motaSystem.gameTime / (2 * (1 + motaSystem.frameRate))) % 4), 32 * pos, 32, 32));

    // 描绘商店名称
    drawWText(sf::IntRect(64, 16, 192, 20), name, 0, 22L);

    // 描绘商店描述
    std::string realdesc = insertNewLines(Interpreter::replaceToVar(desc), 28);
    drawWText(sf::IntRect(16, 64, 224, 20), realdesc, 0, 16L);

    // 获取描述行数
    int lines = strIncludeNum(realdesc, "\n");
    int yy = 84 + 20 * lines, yyy = yy;

    // 描绘商品列表
    for (auto [item, condition, order] : items) {
        drawWText(sf::IntRect(16, yyy, 224, 24), item, 1, 16L);
        yyy += 24;
    }

    // 描绘选择矩形
    drawRect(sf::IntRect(16, yy + 24 * index, 224, 24));

    if (auto mousePos = sf::Mouse::getPosition(motaSystem.window); inRange(std::make_pair(mousePos.x / motaSystem.resolutionRatio, mousePos.y / motaSystem.resolutionRatio), std::make_tuple(x + 16, y + yy, width - 32, 24 * items.size()))) {
        int focusIndex = (mousePos.y / motaSystem.resolutionRatio - (y + yy)) / 24;
        if (!(focusIndex < 0 || focusIndex >= items.size())) {
            if (focusIndex != index) {
                playSE(motaSystem.cursorSE, motaSystem.SEVolume);
            }
            index = focusIndex;
        }
    }

    // 按下上键时
    if (Input::repeat(Input::KeyUP) || Input::scrollUp()) {
        if (Input::trigger(Input::KeyUP) || index > 0) {
            playSE(motaSystem.cursorSE, motaSystem.SEVolume);
            index = (index + items.size() - 1) % items.size();
        }
        return;
    }

    // 按下下键时
    if (Input::repeat(Input::KeyDOWN) || Input::scrollDown()) {
        if (Input::trigger(Input::KeyDOWN) || index < items.size() - 1) {
            playSE(motaSystem.cursorSE, motaSystem.SEVolume);
            index = (index + 1) % items.size();
        }
        return;
    }

    // 按下确定键时
    if (Input::triggerConfirm()) {
        auto [require, val] = get<1>(items[index]);
        auto order = get<2>(items[index]);
        if (*require >= stoi(Interpreter::replaceToVar(val))) {
            playSE(motaSystem.shopSE, motaSystem.SEVolume);
            screenData.doOrder(order);
        }
        else {
            playSE(motaSystem.buzzerSE, motaSystem.SEVolume);
        }
        return;
    }
}
