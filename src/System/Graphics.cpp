#include <Game/System/Graphics.hpp>

GameGraphics motaGraphics;

void GameGraphics::update(bool clear_device) {
    // 需要清屏时则清屏，因为地图渲染和其他场景渲染不同
    if (clear_device) motaSystem.window.clear();

    for (int z = 0; z <= 99; ++z) {
        // 依次渲染图像、窗口、文本
        if (!patterns.empty()) {
            for (auto spr : patterns) {
                if (!spr->visible || spr->z != z) continue;
                spr->show();
            }
        }
        if (!windows.empty()) {
            for (auto win : windows) {
                if (!win->visible || win->z != z) continue;
                win->showBackground(0, 0, win->width, win->height);
                if (win->haveFunction) win->refresh();
            }
        }
        if (!texts.empty()) {
            for (auto txt : texts) {
                if (txt->z != z) continue;
                txt->show();
            }
        }
    }

    // 音效的播放
    std::queue <std::tuple <sf::SoundBuffer*, sf::Sound*, bool> > temp;
    while (!SE.empty()) {
        auto [_, se, flag] = SE.front();
        SE.pop();
        if (!flag) {
            se->play();
            flag = true;
        }
        if (se->getStatus() == sf::Sound::Stopped) {
            delete _;
            delete se;
        }
        else {
            temp.emplace(_, se, flag);
        }
    }
    if (!temp.empty()) swap(temp, SE);

    // 更新游戏时间
    ++motaSystem.gameTime;
    motaSystem.window.display();
}

void GameGraphics::addImage(GameImage *obj) {
    patterns.push_back(obj);
}

void GameGraphics::eraseImage(GameImage *obj) {
    patterns.erase(find(patterns.begin(), patterns.end(), obj));
}

void GameGraphics::addWindow(GameWindow *obj) {
    windows.push_back(obj);
}

void GameGraphics::eraseWindow(GameWindow *obj) {
    windows.erase(find(windows.begin(), windows.end(), obj));
}

void GameGraphics::addText(GameText *obj) {
    texts.push_back(obj);
}

void GameGraphics::eraseText(GameText *obj) {
    texts.erase(find(texts.begin(), texts.end(), obj));
}

void GameGraphics::dispose() {
    motaSystem.window.clear();
}
