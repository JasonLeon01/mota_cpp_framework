#include <Game/Screen/WindowEnemyBook.hpp>

WindowEnemyBook::WindowEnemyBook(int x, int y) {
    // 初始化对象
    this->x = x;
    this->y = y;
    width = 352;
    height = 352;
    visible = false;
    haveFunction = true;
    index = 0;
    nowPage = 0;
    maxPage = 0;
    from = 0;
    motaGraphics.addWindow(this);
}

void WindowEnemyBook::refresh() {
    // 不可见或者无怪物时返回
    if (!visible || motaTemp.floorEnemies.size() == 0) return;

    // 确认最大页数
    maxPage = ceil(motaTemp.floorEnemies.size() / 4.f) - 1;

    // 根据页数描绘选项，每页最多四个怪物
    for (int i = 0; i < std::min(4, (int)motaTemp.floorEnemies.size() - nowPage * 4); ++i) {
        auto en = motaTemp.floorEnemies[nowPage * 4 + i];
        windowBitmap("character\\" + en.file, 48, 16 + 80 * i, sf::IntRect(32 * ((motaSystem.gameTime / (2 * (1 + motaSystem.frameRate))) % 4), 32 * en.pos, 32, 32));
        drawWText(sf::IntRect(16, 48 + 80 * i, 96, 20), split(en.name, "/")[0], 1, 16L);
        std::string ele = "";
        if (en.element.size() > 2) {
            ele = "详细…";
        }
        else {
            for (auto j : en.element) {
                ele += en.getElement(motaData.elements[j]).first + " ";
            }
            ele.pop_back();
        }

        // 描绘各项数值
        auto& act = screenData.actors[motaVariables.variables[0]];
        drawWText(sf::IntRect(16, 64 + 80 * i, 96, 20), ele, 1, 16L);
        drawWText(sf::IntRect(112, 32 + 80 * i, 72, 20), std::format("生命:{}", en.hp), 0, 16L);
        drawWText(sf::IntRect(184, 32 + 80 * i, 72, 20), std::format("攻击:{}", en.atk), 0, 16L);
        drawWText(sf::IntRect(256, 32 + 80 * i, 72, 20), std::format("防御:{}", en.getDef(&act)), 0, 16L);
        drawWText(sf::IntRect(112, 56 + 80 * i, 72, 20), std::format("经验:{}", en.exp), 0, 16L);
        drawWText(sf::IntRect(184, 56 + 80 * i, 72, 20), std::format("金币:{}", en.gold), 0, 16L);
        int dmg = en.getDamage(&act, &motaData.elements);
        drawWText(sf::IntRect(256, 56 + 80 * i, 72, 20), std::format("伤害:{}", dmg == -1 ? "DIE" : std::to_string(dmg)), 0, 16L);
    }

    // 描绘箭头
    showArrow(sf::IntRect(220, 330, 120, 32), nowPage, maxPage, std::format("Page {}/{}", nowPage + 1, maxPage + 1));

    // 描绘选择矩形
    drawRect(sf::IntRect(16, 16 + 80 * index, 320, 80));

    if (auto mousePos = sf::Mouse::getPosition(motaSystem.window); inRange(std::make_pair(mousePos.x / motaSystem.resolutionRatio, mousePos.y / motaSystem.resolutionRatio), std::make_tuple(x + 16, y + 16, width - 32, height - 32))) {
        int focusIndex = (mousePos.y / motaSystem.resolutionRatio - (y + 16)) / 80;
        if (focusIndex >= std::min(4, (int)motaTemp.floorEnemies.size() - nowPage * 4)) {
            focusIndex = std::min(4, (int)motaTemp.floorEnemies.size() - nowPage * 4) - 1;
        }
        if (focusIndex != index) {
            playSE(motaSystem.cursorSE, motaSystem.SEVolume);
        }
        index = focusIndex;
    }

    // 按下左键时
    if (Input::repeat(Input::KeyLEFT)) {
        if (nowPage > 0) {
            playSE(motaSystem.cursorSE, motaSystem.SEVolume);
            index = 0;
            --nowPage;
        }
        return;
    }

    // 按下右键时
    if (Input::repeat(Input::KeyRIGHT)) {
        if (nowPage < maxPage) {
            playSE(motaSystem.cursorSE, motaSystem.SEVolume);
            index = 0;
            ++nowPage;
        }
        return;
    }

    // 按下上键时
    if (Input::repeat(Input::KeyUP) || Input::scrollUp()) {
        if (Input::trigger(Input::KeyUP) || index > 0) {
            playSE(motaSystem.cursorSE, motaSystem.SEVolume);
            index = (index + 3) % std::min(4, (int)motaTemp.floorEnemies.size() - nowPage * 4);
            return;
        }
    }

    // 按下下键时
    if (Input::repeat(Input::KeyDOWN) || Input::scrollDown()) {
        if (int minn = std::min(4, (int)motaTemp.floorEnemies.size() - nowPage * 4); Input::trigger(Input::KeyDOWN) || index < minn - 1) {
            playSE(motaSystem.cursorSE, motaSystem.SEVolume);
            index = (index + 1) % minn;
            return;
        }
    }
}
