#include <Game/Screen/WindowItem.hpp>

WindowItem::WindowItem(int x, int y) {
    // 初始化对象
    this->x = x;
    this->y = y;
    width = 352;
    height = 272;
    visible = false;
    haveFunction = true;
    index = 0;
    hideLines = -1;
    motaGraphics.addWindow(this);
}

void WindowItem::refresh() {
    // 不可见时返回
    if (!visible) return;

    // 获取真实选项，用以确认描绘坐标
    if (hideLines < 0) {
        hideLines = std::max(0, index / 8 - ((height - 32) / 40 - 1));
    }
    if (index == 0 && hideLines != 0) {
        hideLines = 0;
    }

    // 描绘描述框背景
    showBackground(0, 272, 352, 80);

    // 物品列表为空时返回
    if (myItems.empty()) return;

    // 描绘选项
    for (size_t i = 0; i < myItems.size(); ++i) {
        windowBitmapSelect("character\\" + myItems[i].file, 20 + 40 * (i % 8), 20 + 40 * (i / 8 - hideLines), sf::IntRect(32 * myItems[i].pos[0], 32 * myItems[i].pos[1], 32, 32), 255 - 64 * !myItems[i].usable);
        drawWTextSelect(sf::IntRect(20 + 40 * (i % 8), 32 + 40 * (i / 8 - hideLines), 32, 20), myItems[i].cost ? std::to_string(screenData.actors[motaVariables.variables[0]].item[myItems[i].ID]) : "" , 2, 16L);
    }

    // 描绘选择矩形
    drawRect(sf::IntRect(16 + 40 * ((index - 8 * hideLines) % 8), 16 + 40 * ((index - 8 * hideLines) / 8), 40, 40));

    // 鼠标操作
    if (auto mousePos = sf::Mouse::getPosition(motaSystem.window); inRange(std::make_pair(mousePos.x / motaSystem.resolutionRatio, mousePos.y / motaSystem.resolutionRatio), std::make_tuple(x + 16, y + 16, width - 32, height - 32))) {
        int focusX = (mousePos.x / motaSystem.resolutionRatio - (x + 16)) / 40;
        int focusY = (mousePos.y / motaSystem.resolutionRatio - (y + 16)) / 40;
        int focusIndex = focusX + focusY * 8;
        if (!(focusIndex + hideLines * 8 < 0 || focusIndex + hideLines * 8 >= myItems.size())) {
            if (focusIndex != index - hideLines * 8) {
                playSE(motaSystem.cursorSE, motaSystem.SEVolume);
            }
            index = focusIndex + hideLines * 8;
        }
    }

    // 描绘名称和描述
    drawWText(sf::IntRect(16, 288, 320, 32), myItems[index].name, 0, 12L);
    drawWText(sf::IntRect(16, 300, 320, 64), insertNewLines(myItems[index].description, 40), 0, 16L);

    // 按下左键时
    if (Input::repeat(Input::KeyLEFT) || Input::scrollUp()) {
        if (index > 0) {
            if (index - hideLines * 8 == 0) {
                --hideLines;
            }
            playSE(motaSystem.cursorSE, motaSystem.SEVolume);
            --index;
        }
        return;
    }

    // 按下右键时
    if (Input::repeat(Input::KeyRIGHT) || Input::scrollDown()) {
        if (index < myItems.size() - 1) {
            if ((index - hideLines * 8) / 8 == (height - 32) / 40 - 1) {
                ++hideLines;
            }
            playSE(motaSystem.cursorSE, motaSystem.SEVolume);
            ++index;
        }
        return;
    }

    // 按下上键时
    if (Input::repeat(Input::KeyUP)) {
        if (index >= 8) {
            hideLines = -1;
            playSE(motaSystem.cursorSE, motaSystem.SEVolume);
            index -= 8;
        }
        return;
    }

    // 按下下键时
    if (Input::repeat(Input::KeyDOWN)) {
        if (((int)(index / 8) + 1) * 8 <= myItems.size() - 1) {
            hideLines = -1;
            playSE(motaSystem.cursorSE, motaSystem.SEVolume);
            index = std::min(index + 8, (int)myItems.size() - 1);
        }
        return;
    }
}
