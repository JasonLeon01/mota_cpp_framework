#include <Game/GameSystem.h>
#include <Game/GameData.h>

// 对话信息窗口
class WindowMessage :public GameWindow {
public:
    bool haveIndex; // 是否有选项
    int index; // 有选项对话的选项
    map <int, vector<string> > order; // 指令

    // 构造函数，初始化对象
    WindowMessage(int x, int y);
    // 刷新
    void refresh() override;
private:
    int maxIndex;
};

// 快捷键提示窗口
class WindowHint :public GameWindow {
public:
    int nowPage; // 当前页数

    // 构造函数，初始化对象
    WindowHint(int x, int y);
    // 刷新
    void refresh() override;
private:
    vector <string> keys; // 存储快捷键描述的变量
    int maxPage; // 最大页数
};

// 商店窗口
class WindowShop :public GameWindow {
public:
    string name, file, desc; // 商店名称、商店行走图文件名、商店描述
    int index, pos; // 当前选择项、当前位置
    vector <tuple <string, pair <int*, string>, vector <string> > > items; // 商品列表，tuple三个分别为商店选项描述、(需要的项（比如金币）,要求)、(购买后触发的事件)

    // 构造函数，初始化对象
    WindowShop(int x, int y);
    // 刷新
    void refresh() override;
};

// 物品窗口
class WindowItem :public GameWindow {
public:
    int index; // 当前选择项
    vector <GameItem> myItems; // 物品列表

    // 构造函数，初始化对象
    WindowItem(int x, int y);
    // 刷新
    void refresh() override;
};

// 怪物手册窗口
class WindowEnemyBook :public GameWindow {
public:
    int index, nowPage, from; // 当前选择项、当前页数、来源（地图或菜单）

    // 构造函数，初始化对象
    WindowEnemyBook(int x, int y);
    // 刷新
    void refresh() override;
private:
    int maxPage;
};

// 怪物百科窗口
class WindowEnemyEncyclopedia :public GameWindow {
public:
    GameEnemy mon; // 怪物信息

    // 构造函数，初始化对象
    WindowEnemyEncyclopedia(int x, int y);
    // 刷新
    void refresh() override;
};

// 楼层传送器窗口
class WindowFly :public GameWindow {
public:
    int viewFloorIndex; // 当前查看的楼层ID

    // 构造函数，初始化对象
    WindowFly(int x, int y);
    // 刷新
    void refresh() override;
};

// 存读档窗口
class WindowLoadSave :public GameWindow {
public:
    int index, from; // 当前选择项、来源（地图或菜单）
    bool load; // 是否是读档

    // 构造函数，初始化对象
    WindowLoadSave(int x, int y);
    // 刷新
    void refresh() override;
private:
    Texture savTexture; // 存储存档截图的纹理
};

// 处理标题画面
class MotaTitle :public SceneBase {
public:
    // 主函数
    void main() override;
    // 更新
    void update() override;
private:
    GameImage spr; // 标题背景精灵
    GameSelectWindow slcWindow = GameSelectWindow(240, 36, {"新游戏", "继续", "退出"}, false); // 选择命令窗口
    WindowLoadSave loadSaveWindow = WindowLoadSave(MAPX - 130, MAPY + 64); // 存读档窗口
};

// 处理地图画面
class MotaMap :public SceneBase {
public:
    // 主函数
    void main() override;
    // 更新
    void update() override;
private:
    string bgmFile; // BGM文件名
    Image saveImage; // 画面截图
    GameSelectWindow menuWindow = GameSelectWindow(128, 32, {"物品道具", "存档", "读档", "回到标题"}, false); // 菜单窗口
    WindowMessage messageWindow = WindowMessage(MAPX + 16, MAPY + 176); // 对话显示窗口
    WindowHint hintWindow = WindowHint(MAPX, MAPY); // 快捷键提示窗口
    WindowShop shopWindow = WindowShop(MAPX + 48, MAPY + 32); // 商店窗口
    WindowItem itemWindow = WindowItem(MAPX, MAPY); // 物品窗口
    WindowEnemyBook enemyBookWindow = WindowEnemyBook(MAPX, MAPY); // 怪物手册窗口
    WindowEnemyEncyclopedia encyWindow = WindowEnemyEncyclopedia(MAPX, MAPY); // 怪物百科窗口
    WindowFly flyWindow = WindowFly(MAPX, MAPY + 32); // 楼层传送器窗口
    WindowLoadSave loadSaveWindow = WindowLoadSave(MAPX - 130, MAPY + 64); // 存读档窗口

    // 快捷键的处理
    void shortcutKey();
    // 使用物品
    void itemUse(int id);
    // 更新菜单
    void updateMenu();
    // 更新对话框
    void updateMessage();
    // 更新快捷键提示
    void updateHint();
    // 更新商店
    void updateShop();
    // 更新物品
    void updateItem();
    // 更新怪物手册
    void updateEnemyBook();
    // 更新怪物百科
    void updateEncyclopedia();
    // 更新楼层传送器
    void updateFly();
    // 更新存读档
    void updateLoadSave();
    // 定义商店信息
    void setShop();
    // 定义战斗部分
    void setBattle();
    // 定义事件名转换部分
    void setTrans();
};

// 处理字幕画面
class MotaScript :public SceneBase {
public:
    // 主函数
    void main() override;
    // 更新
    void update() override;
private:
    GameText text; // 字幕文字
    GameImage back, space; // 字幕背景、空格图标
};

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // 对各类数据结构的初始化
    motaSystem.init();
    motaData.init();
    motaVariables.init();
    motaTemp.init();
    // 清空屏幕，将场景定位到标题界面
    motaSystem.window.clear();
    motaSystem.scene = new MotaTitle;
    // 游戏的主循环
    while (motaSystem.scene != nullptr && motaSystem.window.isOpen()) {
        Event event;
        while (motaSystem.window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                motaSystem.window.close();
        }
        motaSystem.scene->main();
    }
    // 释放图形类型
    screenData.dispose();
    motaGraphics.dispose();
    return 0;
}

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
    if (id == -2) id = motaTemp.functionEventID;
    if (id == -1) y = MAPY + 160;
    else if (id == -3) y = MAPY + 88;
    else if (id == -4) {
        itmid = stoi(name);
        name = motaData.items[itmid].name;
        message = insertNewLines(motaData.items[itmid].description, 42);
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
            if (id == -1)
                windowBitmap("character\\" + screenData.actors[motaVariables.variables[1]].file, 16, 16, IntRect(0, 0, 32, 32));
            else if (id == -4)
                windowBitmap("character\\" + motaData.items[itmid].file, 16, 16, IntRect(32 * motaData.items[itmid].pos[0], 32 * motaData.items[itmid].pos[1], 32, 32));
            else if (id == -3) {
                auto itm = motaData.items[stoi(split(screenData.visualMap.mapEvents[motaTemp.functionEventID].name, "/")[1])];
                windowBitmap("character\\" + itm.file, 16, 16, IntRect(32 * itm.pos[0], 32 * itm.pos[1], 32, 32));
            }
            else {
                auto ev = screenData.visualMap.mapEvents[motaTemp.functionEventID];
                windowBitmap("character\\" + ev.file, 16, 16, IntRect(32 * ((ev.pos[0] + ev.move * motaSystem.gameTime / 4) % 4), 32 * ev.pos[1], 32, 32));
            }
            yy += 48;
        }
        // 显示名字
        drawWText(IntRect(16, 16, 304, 20), name, 1, 24L, true);
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
                tie(msg, order[maxIndex++]) = make_pair(idxContent, splt.size() == 0 ? vector <string> ({}) : splt);
            }
            // 仅保留有效部分
            drawWText(IntRect(16, yy, 288, 96), msg, 0, 20L);
            // 切换行
            yy += 24;
        }
        // 如果有选择项
        if (haveIndex) {
            drawRect(IntRect(16, starty + 24 * index, 288, 24));
            // 按下上键时
            if (motaKeyBoard.repeat(motaKeyBoard.KeyUP)) {
                if (index > 0) {
                    --index;
                    playSE(motaSystem.cursorSE);
                }
                return;
            }
            // 按下下键时
            if (motaKeyBoard.repeat(motaKeyBoard.KeyDOWN)) {
                if (index < maxIndex - 1) {
                    ++index;
                    playSE(motaSystem.cursorSE);
                }
                return;
            }
        }
    }
    // 在结束后恢复不透明度
    opacity = motaSystem.windowOpacity;
}

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
    for (int i = 0; i < min(10, (int)keys.size() - 10 * nowPage); ++i)
        drawWText(IntRect(16, 16 + 32 * i, 320, 32), keys[10 * nowPage + i]);
    // 描绘箭头
    showArrow(IntRect(208, 330, 128, 16), nowPage, maxPage, 0, format("Page {}/{}", nowPage + 1, maxPage + 1));
    // 按下左键时
    if (motaKeyBoard.repeat(motaKeyBoard.KeyLEFT)) {
        if (nowPage > 0) {
            playSE(motaSystem.cursorSE);
            --nowPage;
        }
        return;
    }
    // 按下右键时
    if (motaKeyBoard.repeat(motaKeyBoard.KeyRIGHT)) {
        if (nowPage < maxPage) {
            playSE(motaSystem.cursorSE);
            ++nowPage;
        }
        return;
    }
}

WindowShop::WindowShop(int x, int y) {
    // 初始化对象
    this->x = x;
    this->y = y;
    width = 256;
    height = 288;
    visible = false;
    haveFunction = true;
    index = 0;
    pos = 0;
    motaGraphics.addWindow(this);
}

void WindowShop::refresh() {
    // 不可见时返回
    if (!visible) return;
    // 描绘行走图
    windowBitmap("character\\" + file, 16, 16, IntRect(32 * ((motaSystem.gameTime / 4) % 4), 32 * pos, 32, 32));
    // 描绘商店名称
    drawWText(IntRect(64, 16, 192, 20), name, 0, 22L);
    // 描绘商店描述
    string realdesc = insertNewLines(motaVariables.replaceToVar(desc), 39);
    drawWText(IntRect(16, 64, 224, 20), realdesc, 0, 16L);
    // 获取描述行数
    int lines = strIncludeNum(realdesc, "\n");
    int yy = 84 + 20 * lines, yyy = yy;
    // 描绘商品列表
    for (auto [item, condition, order] : items) {
        drawWText(IntRect(16, yyy, 224, 24), item, 1, 16L);
        yyy += 24;
    }
    // 描绘选择矩形
    drawRect(IntRect(16, yy + 24 * index, 224, 24));
    // 按下上键时
    if (motaKeyBoard.repeat(motaKeyBoard.KeyUP)) {
        if (index > 0) {
            playSE(motaSystem.cursorSE);
            --index;
        }
        return;
    }
    // 按下下键时
    if (motaKeyBoard.repeat(motaKeyBoard.KeyDOWN)) {
        if (index < items.size() - 1) {
            playSE(motaSystem.cursorSE);
            ++index;
        }
        return;
    }
    // 按下确定键时
    if (motaKeyBoard.triggerConfirm()) {
        auto [require, val] = get<1>(items[index]);
        auto order = get<2>(items[index]);
        if (*require >= stoi(motaVariables.replaceToVar(val))) {
            playSE(motaSystem.shopSE);
            screenData.doOrder(order);
        }
        else
            playSE(motaSystem.buzzerSE);
        return;
    }
}

WindowItem::WindowItem(int x, int y) {
    // 初始化对象
    this->x = x;
    this->y = y;
    width = 352;
    height = 256;
    visible = false;
    haveFunction = true;
    index = 0;
    motaGraphics.addWindow(this);
}

void WindowItem::refresh() {
    // 不可见时返回
    if (!visible) return;
    // 获取真实选项，用以确认描绘坐标
    int realIndex = index, hideLines = 0;
    while (realIndex > 40) {
        realIndex -= 8;
        ++hideLines;
    }
    // 描绘描述框背景
    showBackground(0, 256, 352, 96);
    // 物品列表为空时返回
    if (myItems.empty()) return;
    // 描绘选项
    for (size_t i = 0; i < myItems.size(); ++i) {
        windowBitmapSelect("character\\" + myItems[i].file, 20 + 40 * (i % 8), 20 + 40 * (i / 8 - hideLines), IntRect(32 * myItems[i].pos[0], 32 * myItems[i].pos[1], 32, 32), 255 - 64 * !myItems[i].usable);
        drawWTextSelect(IntRect(20 + 40 * (i % 8), 32 + 40 * (i / 8 - hideLines), 32, 20), myItems[i].cost ? to_string(screenData.actors[motaVariables.variables[0]].item[myItems[i].ID]) : "" , 2, 16L);
    }
    // 描绘选择矩形
    drawRect(IntRect(20 + 40 * (realIndex % 8), 20 + 40 * (realIndex / 8), 32, 32));
    // 描绘名称和描述
    drawWText(IntRect(16, 272, 320, 32), myItems[index].name, 0, 20L, false, Color::White);
    drawWText(IntRect(16, 304, 320, 64), insertNewLines(myItems[index].description, 60), 0, 16L, false, Color::White);
    // 按下左键时
    if (motaKeyBoard.repeat(motaKeyBoard.KeyLEFT)) {
        if (index > 0) {
            playSE(motaSystem.cursorSE);
            --index;
        }
        return;
    }
    // 按下右键时
    if (motaKeyBoard.repeat(motaKeyBoard.KeyRIGHT)) {
        if (index < myItems.size() - 1) {
            playSE(motaSystem.cursorSE);
            ++index;
        }
        return;
    }
    // 按下上键时
    if (motaKeyBoard.repeat(motaKeyBoard.KeyUP)) {
        if (index >= 8) {
            playSE(motaSystem.cursorSE);
            index -= 8;
        }
        return;
    }
    // 按下下键时
    if (motaKeyBoard.repeat(motaKeyBoard.KeyDOWN)) {
        if (((int)(index / 8) + 1) * 8 <= myItems.size() - 1) {
            playSE(motaSystem.cursorSE);
            index = min(index + 8, (int)myItems.size() - 1);
        }
        return;
    }
}

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
    for (int i = 0; i < min(4, (int)motaTemp.floorEnemies.size() - nowPage * 4); ++i) {
        auto en = motaTemp.floorEnemies[nowPage * 4 + i];
        windowBitmap("character\\" + en.file, 48, 16 + 80 * i, IntRect(32 * ((motaSystem.gameTime / 4) % 4), 32 * en.pos, 32, 32));
        drawWText(IntRect(16, 48 + 80 * i, 96, 20), split(en.name, "/")[0], 1, 16L);
        string ele = "";
        if (en.element.size() > 2) ele = "详细…";
        else {
            for (auto j : en.element)
                ele += en.getElement(j).first + " ";
            ele.pop_back();
        }
        // 描绘各项数值
        drawWText(IntRect(16, 64 + 80 * i, 96, 20), ele, 1, 16L);
        drawWText(IntRect(112, 32 + 80 * i, 72, 20), format("生命:{}", en.hp), 0, 16L);
        drawWText(IntRect(184, 32 + 80 * i, 72, 20), format("攻击:{}", en.atk), 0, 16L);
        drawWText(IntRect(256, 32 + 80 * i, 72, 20), format("防御:{}", en.getDef()), 0, 16L);
        drawWText(IntRect(112, 56 + 80 * i, 72, 20), format("经验:{}", en.exp), 0, 16L);
        drawWText(IntRect(184, 56 + 80 * i, 72, 20), format("金币:{}", en.gold), 0, 16L);
        drawWText(IntRect(256, 56 + 80 * i, 72, 20), format("伤害:{}", en.getDamage() == -1 ? "DIE" : to_string(en.getDamage())), 0, 16L);
    }
    // 描绘箭头
    showArrow(IntRect(220, 330, 120, 32), nowPage, maxPage, 0, format("Page {}/{}", nowPage + 1, maxPage + 1));
    // 描绘选择矩形
    drawRect(IntRect(16, 16 + 80 * index, 320, 80));
    // 按下左键时
    if (motaKeyBoard.repeat(motaKeyBoard.KeyLEFT)) {
        if (nowPage > 0) {
            playSE(motaSystem.cursorSE);
            index = 0;
            --nowPage;
        }
        return;
    }
    // 按下右键时
    if (motaKeyBoard.repeat(motaKeyBoard.KeyRIGHT)) {
        if (nowPage < maxPage) {
            playSE(motaSystem.cursorSE);
            index = 0;
            ++nowPage;
        }
        return;
    }
    // 按下上键时
    if (motaKeyBoard.repeat(motaKeyBoard.KeyUP)) {
        if (index > 0) {
            playSE(motaSystem.cursorSE);
            --index;
            return;
        }
    }
    // 按下下键时
    if (motaKeyBoard.repeat(motaKeyBoard.KeyDOWN)) {
        if (index < min(4, (int)motaTemp.floorEnemies.size() - nowPage * 4) - 1) {
            playSE(motaSystem.cursorSE);
            ++index;
            return;
        }
    }
}

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
    windowBitmap("character\\" + mon.file, 160, 16, IntRect(32 * ((motaSystem.gameTime / 4) % 4), 32 * mon.pos, 32, 32));
    drawWText(IntRect(16, 48, 320, 32), split(mon.name, "/")[0], 1, 24L);
    drawWText(IntRect(32, 96, 96, 20), format("生命:{}", mon.hp));
    drawWText(IntRect(128, 96, 96, 20), format("攻击:{}", mon.atk));
    drawWText(IntRect(224, 96, 96, 20), format("防御:{}", mon.getDef()));
    drawWText(IntRect(32, 120, 96, 20), format("经验:{}", mon.exp));
    drawWText(IntRect(128, 120, 96, 20), format("金币:{}", mon.gold));
    drawWText(IntRect(224, 120, 96, 20), format("伤害:{}", mon.getDamage() == -1 ? "DIE" : to_string(mon.getDamage())));
    drawWText(IntRect(32, 144, 96, 20), format("临界:{}", mon.getCrisis() == -1 ? "？" : to_string(mon.getCrisis())));
    drawWText(IntRect(128, 144, 96, 20), format("1防减伤:{}", mon.getDamage() - mon.getDamage(screenData.actors[motaVariables.variables[0]].atk, screenData.actors[motaVariables.variables[0]].def + 1)));
    // 描绘属性信息
    for (int i = 0, cnt = 1, yy = 192; i < mon.element.size(); ++i) {
        string buff = ""; // 有特殊BUFF的怪物描述
        float buff2 = 0.f; // 特殊BUFF数值描述
        int lineMax = 51; // 每行最多容纳的汉字个数乘以3
        if (motaData.elements[mon.element[i]].haveBuff) {
            // 有BUFF的话，将其替换为相应数值，如果是小数，则乘以100加上百分号
            buff2 = stof(split(mon.name, "/")[cnt++]);
            if (buff2 != (int)buff2) buff = to_string((int)(buff2 * 100)) + "%";
            else buff = to_string((int)(buff2));
            // 在屏幕上两个半角字符宽度相当于一个全角字符宽度，对其进行修正
            if (int len = 3 - buff.length() % 3; buff.length() % 3 != 0)
                for (int j = 0; j < len; ++j)
                    buff += " ";
            lineMax -= 6 * (buff.length() / 3);
        }
        // 描绘怪物属性
        drawWText(IntRect(16, yy, 96, 20), mon.getElement(mon.element[i], buff).first, 0, 18L);
        string desc = insertNewLines(mon.getElement(mon.element[i], buff).second, lineMax);
        drawWText(IntRect(64, yy, 256, 20), desc, 0, 18L);
        // 根据属性描绘行数切换y坐标
        yy += 20 * strIncludeNum(desc, "\n") + 24;
    }
}

WindowFly::WindowFly(int x, int y) {
    // 初始化对象
    this->x = x;
    this->y = y;
    width = 64;
    height = 288;
    visible = false;
    haveFunction = true;
    viewFloorIndex = 0;
    motaGraphics.addWindow(this);
}

void WindowFly::refresh() {
    // 不可见时返回
    if (!visible) return;
    showArrow(IntRect(24, 16, 32, 256), viewFloorIndex, motaVariables.floorRecord[motaVariables.variables[1]].size() - 1, 1);
    showArrow(IntRect(24, 48, 32, 192), viewFloorIndex, motaVariables.floorRecord[motaVariables.variables[1]].size() - 1);
    drawWText(IntRect(16, 64, 32, 160), format("{}\n{}\n{}", motaData.motaName[motaVariables.variables[1]], motaVariables.floorRecord[motaVariables.variables[1]][viewFloorIndex], "层"), 1);
    showBackground(64, 0, 288, 288);
    auto gmap = motaData.maps[motaData.searchMap(format("{}:{}", motaVariables.variables[1], motaVariables.floorRecord[motaVariables.variables[1]][viewFloorIndex]))];
    screenData.loadMap(gmap.mapID, &gmap);
    screenData.showMap(gmap, x + 76, y + 12, 0.75, false, false);
    // 按下上键时
    if (motaKeyBoard.repeat(motaKeyBoard.KeyUP)) {
        if (viewFloorIndex < motaVariables.floorRecord[motaVariables.variables[1]].size() - 1) {
            playSE(motaSystem.cursorSE);
            ++viewFloorIndex;
        }
        return;
    }
    // 按下下键时
    if (motaKeyBoard.repeat(motaKeyBoard.KeyDOWN)) {
        if (viewFloorIndex > 0) {
            playSE(motaSystem.cursorSE);
            --viewFloorIndex;
        }
        return;
    }
    // 按下PgUp键时
    if (motaKeyBoard.repeat(motaKeyBoard.KeyPRIOR)) {
        if (viewFloorIndex < motaVariables.floorRecord[motaVariables.variables[1]].size() - 1) {
            playSE(motaSystem.cursorSE);
            viewFloorIndex = min(viewFloorIndex + 5, (int)motaVariables.floorRecord[motaVariables.variables[1]].size() - 1);
        }
        return;
    }
    // 按下PgDn键时
    if (motaKeyBoard.repeat(motaKeyBoard.KeyNEXT)) {
        if (viewFloorIndex > 0) {
            playSE(motaSystem.cursorSE);
            viewFloorIndex = max(viewFloorIndex - 5, 0);
        }
        return;
    }
}

WindowLoadSave::WindowLoadSave(int x, int y) {
    // 初始化对象
    this->x = x;
    this->y = y;
    width = 128;
    height = 288;
    visible = false;
    haveFunction = true;
    index = 0;
    load = false;
    from = 0;
    motaGraphics.addWindow(this);
}

void WindowLoadSave::refresh() {
    // 不可见时返回
    if (!visible) return;
    // 获取真实选项
    int realIndex = index, hideLines = 0;
    while (realIndex >= 8) {
        realIndex -= 1;
        ++hideLines;
    }
    // 描绘选项
    for (int i = 0; i < MAXSAVE; ++i)
        drawWTextSelect(IntRect(16, 16 + 32 * (i - hideLines), 96, 32), format("文件{}", i));
    // 绘制选择矩形
    drawRect(IntRect(16, 16 + 32 * realIndex, 96, 32));
    // 显示存读档描述窗口
    showBackground(0, -64, width, 64);
    drawWText(IntRect(16, -48, 96, 32), format("{}档", load ? "读" : "存"), 1);
    // 显示存档图片窗口
    showBackground(width + 2, -64, 352, 352);
    if (filesystem::exists(format("save\\save_{}.sav", index))) {
        if (filesystem::exists(format("save\\save_{}.png", index)))
            drawOutterImage(format("save\\save_{}.png", index), x + width + 18, y - 48, IntRect(MAPX, MAPY, 352, 352), 255, make_pair(0.9, 0.9));
        else
            drawWText(IntRect(width + 2, -64, 352, 352), "存档图片文件缺失！", 1, 28L, true, Color::White);
    }
    else
        drawWText(IntRect(width + 2, -64, 352, 352), "无存档", 1, 28L, true, Color::White);
    // 按下上键时
    if (motaKeyBoard.repeat(motaKeyBoard.KeyUP)) {
        if (index > 0) {
            playSE(motaSystem.cursorSE);
            --index;
        }
        return;
    }
    // 按下下键时
    if (motaKeyBoard.repeat(motaKeyBoard.KeyDOWN)) {
        if (index < MAXSAVE - 1) {
            playSE(motaSystem.cursorSE);
            ++index;
        }
        return;
    }
    // 按下PgUp键时
    if (motaKeyBoard.repeat(motaKeyBoard.KeyPRIOR)) {
        if (index > 0) {
            playSE(motaSystem.cursorSE);
            index = max(0, index - 8);
        }
        return;
    }
    // 按下PgDn键时
    if (motaKeyBoard.repeat(motaKeyBoard.KeyNEXT)) {
        if (index < MAXSAVE - 1) {
            playSE(motaSystem.cursorSE);
            index = min(MAXSAVE - 1, index + 8);
        }
        return;
    }
}

void MotaTitle::main() {
    // 执行渐变，第一个是打开游戏时的渐变，如果opacity=255则不执行
    screenData.transition2();
    // 设置标题画面精灵
    spr.setSprite(motaSystem.titleName);
    // 设置选择命令窗口坐标
    slcWindow.x = (640 - slcWindow.width) / 2;
    slcWindow.y = 448 - slcWindow.height;
    slcWindow.visible = true;
    // 不存在recent文件时创立一个
    if (!filesystem::exists("save\\recent.dat"))
        saveFile("save\\recent.dat", "0");
    else
        slcWindow.index = 1;
    // 切换BGM
    motaSystem.bgmSwitch(motaSystem.titleBGM);
    screenData.transition1();
    // 主循环
    while (motaSystem.scene == this && motaSystem.window.isOpen()) {
        Event event;
        while (motaSystem.window.pollEvent(event))
            if (event.type == Event::Closed)
                motaSystem.window.close();
        // 当聚焦不在本窗口时直接继续
        if (!motaSystem.window.hasFocus()) continue;
        screenData.waitCount(1);
        update();
    }
    // 执行渐变
    screenData.transition2();
    // 消除元素
    spr.dispose();
    slcWindow.dispose();
    loadSaveWindow.dispose();
}

void MotaTitle::update() {
    // 更新存读档窗口
    if (loadSaveWindow.visible) {
        // 按下确认键时
        if (motaKeyBoard.triggerConfirm()) {
            // 如果存档文件存在
            if (filesystem::exists(format("save\\save_{}.sav", loadSaveWindow.index))) {
                playSE(motaSystem.loadSE);
                // 初始化部分数据
                motaVariables.init();
                motaTemp.init();
                screenData.init();
                // 读取存档
                screenData.loadData(loadSaveWindow.index);
                // 关闭存读档窗口
                loadSaveWindow.visible = false;
                // 重新加载地图
                screenData.player.direction = 0;
                screenData.loadMap(screenData.actors[motaVariables.variables[0]].mapID, &screenData.visualMap);
                motaSystem.scene = new MotaMap;
                return;
            }
            else {
                playSE(motaSystem.buzzerSE);
                return;
            }
        }
        // 按下取消键时
        if (motaKeyBoard.triggerCancel()) {
            playSE(motaSystem.cancelSE);
            loadSaveWindow.visible = false;
            slcWindow.visible = true;
            return;
        }
        return;
    }
    // 按下确认键时
    if (motaKeyBoard.triggerConfirm()) {
        if (slcWindow.index == 0) {
            playSE(motaSystem.decisionSE);
            motaVariables.init();
            motaTemp.init();
            screenData.init();
            motaVariables.floorRecord[0].push_back(0);
            screenData.loadMap(screenData.actors[motaVariables.variables[0]].mapID, &screenData.visualMap);
            if (filesystem::exists("ref\\begin.txt")) motaSystem.scene = new MotaScript;
            else motaSystem.scene = new MotaMap;
            return;
        }
        else if (slcWindow.index == 1) {
            playSE(motaSystem.decisionSE);
            loadSaveWindow.index = stoi(readFile("save\\recent.dat")[0]);
            loadSaveWindow.load = true;
            loadSaveWindow.visible = true;
            slcWindow.visible = false;
            return;
        }
        else if (slcWindow.index == 2) {
            playSE(motaSystem.decisionSE);
            motaSystem.scene = nullptr;
        }
    }
}

void MotaMap::main() {
    screenData.onMap = true;
    // 执行渐变
    screenData.transition1();
    // 设置菜单窗口坐标
    menuWindow.x = MAPX - menuWindow.width - 4;
    menuWindow.y = MAPY;
    menuWindow.visible = false;
    // 设置BGM
    bgmFile = screenData.visualMap.bgmFile;
    motaSystem.bgmSwitch(bgmFile);
    // 主循环
    while (motaSystem.scene == this && motaSystem.window.isOpen()) {
        Event event;
        while (motaSystem.window.pollEvent(event))
            if (event.type == Event::Closed)
                motaSystem.window.close();
        // 当聚焦不在本窗口时直接继续
        if (!motaSystem.window.hasFocus()) continue;
        screenData.waitCount(1);
        update();
    }
    // 执行渐变
    screenData.transition2();
    screenData.onMap = false;
    // 消除元素
    menuWindow.dispose();
    messageWindow.dispose();
    hintWindow.dispose();
    shopWindow.dispose();
    itemWindow.dispose();
    enemyBookWindow.dispose();
    encyWindow.dispose();
    flyWindow.dispose();
    loadSaveWindow.dispose();
}

void MotaMap::update() {
    // 当场景已经不在这里时，将不执行任何内容
    if (motaSystem.scene != this) return;
    // 当当前BGM与地图数据的BGM不匹配时切换BGM
    if (bgmFile != screenData.visualMap.bgmFile) {
        bgmFile = screenData.visualMap.bgmFile;
        motaSystem.bgmSwitch(bgmFile);
    }
    // 更新菜单窗口
    if (menuWindow.visible) {
        updateMenu();
        if (menuWindow.active)
            return;
    }
    // 更新物品窗口
    if (itemWindow.visible) {
        updateItem();
        return;
    }
    // 更新存读档窗口
    if (loadSaveWindow.visible) {
        updateLoadSave();
        return;
    }
    // 更新快捷键提示窗口
    if (hintWindow.visible) {
        updateHint();
        return;
    }
    // 当有商店信息时
    if (motaTemp.shopType >= 0)
        setShop();
    // 更新商店窗口
    if (shopWindow.visible) {
        updateShop();
        return;
    }
    // 当消息队列不为空时
    if (!motaTemp.messageInfo.empty()) {
        updateMessage();
        return;
    }
    // 更新怪物手册窗口
    if (enemyBookWindow.visible) {
        updateEnemyBook();
        return;
    }
    // 更新怪物百科窗口
    if (encyWindow.visible) {
        updateEncyclopedia();
        return;
    }
    // 更新楼层传送器窗口
    if (flyWindow.visible) {
        updateFly();
        return;
    }
    // 当有战斗事件时
    if (motaTemp.battleEnemyID != -1) {
        setBattle();
        return;
    }
    // 如果要转换事件名
    if (!motaTemp.transEventName.empty())
        setTrans();
    // 如果事件记录不为空
    if (motaTemp.functionEventID != -1) {
        // 消除事件
        if (screenData.visualMap.mapEvents[motaTemp.functionEventID].toDispose)
            screenData.visualMap.mapEvents[motaTemp.functionEventID].endEvent();
        motaTemp.functionEventID = -1;
        return;
    }
    // 游戏结束则回到标题
    if (motaTemp.gameOver) {
        if (motaTemp.ending && filesystem::exists("ref\\end.txt")) motaSystem.scene = new MotaScript;
        else motaSystem.scene = new MotaTitle;
        return;
    }
    // 结局
    if (motaTemp.ending) {
        if (filesystem::exists("ref\\end.txt")) motaSystem.scene = new MotaScript;
        else motaSystem.scene = new MotaTitle;
        return;
    }
    // 快捷键的判定
    shortcutKey();
    // 更新玩家画面
    screenData.player.update();
}

void MotaMap::shortcutKey() {
    auto capture = [&]() {
        Texture captureImage;
        captureImage.create(motaSystem.window.getSize().x, motaSystem.window.getSize().y);
        captureImage.update(motaSystem.window);
        Sprite captureSprite(captureImage);
        captureSprite.setScale(1.f / motaSystem.resolutionRatio, 1.f / motaSystem.resolutionRatio);
        RenderTexture captureTexture;
        captureTexture.create(640, 480);
        captureTexture.clear();
        captureTexture.draw(captureSprite);
        captureTexture.display();
        saveImage = captureTexture.getTexture().copyToImage();
    };
    // 按下0键时
    if (motaKeyBoard.trigger(motaKeyBoard.Key0)) {
        motaVariables.variables[7] = !motaVariables.variables[7];
        return;
    }
    // 按下取消键时
    if (motaKeyBoard.triggerCancel()) {
        // 保存屏幕截图
        capture();
        // 打开菜单栏
        menuWindow.index = 0;
        menuWindow.visible = true;
        menuWindow.active = true;
        // 关闭状态栏
        motaTemp.closeMS = true;
        return;
    }
    // 按下L键（有ShortcutKey.txt时）
    if (motaKeyBoard.trigger(motaKeyBoard.KeyL) && filesystem::exists("ref\\ShortcutKey.txt")) {
        // 打开快捷键提示窗口
        hintWindow.nowPage = 0;
        hintWindow.visible = true;
        return;
    }
    // 按下I键时
    if (motaKeyBoard.trigger(motaKeyBoard.KeyI)) {
        // 打开物品界面
        itemWindow.myItems.clear();
        for (size_t i = 0; i < motaData.items.size(); ++i)
            if (screenData.actors[motaVariables.variables[0]].item[i] > 0)
                itemWindow.myItems.push_back(motaData.items[i]);
        itemWindow.index = 0;
        itemWindow.visible = true;
        return;
    }
    // 按下C键时
    if (motaKeyBoard.trigger(motaKeyBoard.KeyC)) {
        // 保存屏幕截图
        capture();
        // 打开存档窗口
        loadSaveWindow.index = stoi(readFile("save\\recent.dat")[0]);
        loadSaveWindow.load = false;
        loadSaveWindow.visible = true;
        loadSaveWindow.from = 0;
        // 关闭状态栏
        motaTemp.closeMS = true;
        return;
    }
    // 按下V键时
    if (motaKeyBoard.trigger(motaKeyBoard.KeyV)) {
        // 打开读档窗口
        loadSaveWindow.index = stoi(readFile("save\\recent.dat")[0]);
        loadSaveWindow.load = true;
        loadSaveWindow.visible = true;
        loadSaveWindow.from = 0;
        // 关闭状态栏
        motaTemp.closeMS = true;
        return;
    }
    // 按下P键时
    if (motaKeyBoard.trigger(motaKeyBoard.KeyP)) {
        // 更新地图显示伤害的类型
        playSE(motaSystem.cursorSE);
        ++motaVariables.variables[4];
        return;
    }
    // 按下D键且有怪物手册时
    if (motaKeyBoard.trigger(motaKeyBoard.KeyD) && screenData.actors[motaVariables.variables[0]].item[3] > 0) {
        // 打开怪物手册窗口
        enemyBookWindow.index = 0;
        enemyBookWindow.nowPage = 0;
        enemyBookWindow.visible = true;
        enemyBookWindow.from = 0;
        return;
    }
    // 按下F键且有楼层传送器时
    if (motaKeyBoard.trigger(motaKeyBoard.KeyF) && screenData.actors[motaVariables.variables[0]].item[4] > 0) {
        itemUse(4);
    }
}

void MotaMap::itemUse(int id) {
    // 根据下左右上顺序设置的方向数组，代入角色朝向即可获取前方一格的事件。
    int dir[4][2] = {{0, 1}, {-1, 0}, {1, 0}, {0, -1}};
    auto aPos = make_pair(&screenData.actors[motaVariables.variables[0]].x, &screenData.actors[motaVariables.variables[0]].y);
    auto facePos = make_pair(*aPos.first + dir[screenData.player.direction][0], *aPos.second + dir[screenData.player.direction][1]);
    // 类似破墙道具使用的Lambda表达式，直接调用即可
    auto breaksth = [&](const string& breakName) {
        if (screenData.visualMap.haveAnEvent(facePos.first, facePos.second)) {
            auto ev = screenData.visualMap.EcheckEvent(facePos.first, facePos.second);
            auto namelist = split(ev->name, "<>");
            for (const auto& eachname : namelist) {
                if (auto evn = split(eachname, "/")[0]; ev->exist && evn == breakName) {
                    if (evn == "monster" && motaData.enemies[stoi(split(eachname, "/")[1])].getP(11))
                        return 2;
                    ev->endEvent();
                    return 1;
                }
            }
        }
        return 0;
    };
    // 使用成功标志
    int flag = 0;
    // 怪物手册
    if (id == 3) {
        enemyBookWindow.index = 0;
        enemyBookWindow.nowPage = 0;
        enemyBookWindow.from = 1;
        enemyBookWindow.visible = true;
        return;
    }
    // 楼层传送器
    if (id == 4) {
        // 打开楼层传送器窗口
        if (motaVariables.variables[3]) {
            motaTemp.messageInfo.emplace_back(-3, "", "本层禁止使用楼层传送器");
            return;
        }
        flyWindow.visible = true;
        flyWindow.viewFloorIndex = find(motaVariables.floorRecord[motaVariables.variables[1]].begin(), motaVariables.floorRecord[motaVariables.variables[1]].end(), motaVariables.variables[2]) - motaVariables.floorRecord[motaVariables.variables[1]].begin();
        return;
    }
    // 破墙镐
    if (id == 5) {
        flag = breaksth("wall");
        if (!flag) motaTemp.messageInfo.emplace_back(-3, "", "前方没有墙");
        goto endUse;
    }
    // 地震卷轴
    if (id == 6) {
        for (auto& ev : screenData.visualMap.mapEvents) {
            if (ev.exist && ev.name == "wall") {
                ev.endEvent();
                flag = true;
            }
        }
        if (!flag) motaTemp.messageInfo.emplace_back(-3, "", "本层没有墙");
        goto endUse;
    }
    // 解毒药水
    if (id == 7) {
        if (screenData.actors[motaVariables.variables[0]].status.count(1)) {
            screenData.actors[motaVariables.variables[0]].status.erase(1);
            motaVariables.variables[5] = 0;
            flag = 1;
        }
        else
            motaTemp.messageInfo.emplace_back(-3, "", "你并没有中毒");
        goto endUse;
    }
    // 解衰药水
    if (id == 8) {
        if (screenData.actors[motaVariables.variables[0]].status.count(2)) {
            screenData.actors[motaVariables.variables[0]].status.erase(2);
            motaVariables.variables[6] = 0;
            flag = 1;
        }
        else
            motaTemp.messageInfo.emplace_back(-3, "", "你并没有衰弱");
        goto endUse;
    }
    // 炸弹
    if (id == 9) {
        flag = breaksth("monster");
        if (flag == 0) motaTemp.messageInfo.emplace_back(-3, "", "前方没有怪物");
        else if (flag == 2) {
            motaTemp.messageInfo.emplace_back(-3, "", "无法炸死ＢＯＳＳ怪物");
            flag = 0;
        }
        goto endUse;
    }
    // 冰冻徽章
    if (id == 10) {
        flag = breaksth("fire");
        if (flag == 0) motaTemp.messageInfo.emplace_back(-3, "", "前方没有熔岩");
        goto endUse;
    }
    // 中心飞
    if (id == 11) {
        auto npos = make_pair(10 - *aPos.first, 10 - *aPos.second);
        if (screenData.visualMap.passible(npos.first, npos.second)) {
            *aPos.first = npos.first;
            *aPos.second = npos.second;
            flag = 1;
        }
        else {
            motaTemp.messageInfo.emplace_back(-3, "", "有障碍物，无法飞行");
            flag = 0;
        }
    }
    // 圣锤
    if (id == 13) {
        auto evlist = screenData.visualMap.getLineEvents(make_pair(*aPos.first, *aPos.second), make_pair(dir[screenData.player.direction][0] == 1 ? 10 : (dir[screenData.player.direction][0] == -1 ? 0 : *aPos.first), dir[screenData.player.direction][1] == 1 ? 10 : (dir[screenData.player.direction][1] == -1 ? 0 : *aPos.second)));
        for (auto bev : evlist) {
            if (bev->exist) {
                auto namelist = split(bev->name, "<>");
                for (const auto& eachname : namelist) {
                    if (auto evn = split(eachname, "/")[0]; evn == "monster") {
                        if (motaData.enemies[stoi(split(eachname, "/")[1])].getP(11))
                            break;
                        bev->endEvent();
                        flag = 1;
                    }
                }
            }
        }
        if (!flag) motaTemp.messageInfo.emplace_back(-3, "", "使用失败");
        goto endUse;
    }
    // 使用物品的结算，使用成功则扣除道具
    endUse:
    if (flag == 1 && motaData.items[id].cost) --screenData.actors[motaVariables.variables[0]].item[id];
}

void MotaMap::updateMenu() {
    // 按下确认键时
    if (menuWindow.active && motaKeyBoard.triggerConfirm()) {
        if (menuWindow.index == 0) {
            playSE(motaSystem.decisionSE);
            // 转到物品界面，读入当前物品信息
            itemWindow.myItems.clear();
            for (size_t i = 0; i < motaData.items.size(); ++i)
                if (screenData.actors[motaVariables.variables[0]].item[i] > 0)
                    itemWindow.myItems.push_back(motaData.items[i]);
            itemWindow.index = 0;
            itemWindow.visible = true;
            // 关闭菜单活动
            menuWindow.active = false;
            return;
        }
        else if (menuWindow.index == 1) {
            playSE(motaSystem.decisionSE);
            // 转到存档界面，from=1表示从菜单来的
            loadSaveWindow.index = stoi(readFile("save\\recent.dat")[0]);
            loadSaveWindow.load = false;
            loadSaveWindow.visible = true;
            loadSaveWindow.from = 1;
            // 关闭菜单窗口
            menuWindow.visible = false;
            // 关闭状态栏显示
            motaTemp.closeMS = true;
            return;
        }
        else if (menuWindow.index == 2) {
            playSE(motaSystem.decisionSE);
            // 转到读档界面，from=1表示从菜单来的
            loadSaveWindow.index = stoi(readFile("save\\recent.dat")[0]);
            loadSaveWindow.load = true;
            loadSaveWindow.visible = true;
            loadSaveWindow.from = 1;
            // 关闭菜单窗口
            menuWindow.visible = false;
            // 关闭状态栏显示
            motaTemp.closeMS = true;
            return;
        }
        else if (menuWindow.index == 3) {
            playSE(motaSystem.decisionSE);
            // 回到标题界面
            motaSystem.scene = new MotaTitle;
            return;
        }
    }
    // 按下取消键时
    if (menuWindow.active && motaKeyBoard.triggerCancel()) {
        playSE(motaSystem.cancelSE);
        // 关闭菜单窗口
        menuWindow.visible = false;
        // 恢复状态栏显示
        motaTemp.closeMS = false;
        return;
    }
}

void MotaMap::updateItem() {
    // 按下确认键时
    if (motaKeyBoard.triggerConfirm()) {
        if (itemWindow.myItems.empty()) return;
        if (itemWindow.myItems[itemWindow.index].usable) {
            playSE(motaSystem.decisionSE);
            // 关闭物品窗口
            itemWindow.visible = false;
            // 如果菜单窗口还在，就也关闭，除非是使用怪物手册
            if (menuWindow.visible && itemWindow.myItems[itemWindow.index].ID != 3) {
                menuWindow.visible = false;
                motaTemp.closeMS = false;
            }
            // 转到物品使用函数
            itemUse(itemWindow.myItems[itemWindow.index].ID);
        }
        else
            playSE(motaSystem.buzzerSE);
        return;
    }
    // 按下取消键时
    if (motaKeyBoard.triggerCancel()) {
        playSE(motaSystem.cancelSE);
        // 关闭物品窗口
        itemWindow.visible = false;
        // 如果菜单窗口可见，就恢复其活动
        if (menuWindow.visible) menuWindow.active = true;
        return;
    }
}

void MotaMap::updateLoadSave() {
    // 按下确认键时
    if (motaKeyBoard.triggerConfirm()) {
        // 如果是读档
        if (loadSaveWindow.load) {
            // 如果存档文件存在
            if (filesystem::exists(format("save\\save_{}.sav", loadSaveWindow.index))) {
                playSE(motaSystem.loadSE);
                // 关闭存读档窗口
                loadSaveWindow.visible = false;
                // 关闭菜单窗口
                menuWindow.visible = false;
                // 打开状态栏窗口
                motaTemp.closeMS = false;
                // 加载动画
                GameImage stairImg("system\\mting.png");
                tie (stairImg.x, stairImg.y, stairImg.opacity, stairImg.z) = make_tuple(MAPX, MAPY, 0, 2);
                for (int i = 0; i < 15; ++i) {
                    stairImg.opacity += 17;
                    screenData.showMap(screenData.visualMap, MAPX, MAPY);
                    motaGraphics.update(false);
                }
                // 初始化部分数据
                motaVariables.init();
                motaTemp.init();
                screenData.init();
                // 读取存档
                screenData.loadData(loadSaveWindow.index);
                // 重新加载地图
                screenData.player.direction = 0;
                screenData.loadMap(screenData.actors[motaVariables.variables[0]].mapID, &screenData.visualMap);
                // 加载动画
                for (int i = 0; i < 15; ++i) {
                    stairImg.opacity -= 17;
                    screenData.showMap(screenData.visualMap, MAPX, MAPY);
                    motaGraphics.update(false);
                }
                stairImg.dispose();
                return;
            }
            else {
                playSE(motaSystem.buzzerSE);
                return;
            }
        }
        else {
            playSE(motaSystem.saveSE);
            screenData.saveData(loadSaveWindow.index);
            saveImage.saveToFile(format("save\\save_{}.png", loadSaveWindow.index));
            saveFile("save\\recent.dat", to_string(loadSaveWindow.index));
            // 关闭存读档窗口
            loadSaveWindow.visible = false;
            // 关闭菜单窗口
            menuWindow.visible = false;
            // 打开状态栏窗口
            motaTemp.closeMS = false;
            return;
        }
    }
    // 按下取消键时
    if (motaKeyBoard.triggerCancel()) {
        playSE(motaSystem.cancelSE);
        // 关闭存读档窗口
        loadSaveWindow.visible = false;
        // 打开状态栏窗口
        motaTemp.closeMS = false;
        // 如果是从菜单来的，就别打开了，打开菜单窗口
        if (loadSaveWindow.from == 1) {
            menuWindow.visible = true;
            motaTemp.closeMS = true;
        }
        return;
    }
}

void MotaMap::updateHint() {
    // 按下取消键时
    if (motaKeyBoard.triggerCancel()) {
        playSE(motaSystem.cancelSE);
        // 关闭快捷键提示窗口
        hintWindow.visible = false;
    }
}

void MotaMap::updateShop() {
    // 按下取消键时
    if (motaKeyBoard.triggerCancel()) {
        playSE(motaSystem.cancelSE);
        // 关闭商店窗口
        shopWindow.visible = false;
        return;
    }
}

void MotaMap::updateMessage() {
    // 显示对话窗口
    messageWindow.visible = true;
    // 按下确认键时
    if (motaKeyBoard.triggerConfirm()) {
        // 如果这是个带选择项的窗口
        if (messageWindow.haveIndex) {
            // 如果有指令的话
            if (!messageWindow.order[messageWindow.index].empty())
                screenData.doOrder(messageWindow.order[messageWindow.index]);
            messageWindow.index = -1;
        }
        // 将本次对话从队列中清除
        motaTemp.messageInfo.erase(motaTemp.messageInfo.begin());
        // 将不透明度清零防止闪烁
        messageWindow.opacity = 0;
        // 如果队列清空了，就关闭对话窗口显示
        if (motaTemp.messageInfo.empty()) messageWindow.visible = false;
        screenData.waitCount(1);
        update();
    }
}

void MotaMap::updateEnemyBook() {
    // 按下确认键时
    if (motaKeyBoard.triggerConfirm() && !motaTemp.floorEnemies.empty()) {
        playSE(motaSystem.decisionSE);
        // 关闭怪物手册窗口
        enemyBookWindow.visible = false;
        // 录入怪物信息，打开怪物百科窗口
        encyWindow.mon = motaTemp.floorEnemies[enemyBookWindow.nowPage * 4 + enemyBookWindow.index];
        encyWindow.visible = true;
        return;
    }
    // 按下取消键时
    if (motaKeyBoard.triggerCancel()) {
        playSE(motaSystem.cancelSE);
        // 关闭怪物手册窗口
        enemyBookWindow.visible = false;
        // 如果是从物品栏来的，就打开物品栏窗口
        if (enemyBookWindow.from == 1) itemWindow.visible = true;
        return;
    }
}

void MotaMap::updateEncyclopedia() {
    // 按下取消键时
    if (motaKeyBoard.triggerCancel()) {
        playSE(motaSystem.cancelSE);
        // 关闭怪物百科窗口，打开怪物手册窗口
        enemyBookWindow.visible = true;
        encyWindow.visible = false;
        return;
    }
}

void MotaMap::updateFly() {
    // 按下确认键时
    if (motaKeyBoard.triggerConfirm()) {
        playSE(motaSystem.stairSE);
        // 关闭楼层传送器窗口
        flyWindow.visible = false;
        // 获取目标地图信息
        auto targetMap = motaData.maps[motaData.searchMap(format("{}:{}", motaVariables.variables[1], motaVariables.floorRecord[motaVariables.variables[1]][flyWindow.viewFloorIndex]))];
        // 传送至指定楼层
        if (motaVariables.floorRecord[motaVariables.variables[1]][flyWindow.viewFloorIndex] > motaVariables.variables[2]) {
            // 如果是上楼
            motaVariables.variables[2] = motaVariables.floorRecord[motaVariables.variables[1]][flyWindow.viewFloorIndex] - 1;
            // 坐标调整至目标地图下楼梯位置
            GameEvent(format("up/{}/{}", targetMap.mapEvents[0].x, targetMap.mapEvents[0].y)).order(false);
        }
        else if (motaVariables.floorRecord[motaVariables.variables[1]][flyWindow.viewFloorIndex] < motaVariables.variables[2]) {
            // 如果是下楼
            motaVariables.variables[2] = motaVariables.floorRecord[motaVariables.variables[1]][flyWindow.viewFloorIndex] + 1;
            // 坐标调整至目标地图上楼梯位置
            GameEvent(format("down/{}/{}", targetMap.mapEvents[1].x, targetMap.mapEvents[1].y)).order(false);
        }
        else {
            // 如果是同楼
            if (motaVariables.variables[2] >= 0) {
                // 地上同层传送到下楼梯位置
                GameEvent(format("move/{}/{}/{}", screenData.actors[motaVariables.variables[0]].mapID, targetMap.mapEvents[0].x, targetMap.mapEvents[0].y)).order(false);
            }
            else {
                // 地下同层传送到上楼梯位置
                GameEvent(format("move/{}/{}/{}", screenData.actors[motaVariables.variables[1]].mapID, targetMap.mapEvents[1].x, targetMap.mapEvents[0].y)).order(false);
            }
        }
        return;
    }
    // 按下取消键时
    if (motaKeyBoard.triggerCancel()) {
        playSE(motaSystem.cancelSE);
        // 关闭楼层传送器窗口
        flyWindow.visible = false;
        return;
    }
}

void MotaMap::setShop() {
    shopWindow.visible = true;
    shopWindow.index = 0;
    shopWindow.items.clear();

    auto setInfo = [&](string name, string file, string desc, int pos) {
        shopWindow.name = std::move(name);
        shopWindow.file = std::move(file);
        shopWindow.desc = std::move(desc);
        shopWindow.pos = pos;
    };

    auto setItem = [&](const string& name, int* ref, string cost, vector <string> order = {}) {
        if (ref == &screenData.actors[motaVariables.variables[0]].gold)
            order.emplace_back(format("bonus/5/-{}", cost));
        else if (ref == &screenData.actors[motaVariables.variables[0]].exp)
            order.emplace_back(format("bonus/4/-{}", cost));
        shopWindow.items.emplace_back(name,
                                      make_pair(ref, cost),
                                      order);
    };

    if (motaTemp.shopType == 0) {
        // 贪婪之神
        if (!strInclude(motaTemp.initPrice[0], "[") && motaTemp.initPrice[0].length() % 3 != 0)
            for (int i = 0, len = motaTemp.initPrice[0].length() % 3; i < 3 - len; ++i)
                motaTemp.initPrice[0] += " ";
        setInfo("贪婪之神", "NPC01-02.png", format("愚蠢的人类，如果你能提供{}个金币，我将可以提升你的力量！", motaTemp.initPrice[0]), 2);
        string desc[] = {"生命值", "攻击力", "防御力"};
        string var = motaTemp.initPrice[0];
        replaceAll(var, "[", "");
        replaceAll(var, "]", "");
        for (int i = 0; i < 3; ++i) {
            setItem(format("增加{}点{}", motaTemp.addPower[i], desc[i]),
                    &screenData.actors[motaVariables.variables[0]].gold, motaTemp.initPrice[0],
                    {format("bonus/{}/{}", i, motaTemp.addPower[i]), // 增加能力
                     format("var/{}/{}", var, motaTemp.rise)});// 涨价
        }
    }
    else if (motaTemp.shopType == 1) {
        // 战斗之神
        setInfo("战斗之神", "NPC01-02.png", "英雄的人类！如果你可以给我提供经验，我将可以提升你的力量！", 1);
        string desc[] = {"等级", "攻击", "防御"};
        int kind[] = {6, 1, 2};
        for (int i = 0; i < 3; ++i) {
            shopWindow.items.emplace_back(format("{}经验增加{}个{}",motaTemp.initPrice[i], motaTemp.addPower[i], desc[i]),
                                          make_pair(&screenData.actors[motaVariables.variables[0]].exp, motaTemp.initPrice[i]),
                                          vector <string> ({format("bonus/4/-{}", motaTemp.initPrice[i]), // 扣除经验
                                                            format("bonus/{}/{}", kind[i], motaTemp.addPower[i])})); // 增加等级
        }
    }
    motaTemp.shopType = -1;
    return;
}

void MotaMap::setBattle() {
    // 添加双方动画
    screenData.addEVAnimation(screenData.actors[motaVariables.variables[0]].animationID, screenData.visualMap.mapEvents[motaTemp.functionEventID].x, screenData.visualMap.mapEvents[motaTemp.functionEventID].y);
    screenData.waitCount(motaData.animations[screenData.actors[motaVariables.variables[0]].animationID].pattern.size());
    screenData.addEVAnimation(motaData.enemies[motaTemp.battleEnemyID].animationID, screenData.actors[motaVariables.variables[0]].x, screenData.actors[motaVariables.variables[0]].y);
    screenData.waitCount(motaData.animations[motaData.enemies[motaTemp.battleEnemyID].animationID].pattern.size());
    // 计算伤害
    int damage = motaData.enemies[motaTemp.battleEnemyID].getDamage();
    // 如果返回值为-1代表不可战胜
    if (damage == -1) damage = screenData.actors[motaVariables.variables[0]].hp;
    // 当我方生命大于对方生命时正常扣血
    if (auto en = motaData.enemies[motaTemp.battleEnemyID]; screenData.actors[motaVariables.variables[0]].hp > damage) {
        screenData.actors[motaVariables.variables[0]].hp -= damage;
        screenData.actors[motaVariables.variables[0]].exp += en.exp;
        screenData.actors[motaVariables.variables[0]].gold += en.gold * (1 + (screenData.actors[motaVariables.variables[0]].item[12] > 0));
        screenData.visualMap.mapEvents[motaTemp.functionEventID].toDispose = true;
        // 有[v]需要在战斗后使用的话，写在这里
        for (int i = 0, cnt = 1; i < en.element.size(); ++i) {
            if (motaData.elements[en.element[i]].haveBuff) {
                float buff = stof(split(en.name, "/")[cnt]);
                if (en.element[i] == 1) {
                    // 中毒的效果
                    motaVariables.variables[5] += buff;
                    screenData.actors[motaVariables.variables[0]].status.insert(1);
                }
                else if (en.element[i] == 2) {
                    // 衰弱的效果
                    motaVariables.variables[6] += buff;
                    screenData.actors[motaVariables.variables[0]].status.insert(2);
                }
                else if (en.element[i] == 8) {
                    // 再生的效果
                    motaTemp.transEventName = format("monster/{}", (int)buff); // 更改事件名
                    screenData.visualMap.mapEvents[motaTemp.functionEventID].toDispose = false; // 不需要消失
                }

                cnt++;
            }
        }
    }
    else {
        // 如果不敌则游戏结束
        screenData.actors[motaVariables.variables[0]].hp = 0;
        GameEvent("gg").order(true);
    }
    // 清空对战斗事件的记录
    motaTemp.battleEnemyID = -1;
}

void MotaMap::setTrans() {
    // 更新事件名
    screenData.visualMap.mapEvents[motaTemp.functionEventID].name = motaTemp.transEventName;
    // 特殊更改的处理
    auto namelist = split(motaTemp.transEventName, "<>");
    for (const auto& eachname : namelist) {
        auto namesplt = split(eachname, "/");
        if (namesplt[0] == "monster") {
            // 更改成了怪物
            screenData.visualMap.mapEvents[motaTemp.functionEventID].file = motaData.enemies[stoi(namesplt[1])].file;
            screenData.visualMap.mapEvents[motaTemp.functionEventID].pos[1] = motaData.enemies[stoi(namesplt[1])].pos;
            screenData.visualMap.mapEvents[motaTemp.functionEventID].move = true;
            screenData.visualMap.mapEvents[motaTemp.functionEventID].through = false;
        }
    }
    // 记录更新事件
    motaVariables.transRecord[screenData.visualMap.mapID][motaTemp.functionEventID] = motaTemp.transEventName;
    // 清空更新事件名
    motaTemp.transEventName = "";
    // 如果直接触发新事件
    if (motaTemp.directlyFunction) {
        motaTemp.directlyFunction = false;
        screenData.visualMap.mapEvents[motaTemp.functionEventID].order();
    }
}

void MotaScript::main() {
    auto content = readFile(motaTemp.ending ? "ref\\end.txt" : "ref\\begin.txt");
    string allcontent = "";
    for (const auto& ctt : content)
        allcontent += ctt + "\n";
    text.setText(allcontent);
    text.x = (640 - text.getSize().first) / 2;
    text.y = 480;
    text.z = 3;
    back.setSprite("system\\script-back.png");
    back.z = 4;
    space.setSprite("system\\script-space.png");
    space.x = 270;
    space.y = 450;
    space.z = 4;
    space.opacity = 135;
    // 执行渐变
    screenData.transition1();
    // 主循环
    while (motaSystem.scene == this && motaSystem.window.isOpen()) {
        Event event;
        while (motaSystem.window.pollEvent(event))
            if (event.type == Event::Closed)
                motaSystem.window.close();
        // 当聚焦不在本窗口时直接继续
        if (!motaSystem.window.hasFocus()) continue;
        screenData.waitCount(1);
        update();
    }
    // 执行渐变
    screenData.transition2();
    text.dispose();
    back.dispose();
    space.dispose();
}

void MotaScript::update() {
    // 更新文字位置
    text.y -= 2;
    space.opacity = abs((int)(120 - 20 * ((motaSystem.gameTime / 4) % 7))) + 135;
    if (motaKeyBoard.triggerConfirm() || text.y <= 0 - text.getSize().second) {
        if (motaTemp.ending) motaSystem.scene = new MotaTitle;
        else motaSystem.scene = new MotaMap;
    }
}
