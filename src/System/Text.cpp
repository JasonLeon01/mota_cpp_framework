#include <Game/System/Text.hpp>

GameText::GameText() {
    // 初始化
    this->x = 0;
    this->y = 0;
    this->z = 0;
    this->txt = "";
    this->text = sf::Text();
}

GameText::GameText(sf::Font* font, std::string txt, float x, float y) : GameText() {
    // 初始化
    this->x = x;
    this->y = y;
    this->text.setString(str2wstr(txt));
    this->text.setFont(*font);
    this->text.setCharacterSize(20L);
    this->text.setPosition(x, y);
    this->txt = move(txt);
}

void GameText::setText(sf::Font* font, std::string txt, float x, float y) {
    // 设置文本
    this->x = x;
    this->y = y;
    this->text.setString(str2wstr(txt));
    this->text.setFont(*font);
    this->text.setCharacterSize(20L);
    this->text.setPosition(x, y);
    this->txt = move(txt);
}

std::pair<float, float> GameText::getSize() {
    // 获取文本大小
    auto txtsize = text.getGlobalBounds();
    return std::make_pair(txtsize.width, txtsize.height);
}

void GameText::show() {
    // 显示文本
    this->text.setPosition(x, y);
    motaSystem.window.draw(text);
}
