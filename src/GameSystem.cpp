#include <Game/GameSystem.h>

#include <utility>

System motaSystem;
GameKeyBoard motaKeyBoard;
GameGraphics motaGraphics;
Texture tempTexture;
queue <tuple <SoundBuffer*, Sound*, bool> > SE;

void playSE(const string& file) {
    if (!filesystem::exists("sound\\" + file)) {
        print(format("Haven't found file: {}", file));
        return;
    }
    SoundBuffer* buffer = new SoundBuffer;
    Sound* se = new Sound;
    buffer->loadFromFile("sound\\" + file);
    se->setBuffer(*buffer);
    se->setVolume(motaSystem.SEVolume);
    SE.emplace(buffer, se, false);
}

void drawText(float x, float y, const string& content, LONG size, bool bond, Color colour, float angle) {
    Text temptxt(str2wstr(content), motaSystem.font, size);
    temptxt.setFillColor(colour);
    temptxt.setPosition(x * motaSystem.resolutionRatio, y * motaSystem.resolutionRatio);
    temptxt.setScale(motaSystem.resolutionRatio, motaSystem.resolutionRatio);
    if (bond) temptxt.setStyle(Text::Bold);
    temptxt.setRotation(angle);
    motaSystem.window.draw(temptxt);
}

void drawText(IntRect rect, const string& content, int pos, LONG size, bool bond, Color colour, float angle) {
    Text temptxt(str2wstr(content), motaSystem.font, size);
    temptxt.setFillColor(colour);
    if (bond) temptxt.setStyle(Text::Bold);
    temptxt.setRotation(angle);
    auto txtsize = temptxt.getGlobalBounds();
    auto [wx, wy] = make_pair(rect.left, rect.top);
    pos = max(min(pos, 2), 0);
    wx += (rect.width - txtsize.width) / 2 * pos;
    wy += (rect.height - txtsize.height) / 2 * (pos == 1);
    temptxt.setScale(motaSystem.resolutionRatio, motaSystem.resolutionRatio);
    temptxt.setPosition(wx * motaSystem.resolutionRatio, wy * motaSystem.resolutionRatio);
    motaSystem.window.draw(temptxt);
}

void drawImage(const string& file, float x, float y, int opacity, pair <float, float> scale) {
    Sprite tempspr(motaSystem.textureCache[file]);
    tempspr.setPosition(x * motaSystem.resolutionRatio, y * motaSystem.resolutionRatio);
    tempspr.setScale(motaSystem.resolutionRatio * scale.first, motaSystem.resolutionRatio * scale.second);
    auto cl = tempspr.getColor();
    cl.a = opacity;
    tempspr.setColor(cl);
    motaSystem.window.draw(tempspr);
}

void drawImage(const string& file, float x, float y, IntRect rect, int opacity, pair <float, float> scale) {
    Sprite tempspr(motaSystem.textureCache[file]);
    tempspr.setTextureRect(IntRect(rect.left, rect.top, rect.width, rect.height));
    tempspr.setPosition(x * motaSystem.resolutionRatio, y * motaSystem.resolutionRatio);
    tempspr.setScale(motaSystem.resolutionRatio * scale.first, motaSystem.resolutionRatio * scale.second);
    auto cl = tempspr.getColor();
    cl.a = opacity;
    tempspr.setColor(cl);
    motaSystem.window.draw(tempspr);
}

void drawOutterImage(const string& file, float x, float y, IntRect rect, int opacity, pair <float, float> scale) {
    tempTexture.loadFromFile(file);
    Sprite tempspr(tempTexture);
    tempspr.setTextureRect(IntRect(rect.left, rect.top, rect.width, rect.height));
    tempspr.setPosition(x * motaSystem.resolutionRatio, y * motaSystem.resolutionRatio);
    tempspr.setScale(motaSystem.resolutionRatio * scale.first, motaSystem.resolutionRatio * scale.second);
    auto cl = tempspr.getColor();
    cl.a = opacity;
    tempspr.setColor(cl);
    motaSystem.window.draw(tempspr);
}

GameImage::GameImage() {
    this->x = 0;
    this->y = 0;
    this->z = 0;
    this->width = 0;
    this->height = 0;
    this->sx = 0;
    this->sy = 0;
    this->origin_x = 0;
    this->origin_y = 0;
    this->scale_x = 1.f;
    this->scale_y = 1.f;
    this->angle = 0;
    this->opacity = 255;
    this->visible = true;
    this->imgFile = "";
}

GameImage::GameImage(string file, float x, float y, int width, int height, int sx, int sy) : GameImage() {
    this->x = x;
    this->y = y;
    this->z = 0;
    this->width = width;
    this->height = height;
    this->sx = sx;
    this->sy = sy;
    if (!filesystem::exists("graphics\\" + file)) {
        print(format("Haven't found file: {}", file));
        return;
    }
    motaGraphics.addImage(this);
    this->imgFile = file;
}

void GameImage::setSprite(string file, float x, float y, int width, int height, int sx, int sy) {
    this->x = x;
    this->y = y;
    this->z = 0;
    this->width = width;
    this->height = height;
    this->sx = sx;
    this->sy = sy;
    if (!filesystem::exists("graphics\\" + file)) {
        print(format("Haven't found file: {}", file));
        return;
    }
    if (this->imgFile.empty()) motaGraphics.addImage(this);
    this->imgFile = file;
}

void GameImage::dispose() {
    motaGraphics.eraseImage(this);
}

void GameImage::show() {
    Sprite sprite(motaSystem.textureCache[imgFile]);
    if (width != 0 && height != 0) sprite.setTextureRect(IntRect(sx, sy, width, height));
    sprite.setPosition(x * motaSystem.resolutionRatio, y * motaSystem.resolutionRatio);
    sprite.setScale(scale_x * motaSystem.resolutionRatio, scale_y * motaSystem.resolutionRatio);
    sprite.setOrigin(origin_x * motaSystem.resolutionRatio, origin_y * motaSystem.resolutionRatio);
    sprite.setRotation(angle);
    auto cl = sprite.getColor();
    cl.a = opacity;
    sprite.setColor(cl);
    motaSystem.window.draw(sprite);
}

GameText::GameText() {
    this->x = 0;
    this->y = 0;
    this->z = 0;
    this->txt = "";
    this->text = Text();
}

GameText::GameText(string txt, float x, float y) : GameText() {
    this->x = x;
    this->y = y;
    this->text.setString(str2wstr(txt));
    this->text.setFont(motaSystem.font);
    this->text.setCharacterSize(20L);
    this->text.setPosition(x * motaSystem.resolutionRatio, y * motaSystem.resolutionRatio);
    this->text.setScale(motaSystem.resolutionRatio, motaSystem.resolutionRatio);
    motaGraphics.addText(this);
    this->txt = std::move(txt);
}

void GameText::setText(string txt, float x, float y) {
    this->x = x;
    this->y = y;
    this->text.setString(str2wstr(txt));
    this->text.setFont(motaSystem.font);
    this->text.setCharacterSize(20L);
    this->text.setPosition(x * motaSystem.resolutionRatio, y * motaSystem.resolutionRatio);
    this->text.setScale(motaSystem.resolutionRatio, motaSystem.resolutionRatio);
    if (this->txt.empty()) motaGraphics.addText(this);
    this->txt = std::move(txt);
}

pair<float, float> GameText::getSize() {
    auto txtsize = text.getGlobalBounds();
    return make_pair(txtsize.width / motaSystem.resolutionRatio, txtsize.height / motaSystem.resolutionRatio);
}

void GameText::dispose() {
    motaGraphics.eraseText(this);
}

void GameText::show() {
    this->text.setPosition(x * motaSystem.resolutionRatio, y * motaSystem.resolutionRatio);
    motaSystem.window.draw(text);
}

void System::init() {
    auto processing = [](const wstring& content) {
        Text temptxt(content, motaSystem.font, 28);
        temptxt.setPosition(160 * motaSystem.resolutionRatio, 400 * motaSystem.resolutionRatio);
        motaSystem.window.clear();
        motaSystem.window.draw(temptxt);
        motaSystem.window.display();
    };
    auto processingAssets = [](const string& subroot, const string& fext) {
        string inPath = format("graphics\\{}", subroot);
        for (const auto& entry : filesystem::recursive_directory_iterator(inPath))
            if (entry.is_regular_file() && entry.path().extension() == fext)
                motaSystem.textureCache[string(subroot + "\\") + entry.path().filename().string()].loadFromFile(format("graphics\\{}\\{}", subroot, entry.path().filename().string()));
    };

	auto mainSet = readData("ref\\main.ini", "=");
	auto configSet = readData("ref\\config.ini", "=");

	windowskinName = "system\\" + mainSet["windowskinName"];
	titleName = "system\\" + mainSet["titleName"];
	fontName = mainSet["fontName"];
	titleBGM = mainSet["titleBGM"];
	cursorSE = mainSet["cursorSE"];
	decisionSE = mainSet["decisionSE"];
	cancelSE = mainSet["cancelSE"];
	buzzerSE = mainSet["buzzerSE"];
	shopSE = mainSet["shopSE"];
	saveSE = mainSet["saveSE"];
	loadSE = mainSet["loadSE"];
	gateSE = mainSet["gateSE"];
	stairSE = mainSet["stairSE"];
	getSE = mainSet["getSE"];
	windowOpacity = stoi(mainSet["windowOpacity"]);

	resolutionRatio = stof(configSet["resolutionRatio"]);
	frameRate = stoi(configSet["frameRate"]);
	BGMVolume = stoi(configSet["BGMVolume"]);
	SEVolume = stoi(configSet["SEVolume"]);

    window.create(VideoMode(640 * resolutionRatio, 480 * resolutionRatio), "Mota", sf::Style::Titlebar | sf::Style::Close);
    font.loadFromFile("font\\" + fontName);

    processing(L"正在加载动画资源...");
    processingAssets("animation", ".png");

    processing(L"正在加载行走图资源...");
    processingAssets("character", ".png");

    processing(L"正在加载系统资源...");
    processingAssets("system", ".png");

}

void System::bgmSwitch(const string& file) {
    bgm.stop();
    bgm.openFromFile("sound\\" + file);
    bgm.setLoop(true);
    bgm.setVolume(BGMVolume);
    bgm.play();
}

bool GameKeyBoard::press(int key) {
    if ((GetAsyncKeyState(key) & 0x8000) != 0) {
        ++hit_times;
        return true;
    }
    return false;
}

bool GameKeyBoard::repeat(int key) {
    int result = GetAsyncKeyState(key) & 0x8000;
    if (result != 0) {
        ++hit_times;
        if (key_repeat.find(key) == key_repeat.end()) {
            key_repeat[key] = 0;
            return true;
        }
        key_repeat[key]++;
    }
    else {
        key_repeat.erase(key);
        key_hash[key] = 0;
    }
    if (!(key_repeat.find(key) == key_repeat.end()) && key_repeat[key] > 2) {
        key_repeat[key] = 0;
        ++hit_times;
        return true;
    }
    else
        return false;
}

bool GameKeyBoard::trigger(int key) {
    int result = GetAsyncKeyState(key) & 0x8000;
    if (key_hash[key] == 1 && result != 0) return false;
    if (result != 0) {
        ++hit_times;
        key_hash[key] = 1;
        return true;
    }
    else {
        key_hash[key] = 0;
        return false;
    }
}

bool GameKeyBoard::pressConfirm() {
    return (press(KeySPACE) || press(KeyRETURN));
}

bool GameKeyBoard::repeatConfirm() {
    return (repeat(KeySPACE) || repeat(KeyRETURN));
}

bool GameKeyBoard::triggerConfirm() {
    return (trigger(KeySPACE) || trigger(KeyRETURN));
}

bool GameKeyBoard::pressCancel() {
    return (press(KeyESCAPE) || press(KeyX));
}

bool GameKeyBoard::repeatCancel() {
    return (repeat(KeyESCAPE) || repeat(KeyX));
}

bool GameKeyBoard::triggerCancel() {
    return (trigger(KeyESCAPE) || trigger(KeyX));
}

bool GameKeyBoard::doubleClick(int key) {
    int result = trigger(key);
    if (result == 0) return false;
    ++hit_times;
    if (double_click[key] == 0) {
        double_click[key] = clock();
        return false;
    }
    else {
        if (clock() - double_click[key] <= 500) {
            double_click[key] = 0;
            return true;
        }
        else {
            double_click[key] = clock();
            return false;
        }
    }
}

int GameKeyBoard::dir4() {
    if (press(KeyDOWN)) return 0;
    if (press(KeyLEFT)) return 1;
    if (press(KeyRIGHT)) return 2;
    if (press(KeyUP)) return 3;
    return -1;
}

GameWindow::GameWindow() {
    this->x = 0;
    this->y = 0;
    this->z = 0;
    this->width = 0;
    this->height = 0;
    this->opacity = motaSystem.windowOpacity;
    this->haveFunction = false;
    this->visible = true;
    windowBackground.loadFromImage(motaSystem.textureCache[motaSystem.windowskinName].copyToImage());
}

GameWindow::GameWindow(IntRect rect, int opacity) : GameWindow() {
    this->x = rect.left;
    this->y = rect.top;
    this->width = rect.width;
    this->height = rect.height;
    this->opacity = opacity;
    this->haveFunction = true;
    motaGraphics.addWindow(this);
}

void GameWindow::drawRect(IntRect rect) {
    --rect.left; --rect.top; rect.width += 2; rect.height += 2;
    drawImage(motaSystem.windowskinName, x + rect.left, y + rect.top, IntRect(128, 64, 1, 32), abs((int)(135 - 15 * (motaSystem.gameTime % 16))) + 105, make_pair(1.f, rect.height / 32.0));
    drawImage(motaSystem.windowskinName, x + rect.left + rect.width - 1, y + rect.top, IntRect(159, 64, 1, 32), abs((int)(135 - 15 * (motaSystem.gameTime % 16))) + 105, make_pair(1.f, rect.height / 32.0));
    drawImage(motaSystem.windowskinName, x + rect.left + 1, y + rect.top, IntRect(129, 64, 30, 1), abs((int)(135 - 15 * (motaSystem.gameTime % 16))) + 105, make_pair((rect.width - 2) / 30.0, 1.f));
    drawImage(motaSystem.windowskinName, x + rect.left + 1, y + rect.top + rect.height - 1, IntRect(129, 95, 30, 1), abs((int)(135 - 15 * (motaSystem.gameTime % 16))) + 105, make_pair((rect.width - 2) / 30.0, 1.f));
    drawImage(motaSystem.windowskinName, x + rect.left + 1, y + rect.top + 1, IntRect(129, 65, 30, 30), abs((int)(135 - 15 * (motaSystem.gameTime % 16))) + 105, make_pair((rect.width - 2) / 30.0, (rect.height - 2) / 30.0));
}

void GameWindow::drawWText(float x, float y, const string& content, LONG size, bool bond, Color colour) const {
    drawText(this->x + x, this->y + y, content, size, bond, colour);
}

void GameWindow::drawWText(IntRect rect, const string& content, int pos, LONG size, bool bond, Color colour) const{
    rect.left += x;
    rect.top += y;
    drawText(rect, content, pos, size, bond, colour);
}

void GameWindow::drawWTextRotate(IntRect rect, const string &content, int pos, LONG size, bool bond, Color colour, float angle) const {
    rect.left += x;
    rect.top += y;
    drawText(rect, content, pos, size, bond, colour, angle);
}

void GameWindow::drawWTextSelect(IntRect rect, const string &content, int pos, LONG size, bool bond, Color colour) const {
    if (rect.left < 0 || rect.top < 0 || rect.left + rect.width > width || rect.top + rect.height > height) return;
    rect.left += x;
    rect.top += y;
    drawText(rect, content, pos, size, bond, colour);
}

void GameWindow::windowBitmap(const string& file, float x, float y, int opacity) const {
    drawImage(file, this->x + x, this->y + y, opacity);
}

void GameWindow::windowBitmap(const string& file, float x, float y, IntRect rect, int opacity) const {
    drawImage(file, this->x + x, this->y + y, rect, opacity);
}

void GameWindow::windowBitmapSelect(const string &file, float x, float y, sf::IntRect rect, int opacity) {
    if (x < 0 || y < 0 || x + rect.width > width || y + rect.height > height) return;
    drawImage(file, this->x + x, this->y + y, rect, opacity);
}

void GameWindow::showArrow(sf::IntRect rect, int nowPage, int maxPage, int type, string pagestr) {
    windowBitmap("system\\arrow.png", rect.left, rect.top, IntRect(16 * ((rect.height >= rect.width) ? (nowPage < maxPage) : (nowPage > 0)), 32 * (rect.height >= rect.width) + 16 * type, 16, 16));
    windowBitmap("system\\arrow.png", rect.left + (rect.width - 16) * (rect.width > rect.height), rect.top + (rect.height - 16) * (rect.height >= rect.width), IntRect(32 + 16 * ((rect.height >= rect.width) ? (nowPage > 0) : (nowPage < maxPage)), 32 * (rect.height >= rect.width) + 16 * type, 16, 16));
    if (!pagestr.empty()) drawWTextRotate(IntRect(rect.left, rect.top, rect.width > rect.height ? rect.width : 16, rect.width > rect.height ? 16 : rect.height), pagestr, 1, 12L, false, Color::White, (90 + 180 * (rect.left < width / 2)) * (rect.height >= rect.width));
}

void GameWindow::dispose() {
    motaGraphics.eraseWindow(this);
}

void GameWindow::showBackground(int x, int y, int width, int height) {
    Sprite wspr(windowBackground);
    wspr.setTextureRect(IntRect(0, 0, 128, 128));
    wspr.setPosition((this->x + x) * motaSystem.resolutionRatio, (this->y + y) * motaSystem.resolutionRatio);
    auto cl = wspr.getColor();
    cl.a = opacity;
    wspr.setColor(cl);
    wspr.setScale(motaSystem.resolutionRatio * width / 128.0, motaSystem.resolutionRatio * height / 128.0);
    motaSystem.window.draw(wspr);
    drawImage(motaSystem.windowskinName, this->x + x, this->y + y, IntRect(128, 0, 16, 16), opacity);
    drawImage(motaSystem.windowskinName, this->x + x + width - 16, this->y + y, IntRect(176, 0, 16, 16), opacity);
    drawImage(motaSystem.windowskinName, this->x + x, this->y + y + height - 16, IntRect(128, 48, 16, 16), opacity);
    drawImage(motaSystem.windowskinName, this->x + x + width - 16, this->y + y + height - 16, IntRect(176, 48, 16, 16), opacity);
    for (int i = 0; i < width - 32; i += 32) {
        drawImage(motaSystem.windowskinName, this->x + x + 16 + i, this->y + y, IntRect(144, 0, min(32, width - 32 - i), 16), opacity);
        drawImage(motaSystem.windowskinName, this->x + x + 16 + i, this->y + y + height - 16, IntRect(144, 48, min(32, width - 32 - i), 16), opacity);
    }
    for (int i = 0; i < height - 32; i += 32) {
        drawImage(motaSystem.windowskinName, this->x + x, this->y + y + 16 + i, IntRect(128, 16, 16, min(32, height - 32 - i)), opacity);
        drawImage(motaSystem.windowskinName, this->x + x + width - 16, this->y + y + 16 + i, IntRect(176, 16, 16, min(32, height - 32 - i)), opacity);
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
    motaGraphics.addWindow(this);
}

void GameSelectWindow::refresh() {
    for (int i = 0; i < items.size(); ++i)
        drawItem(i, Color::White);
    drawRect(IntRect(16, 16 + rectHeight * index, width - 32, rectHeight));
    if (!active) return;
    if (motaKeyBoard.repeat(motaKeyBoard.KeyUP)) {
        if (index > 0) {
            --index;
            playSE(motaSystem.cursorSE);
        }
        return;
    }
    if (motaKeyBoard.repeat(motaKeyBoard.KeyDOWN)) {
        if (index < items.size() - 1) {
            ++index;
            playSE(motaSystem.cursorSE);
        }
        return;
    }
}

void GameSelectWindow::drawItem(int idx, Color colour) {
    drawWText(IntRect(16, 16 + rectHeight * idx, width - 32, rectHeight), items[idx], 1, (LONG)(rectHeight - 10));
}

void GameGraphics::update(bool clear_device) {
    if (clear_device) motaSystem.window.clear();

    for (int z = 0; z <= 9; ++z) {
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

    queue <tuple <SoundBuffer*, Sound*, bool> > temp;
    while (!SE.empty()) {
        auto [_, se, flag] = SE.front();
        SE.pop();
        if (!flag) {
            se->play();
            flag = true;
        }
        if (se->getStatus() == Sound::Stopped) {
            delete _;
            delete se;
        }
        else
            temp.emplace(_, se, flag);
    }
    if (!temp.empty()) swap(temp, SE);

    ++motaSystem.gameTime;
    motaSystem.window.display();
    Sleep(motaSystem.frameRate);
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
