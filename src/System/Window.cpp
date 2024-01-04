#include <Game/System/Window.hpp>

GameWindow::GameWindow() {
    // 初始化窗口的位置和大小
    this->x = 0;
    this->y = 0;
    this->z = 0;
    this->width = 0;
    this->height = 0;
    this->opacity = motaSystem.windowOpacity;
    this->haveFunction = false;
    this->visible = true;
}

GameWindow::GameWindow(sf::IntRect rect, int opacity) : GameWindow() {
    // 初始化窗口的位置和大小
    this->x = rect.left;
    this->y = rect.top;
    this->width = rect.width;
    this->height = rect.height;
    this->opacity = opacity;
    this->haveFunction = true;
}

void GameWindow::drawRect(sf::IntRect rect) {
    --rect.left; --rect.top; rect.width += 2; rect.height += 2;
    drawImage(&motaSystem.window, &motaSystem.textureCache[motaSystem.windowskinName], x + rect.left, y + rect.top, sf::IntRect(128, 64, 1, 32), abs((int)(135 - 15 * ((motaSystem.gameTime / (2 * (1 + motaSystem.frameRate))) % 16))) + 105, std::make_pair(1.f, rect.height / 32.0));
    drawImage(&motaSystem.window, &motaSystem.textureCache[motaSystem.windowskinName], x + rect.left + rect.width - 1, y + rect.top, sf::IntRect(159, 64, 1, 32), abs((int)(135 - 15 * ((motaSystem.gameTime / (2 * (1 + motaSystem.frameRate))) % 16))) + 105, std::make_pair(1.f, rect.height / 32.0));
    drawImage(&motaSystem.window, &motaSystem.textureCache[motaSystem.windowskinName], x + rect.left + 1, y + rect.top, sf::IntRect(129, 64, 30, 1), abs((int)(135 - 15 * ((motaSystem.gameTime / (2 * (1 + motaSystem.frameRate))) % 16))) + 105, std::make_pair((rect.width - 2) / 30.0, 1.f));
    drawImage(&motaSystem.window, &motaSystem.textureCache[motaSystem.windowskinName], x + rect.left + 1, y + rect.top + rect.height - 1, sf::IntRect(129, 95, 30, 1), abs((int)(135 - 15 * ((motaSystem.gameTime / (2 * (1 + motaSystem.frameRate))) % 16))) + 105, std::make_pair((rect.width - 2) / 30.0, 1.f));
    drawImage(&motaSystem.window, &motaSystem.textureCache[motaSystem.windowskinName], x + rect.left + 1, y + rect.top + 1, sf::IntRect(129, 65, 30, 30), abs((int)(135 - 15 * ((motaSystem.gameTime / (2 * (1 + motaSystem.frameRate))) % 16))) + 105, std::make_pair((rect.width - 2) / 30.0, (rect.height - 2) / 30.0));
}

void GameWindow::drawWText(sf::IntRect rect, const std::string& content, int pos, LONG size, bool bond, sf::Color colour) const{
    rect.left += x;
    rect.top += y;
    drawText(&motaSystem.window, &motaSystem.font, rect, content, pos, size, bond, colour);
}

void GameWindow::drawWTextRotate(sf::IntRect rect, const std::string &content, int pos, LONG size, bool bond, sf::Color colour, float angle) const {
    rect.left += x;
    rect.top += y;
    drawText(&motaSystem.window, &motaSystem.font, rect, content, pos, size, bond, colour, angle);
}

void GameWindow::drawWTextSelect(sf::IntRect rect, const std::string &content, int pos, LONG size, bool bond, sf::Color colour) const {
    if (rect.left < 0 || rect.top < 0 || rect.left + rect.width > width || rect.top + rect.height > height) return;
    rect.left += x;
    rect.top += y;
    drawText(&motaSystem.window, &motaSystem.font, rect, content, pos, size, bond, colour);
}

void GameWindow::windowBitmap(const std::string& file, float x, float y, int opacity) const {
    drawImage(&motaSystem.window, &motaSystem.textureCache[file], this->x + x, this->y + y, opacity);
}

void GameWindow::windowBitmap(const std::string& file, float x, float y, sf::IntRect rect, int opacity) const {
    drawImage(&motaSystem.window, &motaSystem.textureCache[file], this->x + x, this->y + y, rect, opacity);
}

void GameWindow::windowBitmapSelect(const std::string& file, float x, float y, sf::IntRect rect, int opacity) {
    if (x < 0 || y < 0 || x + rect.width > width || y + rect.height > height) return;
    drawImage(&motaSystem.window, &motaSystem.textureCache[file], this->x + x, this->y + y, rect, opacity);
}

void GameWindow::showArrow(sf::IntRect rect, int nowPage, int maxPage, std::string pagestr) {
    if (rect.width >= rect.height) {
        // 横向箭头
        if (nowPage > 0) windowBitmap(motaSystem.windowskinName, rect.left, rect.top, sf::IntRect(144, 24, 8, 16));
        if (nowPage < maxPage) windowBitmap(motaSystem.windowskinName, rect.left + (rect.width - 8), rect.top, sf::IntRect(168, 24, 8, 16));
    }
    else {
        // 纵向箭头
        if (nowPage < maxPage) windowBitmap(motaSystem.windowskinName, rect.left, rect.top, sf::IntRect(152, 16, 16, 8));
        if (nowPage > 0) windowBitmap(motaSystem.windowskinName, rect.left, rect.top + (rect.height - 8), sf::IntRect(152, 40, 16, 8));
    }
    if (!pagestr.empty()) drawWTextRotate(sf::IntRect(rect.left, rect.top, rect.width > rect.height ? rect.width : 16, rect.width > rect.height ? 16 : rect.height), pagestr, 1, 12L, false, sf::Color::White, (90 + 180 * (rect.left < width / 2)) * (rect.height >= rect.width));
}

void GameWindow::showBackground(int x, int y, int width, int height) {
    sf::Sprite wspr(motaSystem.textureCache[motaSystem.windowskinName]);
    wspr.setTextureRect(sf::IntRect(0, 0, 128, 128));
    wspr.setPosition(this->x + x, this->y + y);
    auto cl = wspr.getColor();
    cl.a = opacity;
    wspr.setColor(cl);
    wspr.setScale(width / 128.0, height / 128.0);
    motaSystem.window.draw(wspr);
    drawImage(&motaSystem.window, &motaSystem.textureCache[motaSystem.windowskinName], this->x + x, this->y + y, sf::IntRect(128, 0, 16, 16), opacity);
    drawImage(&motaSystem.window, &motaSystem.textureCache[motaSystem.windowskinName], this->x + x + width - 16, this->y + y, sf::IntRect(176, 0, 16, 16), opacity);
    drawImage(&motaSystem.window, &motaSystem.textureCache[motaSystem.windowskinName], this->x + x, this->y + y + height - 16, sf::IntRect(128, 48, 16, 16), opacity);
    drawImage(&motaSystem.window, &motaSystem.textureCache[motaSystem.windowskinName], this->x + x + width - 16, this->y + y + height - 16, sf::IntRect(176, 48, 16, 16), opacity);
    for (int i = 0; i < width - 32; i += 32) {
        drawImage(&motaSystem.window, &motaSystem.textureCache[motaSystem.windowskinName], this->x + x + 16 + i, this->y + y, sf::IntRect(144, 0, std::min(32, width - 32 - i), 16), opacity);
        drawImage(&motaSystem.window, &motaSystem.textureCache[motaSystem.windowskinName], this->x + x + 16 + i, this->y + y + height - 16, sf::IntRect(144, 48, std::min(32, width - 32 - i), 16), opacity);
    }
    for (int i = 0; i < height - 32; i += 32) {
        drawImage(&motaSystem.window, &motaSystem.textureCache[motaSystem.windowskinName], this->x + x, this->y + y + 16 + i, sf::IntRect(128, 16, 16, std::min(32, height - 32 - i)), opacity);
        drawImage(&motaSystem.window, &motaSystem.textureCache[motaSystem.windowskinName], this->x + x + width - 16, this->y + y + 16 + i, sf::IntRect(176, 16, 16, std::min(32, height - 32 - i)), opacity);
    }
}

bool GameWindow::inRange(std::pair <int, int> pos, std::tuple <int, int, int, int> range) {
    return pos.first > get<0>(range) && pos.first < get<0>(range) + get<2>(range) && pos.second > get<1>(range) && pos.second < get<1>(range) + get<3>(range);
}

GameSelectWindow::GameSelectWindow() {
    // 初始化窗口的位置和大小
    index = 0;
    rectHeight = 32;
    active = true;
}

GameSelectWindow::GameSelectWindow(const std::vector <std::string>& item, int rectheight, int width, int height, bool visible) {
    // 初始化窗口的位置和大小
    index = 0;
    hideLines = -1;
    this->width = width;
    this->height = height;
    this->visible = visible;
    if (height == 0) {
        this->height = 32 + rectheight * item.size();
        this->visible = false;
    }
    haveFunction = true;
    active = true;
    rectHeight = rectheight;
    items = item;
}

void GameSelectWindow::refresh() {
    // 刷新隐藏行数
    if (hideLines < 0 || hideLines >= items.size() - (height - 32) / rectHeight) {
        hideLines = std::max(0, index - ((height - 32) / rectHeight - 1));
    }
    if (index == 0 && hideLines != 0) {
        hideLines = 0;
    }

    // 描绘文字
    for (int i = 0; i < items.size(); ++i) {
        drawWTextSelect(sf::IntRect(16, 16 + rectHeight * (i - hideLines), width - 32, rectHeight), items[i], 1, (LONG)(rectHeight - 10));
    }

    // 描绘矩形
    drawRect(sf::IntRect(16, 16 + rectHeight * (index - hideLines), width - 32, rectHeight));

    if (!active) return;

    // 鼠标操作
    if (auto mousePos = sf::Mouse::getPosition(motaSystem.window); inRange(std::make_pair(mousePos.x / motaSystem.resolutionRatio, mousePos.y / motaSystem.resolutionRatio), std::make_tuple(x + 16, y + 16, width - 32, height - 32))) {
        int focusIndex = (mousePos.y / motaSystem.resolutionRatio - (y + 16)) / rectHeight;
        if (focusIndex != index - hideLines) {
            playSE(motaSystem.cursorSE, motaSystem.SEVolume);
        }
        index = focusIndex + hideLines;
    }

    // 键盘操作
    if (Input::repeat(Input::KeyUP) || Input::scrollUp()) {
        if (Input::trigger(Input::KeyUP) || index > 0) {
            if (index - hideLines == 0) {
                --hideLines;
            }
            index = (index + items.size() - 1) % items.size();
            playSE(motaSystem.cursorSE, motaSystem.SEVolume);
        }
        return;
    }
    if (Input::repeat(Input::KeyDOWN) || Input::scrollDown()) {
        if (Input::trigger(Input::KeyDOWN) || index < items.size() - 1) {
            if (index - hideLines == (height - 32) / 32 - 1) {
                ++hideLines;
            }
            index = (index + 1) % items.size();
            playSE(motaSystem.cursorSE, motaSystem.SEVolume);
        }
        return;
    }
}
