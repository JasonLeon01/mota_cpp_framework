#include <Game/Screen/WindowHint.hpp>

WindowHint::WindowHint(int x, int y) {
    // 初始化对象
    this->x = x;
    this->y = y;
    width = 352;
    height = 352;
    visible = false;
    haveFunction = true;
    keys = readFile("ref\\ShortcutKey.txt");
    nowPage = 0;
    maxPage = ceil(keys.size() / 10.f) - 1;
    motaGraphics.addWindow(this);
}

void WindowHint::refresh() {
    // 不可见时返回
    if (!visible) return;

    //循环描绘快捷键提示
    for (int i = 0; i < std::min(10, (int)keys.size() - 10 * nowPage); ++i) {
        drawWText(sf::IntRect(16, 16 + 32 * i, 320, 32), keys[10 * nowPage + i]);
    }

    // 描绘箭头
    showArrow(sf::IntRect(208, 330, 128, 16), nowPage, maxPage, std::format("Page {}/{}", nowPage + 1, maxPage + 1));
    // 按下左键时
    if (Input::repeat(Input::KeyLEFT)) {
        if (nowPage > 0) {
            playSE(motaSystem.cursorSE, motaSystem.SEVolume);
            --nowPage;
        }
        return;
    }

    // 按下右键时
    if (Input::repeat(Input::KeyRIGHT)) {
        if (nowPage < maxPage) {
            playSE(motaSystem.cursorSE, motaSystem.SEVolume);
            ++nowPage;
        }
        return;
    }
}
