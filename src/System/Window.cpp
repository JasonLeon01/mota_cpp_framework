#include <Game/System/Window.hpp>

GameWindow::GameWindow() {
    this->x = 0;
    this->y = 0;
    this->z = 0;
    this->width = 0;
    this->height = 0;
    this->opacity = motaSystem.windowOpacity;
    this->haveFunction = false;
    this->visible = true;
}

GameWindow::GameWindow(IntRect rect, int opacity) : GameWindow() {
    this->x = rect.left;
    this->y = rect.top;
    this->width = rect.width;
    this->height = rect.height;
    this->opacity = opacity;
    this->haveFunction = true;
}

void GameWindow::drawRect(IntRect rect, int gtime) {
    --rect.left; --rect.top; rect.width += 2; rect.height += 2;
    drawImage(&motaSystem.window, &motaSystem.textureCache[motaSystem.windowskinName], x + rect.left, y + rect.top, IntRect(128, 64, 1, 32), abs((int)(135 - 15 * (gtime % 16))) + 105, make_pair(1.f, rect.height / 32.0));
    drawImage(&motaSystem.window, &motaSystem.textureCache[motaSystem.windowskinName], x + rect.left + rect.width - 1, y + rect.top, IntRect(159, 64, 1, 32), abs((int)(135 - 15 * (gtime % 16))) + 105, make_pair(1.f, rect.height / 32.0));
    drawImage(&motaSystem.window, &motaSystem.textureCache[motaSystem.windowskinName], x + rect.left + 1, y + rect.top, IntRect(129, 64, 30, 1), abs((int)(135 - 15 * (gtime % 16))) + 105, make_pair((rect.width - 2) / 30.0, 1.f));
    drawImage(&motaSystem.window, &motaSystem.textureCache[motaSystem.windowskinName], x + rect.left + 1, y + rect.top + rect.height - 1, IntRect(129, 95, 30, 1), abs((int)(135 - 15 * (gtime % 16))) + 105, make_pair((rect.width - 2) / 30.0, 1.f));
    drawImage(&motaSystem.window, &motaSystem.textureCache[motaSystem.windowskinName], x + rect.left + 1, y + rect.top + 1, IntRect(129, 65, 30, 30), abs((int)(135 - 15 * (gtime % 16))) + 105, make_pair((rect.width - 2) / 30.0, (rect.height - 2) / 30.0));
}

void GameWindow::drawWText(IntRect rect, const string& content, int pos, LONG size, bool bond, Color colour) const{
    rect.left += x;
    rect.top += y;
    drawText(&motaSystem.window, &motaSystem.font, rect, content, pos, size, bond, colour);
}

void GameWindow::drawWTextRotate(IntRect rect, const string &content, int pos, LONG size, bool bond, Color colour, float angle) const {
    rect.left += x;
    rect.top += y;
    drawText(&motaSystem.window, &motaSystem.font, rect, content, pos, size, bond, colour, angle);
}

void GameWindow::drawWTextSelect(IntRect rect, const string &content, int pos, LONG size, bool bond, Color colour) const {
    if (rect.left < 0 || rect.top < 0 || rect.left + rect.width > width || rect.top + rect.height > height) return;
    rect.left += x;
    rect.top += y;
    drawText(&motaSystem.window, &motaSystem.font, rect, content, pos, size, bond, colour);
}

void GameWindow::windowBitmap(const string& file, float x, float y, int opacity) const {
    drawImage(&motaSystem.window, &motaSystem.textureCache[file], this->x + x, this->y + y, opacity);
}

void GameWindow::windowBitmap(const string& file, float x, float y, IntRect rect, int opacity) const {
    drawImage(&motaSystem.window, &motaSystem.textureCache[file], this->x + x, this->y + y, rect, opacity);
}

void GameWindow::windowBitmapSelect(const string& file, float x, float y, sf::IntRect rect, int opacity) {
    if (x < 0 || y < 0 || x + rect.width > width || y + rect.height > height) return;
    drawImage(&motaSystem.window, &motaSystem.textureCache[file], this->x + x, this->y + y, rect, opacity);
}

void GameWindow::showArrow(sf::IntRect rect, int nowPage, int maxPage, string pagestr) {
    if (rect.width >= rect.height) {
        // 横向箭头
        if (nowPage > 0) windowBitmap(motaSystem.windowskinName, rect.left, rect.top, IntRect(144, 24, 8, 16));
        if (nowPage < maxPage) windowBitmap(motaSystem.windowskinName, rect.left + (rect.width - 8), rect.top, IntRect(168, 24, 8, 16));
    }
    else {
        // 纵向箭头
        if (nowPage < maxPage) windowBitmap(motaSystem.windowskinName, rect.left, rect.top, IntRect(152, 16, 16, 8));
        if (nowPage > 0) windowBitmap(motaSystem.windowskinName, rect.left, rect.top + (rect.height - 8), IntRect(152, 40, 16, 8));
    }
    if (!pagestr.empty()) drawWTextRotate(IntRect(rect.left, rect.top, rect.width > rect.height ? rect.width : 16, rect.width > rect.height ? 16 : rect.height), pagestr, 1, 12L, false, Color::White, (90 + 180 * (rect.left < width / 2)) * (rect.height >= rect.width));
}

void GameWindow::showBackground(int x, int y, int width, int height) {
    Sprite wspr(motaSystem.textureCache[motaSystem.windowskinName]);
    wspr.setTextureRect(IntRect(0, 0, 128, 128));
    wspr.setPosition(this->x + x, this->y + y);
    auto cl = wspr.getColor();
    cl.a = opacity;
    wspr.setColor(cl);
    wspr.setScale(width / 128.0, height / 128.0);
    motaSystem.window.draw(wspr);
    drawImage(&motaSystem.window, &motaSystem.textureCache[motaSystem.windowskinName], this->x + x, this->y + y, IntRect(128, 0, 16, 16), opacity);
    drawImage(&motaSystem.window, &motaSystem.textureCache[motaSystem.windowskinName], this->x + x + width - 16, this->y + y, IntRect(176, 0, 16, 16), opacity);
    drawImage(&motaSystem.window, &motaSystem.textureCache[motaSystem.windowskinName], this->x + x, this->y + y + height - 16, IntRect(128, 48, 16, 16), opacity);
    drawImage(&motaSystem.window, &motaSystem.textureCache[motaSystem.windowskinName], this->x + x + width - 16, this->y + y + height - 16, IntRect(176, 48, 16, 16), opacity);
    for (int i = 0; i < width - 32; i += 32) {
        drawImage(&motaSystem.window, &motaSystem.textureCache[motaSystem.windowskinName], this->x + x + 16 + i, this->y + y, IntRect(144, 0, min(32, width - 32 - i), 16), opacity);
        drawImage(&motaSystem.window, &motaSystem.textureCache[motaSystem.windowskinName], this->x + x + 16 + i, this->y + y + height - 16, IntRect(144, 48, min(32, width - 32 - i), 16), opacity);
    }
    for (int i = 0; i < height - 32; i += 32) {
        drawImage(&motaSystem.window, &motaSystem.textureCache[motaSystem.windowskinName], this->x + x, this->y + y + 16 + i, IntRect(128, 16, 16, min(32, height - 32 - i)), opacity);
        drawImage(&motaSystem.window, &motaSystem.textureCache[motaSystem.windowskinName], this->x + x + width - 16, this->y + y + 16 + i, IntRect(176, 16, 16, min(32, height - 32 - i)), opacity);
    }
}

GameSelectWindow::GameSelectWindow() {
    index = 0;
    rectHeight = 32;
    active = true;
}

GameSelectWindow::GameSelectWindow(int width, int rectheight, const vector<string>& item, bool visible) {
    index = 0;
    this->width = width;
    this->visible = visible;
    height = 32 + rectheight * item.size();
    haveFunction = true;
    active = true;
    rectHeight = rectheight;
    items = item;
}

void GameSelectWindow::refresh() {
    for (int i = 0; i < items.size(); ++i) {
        drawItem(i, Color::White);
    }
    drawRect(IntRect(16, 16 + rectHeight * index, width - 32, rectHeight), motaSystem.gameTime);
    if (!active) return;
    if (motaKeyBoard.repeat(motaKeyBoard.KeyUP)) {
        if (motaKeyBoard.trigger(motaKeyBoard.KeyUP) || index > 0) {
            index = (index + items.size() - 1) % items.size();
            playSE(motaSystem.cursorSE, motaSystem.SEVolume);
        }
        return;
    }
    if (motaKeyBoard.repeat(motaKeyBoard.KeyDOWN)) {
        if (motaKeyBoard.trigger(motaKeyBoard.KeyDOWN) || index < items.size() - 1) {
            index = (index + 1) % items.size();
            playSE(motaSystem.cursorSE, motaSystem.SEVolume);
        }
        return;
    }
}

void GameSelectWindow::drawItem(int idx, Color colour) {
    drawWText(IntRect(16, 16 + rectHeight * idx, width - 32, rectHeight), items[idx], 1, (LONG)(rectHeight - 10));
}
