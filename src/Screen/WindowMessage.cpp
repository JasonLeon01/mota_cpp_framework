#include <Game/Screen/WindowMessage.hpp>

WindowMessage::WindowMessage(int x, int y) {
    // 初始化对象
    this->x = x;
    this->y = y;
    width = 320;
    height = 176;
    opacity = 0;
    visible = false;
    haveFunction = true;
    haveIndex = false;
    index = -1;
    maxIndex = 0;
    motaGraphics.addWindow(this);
}

void WindowMessage::refresh() {
    // 没有消息或不可见时返回
    if (motaTemp.messageInfo.empty() || !visible) return;

    // 获取消息信息
    auto [id, name, message] = *motaTemp.messageInfo.begin();

    // 默认没有选项
    haveIndex = false;
    maxIndex = 0;
    order.clear();
    int starty = 0;
    int itmid = 0;
    height = 176;

    // 根据ID确认对话窗口位置
    if (id == -2) {
        id = motaTemp.functionEventID;
    }
    if (id == -1) {
        y = MAPY + 160;
    }
    else if (id == -3) {
        y = MAPY + 88;
    }
    else if (id == -4) {
        itmid = stoi(name);
        name = motaData.items[itmid].name;
        message = insertNewLines(motaData.items[itmid].description, 28);
        y = MAPY + 88;
    }
    else y = MAPY + 16;

    // 不透明度大于0时（防止切换位置闪烁）
    if (opacity > 0) {
        int yy = 16;

        // 获取对话信息
        auto messagesplt = split(message, "\n");
        if (messagesplt.size() > 4) {
            height = 176 + 24 * (messagesplt.size() - 4);
            if (y + height > 480) {
                y = 480 - height;
                if (y < 0) y = 0;
            }
        }

        // 当有名字时显示头像和窗口
        if (!name.empty()) {
            // 显示窗口
            showBackground(8, 8, 48, 48);

            // 显示头像
            if (id == -1) {
                windowBitmap("character\\" + screenData.actors[motaVariables.variables[1]].file, 16, 16, sf::IntRect(0, 0, 32, 32));
            }
            else if (id == -4) {
                auto itm = motaData.items[itmid];
                windowBitmap("character\\" + motaData.items[itmid].file, 16, 16, sf::IntRect(32 * itm.pos[0], 32 * itm.pos[1], 32, 32));
            }
            else if (id == -3) {
                auto itm = motaData.items[stoi(split(screenData.visualMap.mapEvents[motaTemp.functionEventID].name, "/")[1])];
                windowBitmap("character\\" + itm.file, 16, 16, sf::IntRect(32 * itm.pos[0], 32 * itm.pos[1], 32, 32));
            }
            else {
                auto ev = screenData.visualMap.mapEvents[motaTemp.functionEventID];
                windowBitmap("character\\" + ev.file, 16, 16, sf::IntRect(32 * ((ev.pos[0] + ev.move * motaSystem.gameTime / (2 * (1 + motaSystem.frameRate))) % 4), 32 * ev.pos[1], 32, 32));
            }
            yy += 48;
        }

        // 显示名字
        drawWText(sf::IntRect(16, 16, 304, 20), name, 1, 24L, true);

        // 显示对话信息
        for (auto msg : messagesplt) {
            // 包含选择项
            if (strInclude(msg, "[s]")) {
                haveIndex = true;
                if (index == -1) index = 0;
                if (starty == 0) starty = yy;
                auto splt = split(msg, "=>");
                auto idxContent = splt[0]; splt.erase(splt.begin());
                replaceAll(idxContent, "[s]", "");
                std::tie(msg, order[maxIndex++]) = std::make_pair(idxContent, splt.size() == 0 ? std::vector <std::string> ({}) : splt);
            }

            // 仅保留有效部分
            drawWText(sf::IntRect(16, yy, 288, 96), msg, 0, 20L);

            // 切换行
            yy += 24;
        }

        // 如果有选择项
        if (haveIndex) {
            drawRect(sf::IntRect(16, starty + 24 * index, 288, 24));

            if (auto mousePos = sf::Mouse::getPosition(motaSystem.window); inRange(std::make_pair(mousePos.x / motaSystem.resolutionRatio, mousePos.y / motaSystem.resolutionRatio), std::make_tuple(x + 16, y + 16 + starty, width - 32, 24 * maxIndex))) {
                int focusIndex = (mousePos.y / motaSystem.resolutionRatio - (y + 16 + starty)) / 24;
                if (!(focusIndex < 0 || focusIndex >= maxIndex)) {
                    if (focusIndex != index) {
                        playSE(motaSystem.cursorSE, motaSystem.SEVolume);
                    }
                    index = focusIndex;
                }
            }

            // 按下上键时
            if (Input::repeat(Input::KeyUP) || Input::scrollUp()) {
                if (index > 0) {
                    --index;
                    playSE(motaSystem.cursorSE, motaSystem.SEVolume);
                }
                return;
            }

            // 按下下键时
            if (Input::repeat(Input::KeyDOWN) || Input::scrollDown()) {
                if (index < maxIndex - 1) {
                    ++index;
                    playSE(motaSystem.cursorSE, motaSystem.SEVolume);
                }
                return;
            }
        }
    }

    // 在结束后恢复不透明度
    opacity = motaSystem.windowOpacity;
}
