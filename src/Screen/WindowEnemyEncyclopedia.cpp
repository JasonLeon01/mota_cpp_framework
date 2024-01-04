#include <Game/Screen/WindowEnemyEncyclopedia.hpp>

WindowEnemyEncyclopedia::WindowEnemyEncyclopedia(int x, int y) {
    // 初始化对象
    this->x = x;
    this->y = y;
    width = 352;
    height = 352;
    visible = false;
    haveFunction = true;
    motaGraphics.addWindow(this);
}

void WindowEnemyEncyclopedia::refresh() {
    // 不可见时返回
    if (!visible) return;

    // 描绘怪物信息
    auto& act = screenData.actors[motaVariables.variables[0]];
    windowBitmap("character\\" + mon.file, 160, 16, sf::IntRect(32 * ((motaSystem.gameTime / (2 * (1 + motaSystem.frameRate))) % 4), 32 * mon.pos, 32, 32));
    drawWText(sf::IntRect(16, 48, 320, 32), split(mon.name, "/")[0], 1, 24L);
    drawWText(sf::IntRect(32, 96, 96, 20), std::format("生命:{}", mon.hp));
    drawWText(sf::IntRect(128, 96, 96, 20), std::format("攻击:{}", mon.atk));
    drawWText(sf::IntRect(224, 96, 96, 20), std::format("防御:{}", mon.getDef(&act)));
    drawWText(sf::IntRect(32, 120, 96, 20), std::format("经验:{}", mon.exp));
    drawWText(sf::IntRect(128, 120, 96, 20), std::format("金币:{}", mon.gold));
    drawWText(sf::IntRect(224, 120, 96, 20), std::format("伤害:{}", mon.getDamage(&act, &motaData.elements) == -1 ? "DIE" : std::to_string(mon.getDamage(&act, &motaData.elements))));
    drawWText(sf::IntRect(32, 144, 96, 20), std::format("临界:{}", mon.getCrisis(&act) == -1 ? "？" : std::to_string(mon.getCrisis(&act))));

    // 描绘属性信息
    for (int i = 0, cnt = 1, yy = 192; i < mon.element.size(); ++i) {
        std::string buff = ""; // 有特殊BUFF的怪物描述
        float buff2 = 0.f; // 特殊BUFF数值描述
        if (motaData.elements[mon.element[i]].haveBuff) {
            // 有BUFF的话，将其替换为相应数值，如果是小数，则乘以100加上百分号
            buff2 = stof(split(mon.name, "/")[cnt++]);
            if (buff2 != (int)buff2) buff = std::to_string((int)(buff2 * 100)) + "%";
            else buff = std::to_string((int)(buff2));
        }

        // 描绘怪物属性
        drawWText(sf::IntRect(16, yy, 96, 20), mon.getElement(motaData.elements[mon.element[i]], buff).first, 0, 18L);
        std::string desc = insertNewLines(mon.getElement(motaData.elements[mon.element[i]], buff).second, 28);
        drawWText(sf::IntRect(64, yy, 256, 20), desc, 0, 18L);

        // 根据属性描绘行数切换y坐标
        yy += 20 * strIncludeNum(desc, "\n") + 24;
    }
}
