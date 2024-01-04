#include <Game/Screen/WindowLoadSave.hpp>

WindowLoadSave::WindowLoadSave(int x, int y) {
    // 初始化对象
    this->x = x;
    this->y = y;
    width = 128;
    height = 288;
    visible = false;
    haveFunction = true;
    index = 0;
    load = false;
    from = 0;
    hideLines = -1;
    motaGraphics.addWindow(this);
}

void WindowLoadSave::refresh() {
    if (hideLines < 0 || hideLines >= MAXSAVE - 8) {
        hideLines = std::max(0, index - ((height - 32) / 32 - 1));
    }
    if (index == 0 && hideLines != 0) {
        hideLines = 0;
    }
    // 不可见时返回
    if (!visible) return;

    // 描绘选项
    for (int i = 0; i < MAXSAVE; ++i) {
        drawWTextSelect(sf::IntRect(16, 16 + 32 * (i - hideLines), 96, 32), std::format("文件{}", i));
    }

    // 绘制选择矩形
    drawRect(sf::IntRect(16, 16 + 32 * (index - hideLines), 96, 32));

    if (auto mousePos = sf::Mouse::getPosition(motaSystem.window); inRange(std::make_pair(mousePos.x / motaSystem.resolutionRatio, mousePos.y / motaSystem.resolutionRatio), std::make_tuple(x + 16, y + 16, width - 32, height - 32))) {
        int focusIndex = (mousePos.y / motaSystem.resolutionRatio - (y + 16)) / 32;
        if (focusIndex != index - hideLines) {
            playSE(motaSystem.cursorSE, motaSystem.SEVolume);
        }
        index = focusIndex + hideLines;
    }

    // 显示存读档描述窗口
    showBackground(0, -64, width, 64);
    drawWText(sf::IntRect(16, -48, 96, 32), std::format("{}档", load ? "读" : "存"), 1);

    // 显示存档图片窗口
    showBackground(width + 2, -64, 352, 352);
    if (std::filesystem::exists(std::format("save\\save_{}.json", index))) {
        if (std::filesystem::exists(std::format("save\\save_{}.png", index))) {
            drawOutterImage(&motaSystem.window, std::format("save\\save_{}.png", index), x + width + 18, y - 48, sf::IntRect(MAPX, MAPY, 352, 352), 255, std::make_pair(0.9, 0.9));
        }
        else {
            drawWText(sf::IntRect(width + 2, -64, 352, 352), "存档图片文件缺失！", 1, 28L, true, sf::Color::White);
        }
    }
    else {
        drawWText(sf::IntRect(width + 2, -64, 352, 352), "无存档", 1, 28L, true, sf::Color::White);
    }

    // 按下上键时
    if (Input::repeat(Input::KeyUP) || Input::scrollUp()) {
        if (Input::trigger(Input::KeyUP) || index > 0) {
            playSE(motaSystem.cursorSE, motaSystem.SEVolume);
            if (index - hideLines == 0) {
                --hideLines;
            }
            index = (index + MAXSAVE - 1) % MAXSAVE;
        }
        return;
    }

    // 按下下键时
    if (Input::repeat(Input::KeyDOWN) || Input::scrollDown()) {
        if (Input::trigger(Input::KeyDOWN) || index < MAXSAVE - 1) {
            playSE(motaSystem.cursorSE, motaSystem.SEVolume);
            if (index - hideLines == (height - 32) / 32 - 1) {
                ++hideLines;
            }
            index = (index + 1) % MAXSAVE;
        }
        return;
    }

    // 按下PgUp键时
    if (Input::repeat(Input::KeyPRIOR)) {
        if (index > 0) {
            playSE(motaSystem.cursorSE, motaSystem.SEVolume);
            index = std::max(0, index - 8);
            hideLines = -1;
        }
        return;
    }

    // 按下PgDn键时
    if (Input::repeat(Input::KeyNEXT)) {
        if (index < MAXSAVE - 1) {
            playSE(motaSystem.cursorSE, motaSystem.SEVolume);
            index = std::min(MAXSAVE - 1, index + 8);
            hideLines = -1;
        }
        return;
    }
}
