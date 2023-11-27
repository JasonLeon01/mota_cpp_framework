#include <Game/System/Text.hpp>

GameText::GameText() {
    this->x = 0;
    this->y = 0;
    this->z = 0;
    this->txt = "";
    this->text = Text();
}

GameText::GameText(Font* font, string txt, float x, float y) : GameText() {
    this->x = x;
    this->y = y;
    this->text.setString(str2wstr(txt));
    this->text.setFont(*font);
    this->text.setCharacterSize(20L);
    this->text.setPosition(x, y);
    this->txt = move(txt);
}

void GameText::setText(Font* font, string txt, float x, float y) {
    this->x = x;
    this->y = y;
    this->text.setString(str2wstr(txt));
    this->text.setFont(*font);
    this->text.setCharacterSize(20L);
    this->text.setPosition(x, y);
    this->txt = move(txt);
}

pair<float, float> GameText::getSize() {
    auto txtsize = text.getGlobalBounds();
    return make_pair(txtsize.width, txtsize.height);
}

void GameText::show() {
    this->text.setPosition(x, y);
    motaSystem.window.draw(text);
}
