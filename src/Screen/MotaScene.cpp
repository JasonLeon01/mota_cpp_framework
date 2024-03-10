#include <Game/Screen/MotaScene.hpp>

void MotaTitle::main() {
    // 执行渐变，第一个是打开游戏时的渐变，如果opacity=255则不执行
    screenData.transition2();

    // 设置标题画面精灵
    spr.setSprite(motaSystem.titleName);
    motaGraphics.addImage(&spr);

    // 设置选择命令窗口坐标
    slcWindow.x = (640 - slcWindow.width) / 2;
    slcWindow.y = 448 - slcWindow.height;
    slcWindow.opacity = motaSystem.windowOpacity;
    slcWindow.visible = true;
    motaGraphics.addWindow(&slcWindow);

    // 添加存读档窗口
    motaGraphics.addWindow(&loadSaveWindow);

    // 不存在recent文件时创立一个
    if (!std::filesystem::exists("save\\recent.dat")) saveFile("save\\recent.dat", "0");
    else slcWindow.index = 1;

    // 切换BGM
    motaSystem.bgmSwitch(motaSystem.titleBGM);
    screenData.transition1();

    // 主循环
    while (motaSystem.scene == this && motaSystem.window.isOpen()) {
        // 更新画面
        screenData.waitCount(1);

        if (motaSystem.window.hasFocus()) {
            // 更新标题
            update();
        }
    }

    // 执行渐变
    screenData.transition2();

    // 消除元素
    motaGraphics.eraseImage(&spr);
    motaGraphics.eraseWindow(&slcWindow);
    motaGraphics.eraseWindow(&loadSaveWindow);
}

void MotaTitle::update() {
    // 更新存读档窗口
    if (loadSaveWindow.visible) {
        // 按下确认键时
        if (Input::triggerConfirm()) {
            // 如果存档文件存在
            if (std::filesystem::exists(std::format("save\\save_{}.json", loadSaveWindow.index))) {
                playSE(motaSystem.loadSE, motaSystem.SEVolume);

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
                playSE(motaSystem.buzzerSE, motaSystem.SEVolume);
                return;
            }
        }

        // 按下取消键时
        if (Input::triggerCancel()) {
            playSE(motaSystem.cancelSE, motaSystem.SEVolume);
            loadSaveWindow.visible = false;
            slcWindow.visible = true;
            return;
        }
        return;
    }
    // 按下确认键时
    if (Input::triggerConfirm()) {
        if (slcWindow.index == 0) {
            playSE(motaSystem.decisionSE, motaSystem.SEVolume);
            motaVariables.init();
            motaTemp.init();
            screenData.init();
            motaVariables.floorRecord[0].push_back(0);
            screenData.loadMap(screenData.actors[motaVariables.variables[0]].mapID, &screenData.visualMap);
            if (std::filesystem::exists("ref\\begin.txt")) motaSystem.scene = new MotaScript;
            else motaSystem.scene = new MotaMap;
            return;
        }
        else if (slcWindow.index == 1) {
            playSE(motaSystem.decisionSE, motaSystem.SEVolume);
            loadSaveWindow.index = stoi(readFile("save\\recent.dat")[0]);
            loadSaveWindow.load = true;
            loadSaveWindow.visible = true;
            slcWindow.visible = false;
            return;
        }
        else if (slcWindow.index == 2) {
            playSE(motaSystem.decisionSE, motaSystem.SEVolume);
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

    // 设置配置窗口坐标
    configWindow.x = MAPX + 4;
    configWindow.y = MAPY;
    configWindow.visible = false;

    // 将各类窗口添加进队列
    motaGraphics.addWindow(&menuWindow);
    motaGraphics.addWindow(&configWindow);
    motaGraphics.addWindow(&messageWindow);
    motaGraphics.addWindow(&hintWindow);
    motaGraphics.addWindow(&shopWindow);
    motaGraphics.addWindow(&itemWindow);
    motaGraphics.addWindow(&enemyBookWindow);
    motaGraphics.addWindow(&encyWindow);
    motaGraphics.addWindow(&flyWindow);
    motaGraphics.addWindow(&loadSaveWindow);

    // 设置BGM
    bgmFile = screenData.visualMap.bgmFile;
    motaSystem.bgmSwitch(bgmFile);

    // 主循环
    while (motaSystem.scene == this && motaSystem.window.isOpen()) {
        // 更新画面
        screenData.waitCount(1);

        if (motaSystem.window.hasFocus()) {
            // 更新地图
            update();
        }
    }

    // 执行渐变
    screenData.transition2();
    screenData.onMap = false;

    // 消除元素
    motaGraphics.eraseWindow(&menuWindow);
    motaGraphics.eraseWindow(&configWindow);
    motaGraphics.eraseWindow(&messageWindow);
    motaGraphics.eraseWindow(&hintWindow);
    motaGraphics.eraseWindow(&shopWindow);
    motaGraphics.eraseWindow(&itemWindow);
    motaGraphics.eraseWindow(&enemyBookWindow);
    motaGraphics.eraseWindow(&encyWindow);
    motaGraphics.eraseWindow(&flyWindow);
    motaGraphics.eraseWindow(&loadSaveWindow);
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
        if (menuWindow.active) return;
    }

    // 更新设置窗口
    if (configWindow.visible) {
        updateConfig();
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
    if (motaTemp.shopType >= 0) setShop();

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
    if (!motaTemp.transEventName.empty()) setTrans();

    // 如果事件记录不为空
    if (motaTemp.functionEventID != -1) {
        // 消除事件
        if (auto& recev = screenData.visualMap.mapEvents[motaTemp.functionEventID]; recev.exist && recev.toDispose) {
            recev.exist = false;
            motaVariables.eventRecord[screenData.visualMap.mapID].push_back(recev.ID);

            // 清除记录
            motaTemp.functionEventID = -1;
            return;
        }
    }

    // 游戏结束则回到标题
    if (motaTemp.gameOver) {
        if (motaTemp.ending && std::filesystem::exists("ref\\end.txt")) motaSystem.scene = new MotaScript;
        else motaSystem.scene = new MotaTitle;
        return;
    }

    // 结局
    if (motaTemp.ending) {
        if (std::filesystem::exists("ref\\end.txt")) motaSystem.scene = new MotaScript;
        else motaSystem.scene = new MotaTitle;
        return;
    }

    // 快捷键的判定
    shortcutKey();

    // 自动寻路
    if (auto mousePos = sf::Mouse::getPosition(motaSystem.window); !screenData.searchingRoad && GameWindow::inRange(std::make_pair(mousePos.x / motaSystem.resolutionRatio, mousePos.y / motaSystem.resolutionRatio), std::make_tuple(MAPX, MAPY, 352, 352))) {
        if (Input::leftClick()) {
            screenData.targetPos = std::make_pair((int)(mousePos.x / motaSystem.resolutionRatio - MAPX) / 32, (int)(mousePos.y / motaSystem.resolutionRatio - MAPY) / 32);
            autoSearching(screenData.targetPos);
        }
    }

    // 更新玩家画面
    screenData.player.update();
}

void MotaMap::shortcutKey() {
    auto capture = [&]() {
        sf::Texture captureImage;
        captureImage.create(motaSystem.window.getSize().x, motaSystem.window.getSize().y);
        captureImage.update(motaSystem.window);
        sf::Sprite captureSprite(captureImage);
        captureSprite.setScale(1.f / motaSystem.resolutionRatio, 1.f / motaSystem.resolutionRatio);
        sf::RenderTexture captureTexture;
        captureTexture.create(motaSystem.window.getSize().x, motaSystem.window.getSize().y);
        captureTexture.clear();
        captureTexture.draw(captureSprite);
        captureTexture.display();
        saveImage = captureTexture.getTexture().copyToImage();
    };

    // 按下0键时
    if (Input::trigger(Input::Key0)) {
        motaVariables.variables[7] = !motaVariables.variables[7];
        return;
    }

    // 按下Z键时
    if (Input::trigger(Input::KeyZ)) {
        screenData.player.direction = (screenData.player.direction + 1) % 4;
        return;
    }

    // 按下取消键时
    if (Input::triggerCancel()) {
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
    if (Input::trigger(Input::KeyL) && std::filesystem::exists("ref\\ShortcutKey.txt")) {
        // 打开快捷键提示窗口
        hintWindow.nowPage = 0;
        hintWindow.visible = true;
        return;
    }

    // 按下I键时
    if (Input::trigger(Input::KeyI)) {
        // 打开物品界面
        itemWindow.myItems.clear();
        for (size_t i = 0; i < motaData.items.size(); ++i) {
            if (screenData.actors[motaVariables.variables[0]].item[i] > 0) {
                itemWindow.myItems.push_back(motaData.items[i]);
            }
        }
        itemWindow.index = 0;
        itemWindow.visible = true;
        return;
    }

    // 按下[键时
    if (Input::trigger(Input::KeyBra)) {
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

    // 按下]键时
    if (Input::trigger(Input::KeyKet)) {
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
    if (Input::trigger(Input::KeyP)) {
        // 更新地图显示伤害的类型
        playSE(motaSystem.cursorSE, motaSystem.SEVolume);
        ++motaVariables.variables[4];
        return;
    }

    // 按下D键且有怪物手册时
    if (Input::trigger(Input::KeyD) && screenData.actors[motaVariables.variables[0]].item[3] > 0) {
        // 打开怪物手册窗口
        enemyBookWindow.index = 0;
        enemyBookWindow.nowPage = 0;
        enemyBookWindow.visible = true;
        enemyBookWindow.from = 0;
        return;
    }

    // 按下F键且有楼层传送器时
    if (Input::trigger(Input::KeyF) && screenData.actors[motaVariables.variables[0]].item[4] > 0) {
        itemUse(4);
    }
}

void MotaMap::itemUse(int id) {
    // 根据下左右上顺序设置的方向数组，代入角色朝向即可获取前方一格的事件。
    int dir[4][2] = {{0, 1}, {-1, 0}, {1, 0}, {0, -1}};
    auto& act = screenData.actors[motaVariables.variables[0]];
    auto& ply = screenData.player;
    auto aPos = std::make_pair(&act.x, &act.y);
    auto facePos = std::make_pair(*aPos.first + dir[ply.direction][0], *aPos.second + dir[ply.direction][1]);
    // 类似破墙道具使用的Lambda表达式，直接调用即可
    auto breaksth = [&](const std::string& breakName) {
        if (screenData.visualMap.haveAnEvent(facePos.first, facePos.second)) {
            auto ev = screenData.visualMap.EcheckEvent(facePos.first, facePos.second);
            auto namelist = Interpreter::initEventName(ev->name);
            for (const auto& eachname : namelist) {
                if (auto evn = Interpreter::initOrder(eachname); ev->exist && evn[0] == breakName) {
                    if (evn[0] == "monster") {
                        if (motaData.enemies[stoi(evn[1])].getP(11)) {
                            return 2;
                        }
                    }
                    else {
                        Interpreter::openDoor(ev);
                    }
                    ev->exist = false;
                    motaVariables.eventRecord[screenData.visualMap.mapID].push_back(ev->ID);
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
        auto& var = motaVariables.variables;
        auto flr= motaVariables.floorRecord[var[1]];
        flyWindow.viewFloorIndex = find(flr.begin(), flr.end(), var[2]) - flr.begin();
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
                Interpreter::openDoor(&ev);
                motaVariables.eventRecord[screenData.visualMap.mapID].push_back(ev.ID);
                ev.exist = false;
                flag = true;
            }
        }
        if (!flag) motaTemp.messageInfo.emplace_back(-3, "", "本层没有墙");
        goto endUse;
    }

    // 解毒药水
    if (id == 7) {
        if (act.status.count(1)) {
            act.status.erase(1);
            motaVariables.variables[5] = 0;
            flag = 1;
        }
        else {
            motaTemp.messageInfo.emplace_back(-3, "", "你并没有中毒");
        }
        goto endUse;
    }

    // 解衰药水
    if (id == 8) {
        if (auto& act = screenData.actors[motaVariables.variables[0]]; act.status.count(2)) {
            act.status.erase(2);
            motaVariables.variables[6] = 0;
            flag = 1;
        }
        else {
            motaTemp.messageInfo.emplace_back(-3, "", "你并没有衰弱");
        }
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
        auto npos = std::make_pair(10 - *aPos.first, 10 - *aPos.second);
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
        auto evlist = screenData.visualMap.getLineEvents(std::make_pair(*aPos.first, *aPos.second), std::make_pair(dir[screenData.player.direction][0] == 1 ? 10 : (dir[screenData.player.direction][0] == -1 ? 0 : *aPos.first), dir[screenData.player.direction][1] == 1 ? 10 : (dir[screenData.player.direction][1] == -1 ? 0 : *aPos.second)));
        for (auto bev : evlist) {
            if (bev->exist) {
                auto namelist = Interpreter::initEventName(bev->name);
                for (const auto& eachname : namelist) {
                    if (auto evn = Interpreter::initOrder(eachname); evn[0] == "monster") {
                        if (motaData.enemies[stoi(evn[1])].getP(11)) {
                            break;
                        }
                        bev->exist = false;
                        motaVariables.eventRecord[screenData.visualMap.mapID].push_back(bev->ID);
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
    if (flag == 1 && motaData.items[id].cost) {
        --screenData.actors[motaVariables.variables[0]].item[id];
    }
}

void MotaMap::updateMenu() {
    // 按下确认键时
    if (menuWindow.active && Input::triggerConfirm()) {
        if (menuWindow.index == 0) {
            playSE(motaSystem.decisionSE, motaSystem.SEVolume);

            // 转到物品界面，读入当前物品信息
            itemWindow.myItems.clear();
            for (size_t i = 0; i < motaData.items.size(); ++i) {
                if (screenData.actors[motaVariables.variables[0]].item[i] > 0) {
                    itemWindow.myItems.push_back(motaData.items[i]);
                }
            }
            itemWindow.index = 0;
            itemWindow.visible = true;

            // 关闭菜单活动
            menuWindow.active = false;
            return;
        }
        else if (menuWindow.index == 1) {
            playSE(motaSystem.decisionSE, motaSystem.SEVolume);

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
            playSE(motaSystem.decisionSE, motaSystem.SEVolume);

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
            playSE(motaSystem.decisionSE, motaSystem.SEVolume);

            // 转到设置窗口
            configWindow.visible = true;
            configWindow.index = 0;

            // 关闭菜单活动
            menuWindow.active = false;
            return;
        }
        else if (menuWindow.index == 4) {
            playSE(motaSystem.decisionSE, motaSystem.SEVolume);

            // 回到标题界面
            motaSystem.scene = new MotaTitle;
            return;
        }
    }

    // 按下取消键时
    if (menuWindow.active && Input::triggerCancel()) {
        playSE(motaSystem.cancelSE, motaSystem.SEVolume);

        // 关闭菜单窗口
        menuWindow.visible = false;

        // 恢复状态栏显示
        motaTemp.closeMS = false;
        return;
    }
}

void MotaMap::updateConfig() {
    std::map <float, std::string> configMap = {
            {1.f, "640x480"},
            {1.25, "800x600"},
            {1.5, "960x720"},
            {1.75, "1120x840"},
            {2.f, "1280x960"},
    };
    configWindow.items[0] = std::format("窗口大小：{}", configMap[motaSystem.resolutionRatio]);
    configWindow.items[1] = std::format("BGM音量：{}", motaSystem.BGMVolume);
    configWindow.items[2] = std::format("SE音量：{}", motaSystem.SEVolume);
    // 重置窗口大小
    auto resize = [&]() {
        // 重新设置大小
        motaSystem.window.setSize(sf::Vector2((unsigned int)(640 * motaSystem.resolutionRatio), (unsigned int)(480 * motaSystem.resolutionRatio)));
        // 获取当前屏幕的分辨率
        sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();

        // 计算窗口在屏幕中的位置
        sf::Vector2i windowPosition(
                (desktopMode.width - motaSystem.window.getSize().x) / 2,
                (desktopMode.height - motaSystem.window.getSize().y) / 2
        );

        // 设置窗口的位置
        motaSystem.window.setPosition(windowPosition);
    };

    // 按下左键时
    if (Input::repeat(Input::KeyLEFT)) {
        if (configWindow.index == 0 && motaSystem.resolutionRatio > 1) {
            motaSystem.resolutionRatio = std::max(1.f, motaSystem.resolutionRatio - 0.25f);
            resize();
            playSE(motaSystem.cursorSE, motaSystem.SEVolume);
        }
        if (configWindow.index == 1 && motaSystem.BGMVolume > 0)  {
            --motaSystem.BGMVolume;
            motaSystem.bgm.setVolume(motaSystem.BGMVolume);
            playSE(motaSystem.cursorSE, motaSystem.SEVolume);
        }
        if (configWindow.index == 2 && motaSystem.SEVolume > 0)  {
            --motaSystem.SEVolume;
            playSE(motaSystem.cursorSE, motaSystem.SEVolume);
        }
        return;
    }

    // 按下右键时
    if (Input::repeat(Input::KeyRIGHT)) {
        if (configWindow.index == 0 && motaSystem.resolutionRatio < 2) {
            motaSystem.resolutionRatio = std::max(1.f, motaSystem.resolutionRatio + 0.25f);
            resize();
            playSE(motaSystem.cursorSE, motaSystem.SEVolume);
        }
        if (configWindow.index == 1 && motaSystem.BGMVolume < 100)  {
            ++motaSystem.BGMVolume;
            motaSystem.bgm.setVolume(motaSystem.BGMVolume);
            playSE(motaSystem.cursorSE, motaSystem.SEVolume);
        }
        if (configWindow.index == 2 && motaSystem.SEVolume < 100)  {
            ++motaSystem.SEVolume;
            playSE(motaSystem.cursorSE, motaSystem.SEVolume);
        }
        return;
    }

    // 按下取消键时
    if (configWindow.active && Input::triggerCancel()) {
        playSE(motaSystem.cancelSE, motaSystem.SEVolume);

        // 关闭设置窗口
        configWindow.visible = false;

        // 恢复菜单活动
        menuWindow.active = true;
        return;
    }
}

void MotaMap::updateItem() {
    // 按下确认键时
    if (Input::triggerConfirm()) {
        if (itemWindow.myItems.empty()) return;
        if (itemWindow.myItems[itemWindow.index].usable) {
            playSE(motaSystem.decisionSE, motaSystem.SEVolume);

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
        else {
            playSE(motaSystem.buzzerSE, motaSystem.SEVolume);
        }
        return;
    }

    // 按下取消键时
    if (Input::triggerCancel()) {
        playSE(motaSystem.cancelSE, motaSystem.SEVolume);

        // 关闭物品窗口
        itemWindow.visible = false;

        // 如果菜单窗口可见，就恢复其活动
        if (menuWindow.visible) menuWindow.active = true;
        return;
    }
}

void MotaMap::updateLoadSave() {
    // 按下确认键时
    if (Input::triggerConfirm()) {
        // 如果是读档
        if (loadSaveWindow.load) {
            // 如果存档文件存在
            if (std::filesystem::exists(std::format("save\\save_{}.json", loadSaveWindow.index))) {
                playSE(motaSystem.loadSE, motaSystem.SEVolume);

                // 关闭存读档窗口
                loadSaveWindow.visible = false;

                // 关闭菜单窗口
                menuWindow.visible = false;

                // 打开状态栏窗口
                motaTemp.closeMS = false;

                // 加载动画
                GameImage stairImg("picture\\mting.png");
                motaGraphics.addImage(&stairImg);
                std::tie (stairImg.x, stairImg.y, stairImg.opacity, stairImg.z) = std::make_tuple(MAPX, MAPY, 0, 2);
                for (int i = 0; i < 15; ++i) {
                    stairImg.opacity += 17;
                    screenData.waitCount(1);
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
                    screenData.waitCount(1);
                }
                motaGraphics.eraseImage(&stairImg);
                return;
            }
            else {
                playSE(motaSystem.buzzerSE, motaSystem.SEVolume);
                return;
            }
        }
        else {
            playSE(motaSystem.saveSE, motaSystem.SEVolume);
            screenData.saveData(loadSaveWindow.index);
            saveImage.saveToFile(std::format("save\\save_{}.png", loadSaveWindow.index));
            saveFile("save\\recent.dat", std::to_string(loadSaveWindow.index));

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
    if (Input::triggerCancel()) {
        playSE(motaSystem.cancelSE, motaSystem.SEVolume);

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
    if (Input::triggerCancel()) {
        playSE(motaSystem.cancelSE, motaSystem.SEVolume);

        // 关闭快捷键提示窗口
        hintWindow.visible = false;
    }
}

void MotaMap::updateShop() {
    // 按下取消键时
    if (Input::triggerCancel()) {
        playSE(motaSystem.cancelSE, motaSystem.SEVolume);

        // 关闭商店窗口
        shopWindow.visible = false;
        return;
    }
}

void MotaMap::updateMessage() {
    // 显示对话窗口
    messageWindow.visible = true;

    // 按下确认键时
    if (Input::triggerConfirm()) {
        // 如果这是个带选择项的窗口
        if (messageWindow.haveIndex) {
            // 如果有指令的话
            if (!messageWindow.order[messageWindow.index].empty()) {
                screenData.doOrder(messageWindow.order[messageWindow.index]);
            }
            messageWindow.index = -1;
        }

        // 将本次对话从队列中清除
        motaTemp.messageInfo.erase(motaTemp.messageInfo.begin());

        // 将不透明度清零防止闪烁
        messageWindow.opacity = 0;

        // 如果队列清空了，就关闭对话窗口显示
        if (motaTemp.messageInfo.empty()) {
            messageWindow.visible = false;
            motaTemp.pause = false;
        }
        screenData.waitCount(2);
        ++motaTemp.voiceInfo.second;
        motaTemp.voice.stop();
        if (std::filesystem::exists(std::format("assets\\voice\\npc_{}_{}.ogg", motaTemp.voiceInfo.first, motaTemp.voiceInfo.second))) {
            motaTemp.voiceBuffer.loadFromFile(std::format("assets\\voice\\npc_{}_{}.ogg", motaTemp.voiceInfo.first, motaTemp.voiceInfo.second));
            motaTemp.voice.setBuffer(motaTemp.voiceBuffer);
            motaTemp.voice.setVolume(motaSystem.SEVolume);
            motaTemp.voice.play();
        }
    }
}

void MotaMap::updateEnemyBook() {
    // 按下确认键时
    if (Input::triggerConfirm() && !motaTemp.floorEnemies.empty()) {
        playSE(motaSystem.decisionSE, motaSystem.SEVolume);

        // 关闭怪物手册窗口
        enemyBookWindow.visible = false;

        // 录入怪物信息，打开怪物百科窗口
        encyWindow.mon = motaTemp.floorEnemies[enemyBookWindow.nowPage * 4 + enemyBookWindow.index];
        encyWindow.visible = true;
        return;
    }
    // 按下取消键时
    if (Input::triggerCancel()) {
        playSE(motaSystem.cancelSE, motaSystem.SEVolume);

        // 关闭怪物手册窗口
        enemyBookWindow.visible = false;

        // 如果是从物品栏来的，就打开物品栏窗口
        if (enemyBookWindow.from == 1) itemWindow.visible = true;
        return;
    }
}

void MotaMap::updateEncyclopedia() {
    // 按下取消键时
    if (Input::triggerCancel()) {
        playSE(motaSystem.cancelSE, motaSystem.SEVolume);

        // 关闭怪物百科窗口，打开怪物手册窗口
        enemyBookWindow.visible = true;
        encyWindow.visible = false;
        return;
    }
}

void MotaMap::updateFly() {
    // 按下确认键时
    if (Input::triggerConfirm()) {
        playSE(motaSystem.stairSE, motaSystem.SEVolume);

        // 关闭楼层传送器窗口
        flyWindow.visible = false;

        // 获取目标地图信息
        auto& var = motaVariables.variables;
        auto& flr = motaVariables.floorRecord[var[1]];
        auto& act = screenData.actors[motaVariables.variables[0]];
        auto targetMap = motaData.maps[motaData.searchMap(std::format("{}:{}", var[1], flr[flyWindow.viewFloorIndex]))];

        // 传送至指定楼层
        if (flr[flyWindow.viewFloorIndex] > var[2]) {
            // 如果是上楼
            var[2] = flr[flyWindow.viewFloorIndex] - 1;

            // 坐标调整至目标地图下楼梯位置
            Interpreter(std::format("up({},{})", targetMap.mapEvents[0].x, targetMap.mapEvents[0].y)).execute();
        }
        else if (flr[flyWindow.viewFloorIndex] < var[2]) {
            // 如果是下楼
            var[2] = flr[flyWindow.viewFloorIndex] + 1;

            // 坐标调整至目标地图上楼梯位置
            Interpreter(std::format("down({},{})", targetMap.mapEvents[1].x, targetMap.mapEvents[1].y)).execute();
        }
        else {
            // 如果是同楼
            if (var[2] >= 0) {
                // 地上同层传送到下楼梯位置
                Interpreter(std::format("move({},{},{})", act.mapID, targetMap.mapEvents[0].x, targetMap.mapEvents[0].y)).execute();
            }
            else {
                // 地下同层传送到上楼梯位置
                Interpreter(std::format("move({},{},{})", act.mapID, targetMap.mapEvents[1].x, targetMap.mapEvents[0].y)).execute();
            }
        }
        return;
    }

    // 按下取消键时
    if (Input::triggerCancel()) {
        playSE(motaSystem.cancelSE, motaSystem.SEVolume);

        // 关闭楼层传送器窗口
        flyWindow.visible = false;
        return;
    }
}

void MotaMap::setShop() {
    shopWindow.visible = true;
    shopWindow.index = 0;
    shopWindow.items.clear();

    auto setInfo = [&](std::string name, std::string file, std::string desc, int pos) {
        shopWindow.name = move(name);
        shopWindow.file = move(file);
        shopWindow.desc = move(desc);
        shopWindow.pos = pos;
    };

    auto setItem = [&](const std::string& name, int* ref, std::string cost, std::vector <std::string> order = {}) {
        if (ref == &screenData.actors[motaVariables.variables[0]].gold) {
            order.emplace_back(std::format("bonus(5,-{});", cost));
        }
        else if (ref == &screenData.actors[motaVariables.variables[0]].exp) {
            order.emplace_back(std::format("bonus(4,-{});", cost));
        }
        shopWindow.items.emplace_back(name,
                                      std::make_pair(ref, cost),
                                      order);
    };

    if (motaTemp.shopType == 0) {
        // 贪婪之神
        setInfo("贪婪之神", screenData.visualMap.mapEvents[motaTemp.functionEventID].file, std::format("愚蠢的人类，如果你能提供{}个金币，我将可以提升你的力量！", motaTemp.initPrice[0]), screenData.visualMap.mapEvents[motaTemp.functionEventID].pos[1]);
        std::string desc[] = {"生命值", "攻击力", "防御力"};
        std::string var = motaTemp.initPrice[0];
        replaceAll(var, "[v", "");
        replaceAll(var, "]", "");
        for (int i = 0; i < 3; ++i) {
            setItem(std::format("增加{}点{}", motaTemp.addPower[i], desc[i]),
                    &screenData.actors[motaVariables.variables[0]].gold, motaTemp.initPrice[0],
                    {std::format("bonus({},{});", i, motaTemp.addPower[i]), // 增加能力
                     std::format("var({},+,{});", var, motaTemp.rise)});// 涨价
        }
    }
    else if (motaTemp.shopType == 1) {
        // 战斗之神
        setInfo("战斗之神", screenData.visualMap.mapEvents[motaTemp.functionEventID].file, "英雄的人类！如果你可以给我提供经验，我将可以提升你的力量！", screenData.visualMap.mapEvents[motaTemp.functionEventID].pos[1]);
        std::string desc[] = {"等级", "攻击", "防御"};
        int kind[] = {6, 1, 2};
        for (int i = 0; i < 3; ++i) {
            shopWindow.items.emplace_back(std::format("{}经验增加{}个{}",motaTemp.initPrice[i], motaTemp.addPower[i], desc[i]),
                                          std::make_pair(&screenData.actors[motaVariables.variables[0]].exp, motaTemp.initPrice[i]),
                                          std::vector <std::string> ({std::format("bonus(4,-{});", motaTemp.initPrice[i]), // 扣除经验
                                                            std::format("bonus({},{});", kind[i], motaTemp.addPower[i])})); // 增加等级
        }
    }

    // 重置商店类型
    motaTemp.shopType = -1;
}

void MotaMap::setBattle() {
    // 添加双方动画
    auto& act = screenData.actors[motaVariables.variables[0]];
    auto& ev = screenData.visualMap.mapEvents[motaTemp.functionEventID];
    auto& en = motaData.enemies[motaTemp.battleEnemyID];
    screenData.addEVAnimation(act.animationID, ev.x, ev.y);
    screenData.waitCount(motaData.animations[act.animationID].patterns.size());
    screenData.addEVAnimation(en.animationID, act.x, act.y);
    screenData.waitCount(motaData.animations[en.animationID].patterns.size());

    // 计算伤害
    int damage = en.getDamage(&act, &motaData.elements);

    // 如果返回值为-1代表不可战胜
    if (damage == -1) damage = act.hp;

    // 当我方生命大于对方生命时正常扣血
    if (act.hp > damage) {
        act.hp -= damage;
        act.exp += en.exp;
        act.gold += en.gold * (1 + (act.item[12] > 0));
        ev.toDispose = true;
        // 有[v]需要在战斗后使用的话，写在这里
        for (int i = 0, cnt = 1; i < en.element.size(); ++i) {
            if (motaData.elements[en.element[i]].haveBuff) {
                float buff = stof(split(en.name, "/")[cnt]);
                if (en.element[i] == 1) {
                    // 中毒的效果
                    motaVariables.variables[5] += buff;
                    act.status.insert(1);
                }
                else if (en.element[i] == 2) {
                    // 衰弱的效果
                    motaVariables.variables[6] += buff;
                    act.status.insert(2);
                }
                else if (en.element[i] == 8) {
                    // 再生的效果
                    motaTemp.transEventName = std::format("monster({});", (int)buff); // 更改事件名
                    ev.toDispose = false; // 不需要消失
                }

                cnt++;
            }
        }
    }
    else {
        // 如果不敌则游戏结束
        act.hp = 0;
        Interpreter("gg").execute();
    }

    // 清空对战斗事件的记录
    motaTemp.battleEnemyID = -1;
    // 取消暂停，继续执行事件指令
    motaTemp.pause = false;
}

void MotaMap::setTrans() {
    // 更新事件名
    auto& ev = screenData.visualMap.mapEvents[motaTemp.functionEventID];
    ev.name = motaTemp.transEventName;

    // 特殊更改的处理
    auto namelist = Interpreter::initEventName(motaTemp.transEventName);
    for (const auto& eachname : namelist) {
        auto namesplt = Interpreter::initOrder(eachname);
        if (namesplt[0] == "monster") {
            // 更改成了怪物
            ev.file = motaData.enemies[stoi(namesplt[1])].file;
            ev.pos[1] = motaData.enemies[stoi(namesplt[1])].pos;
            ev.move = true;
            ev.through = false;
        }
    }

    // 记录更新事件
    motaVariables.transRecord[screenData.visualMap.mapID][motaTemp.functionEventID] = motaTemp.transEventName;

    // 清空更新事件名
    motaTemp.transEventName = "";
    // 如果直接触发新事件
    if (motaTemp.directlyFunction) {
        motaTemp.directlyFunction = false;
        screenData.player.triggerEvent(&ev);
    }
}

void MotaMap::autoSearching(std::pair <int, int> target) {
    // bfs
    std::vector <std::vector <bool> > maprec(11, std::vector<bool>(11, true));
    auto findPath = [&](int startX, int startY, int targetX, int targetY) {
        std::vector<int> directions = {0, 1, 2, 3}; // 下、左、右、上
        std::vector<std::vector<int>> moves = {{1, 0}, {0, -1}, {0, 1}, {-1, 0}}; // 下、左、右、上 对应的坐标变化

        std::vector<std::vector<bool>> visited(maprec.size(), std::vector<bool>(maprec[0].size(), false)); // 记录是否已访问过
        std::vector<std::vector<int>> prev(maprec.size(), std::vector<int>(maprec[0].size(), -1)); // 记录上一步的移动方向

        std::queue<std::pair<int, int>> q;
        q.push({startX, startY});
        visited[startY][startX] = true;

        while (!q.empty()) {
            int x = q.front().first;
            int y = q.front().second;
            q.pop();

            if (x == targetX && y == targetY) {
                // 找到路径，回溯构建路径
                std::vector<int> path;
                while (x != startX || y != startY) {
                    int prevDir = prev[y][x];
                    path.push_back(prevDir);
                    x -= moves[prevDir][1];
                    y -= moves[prevDir][0];
                }
                reverse(path.begin(), path.end()); // 将路径反转，得到正确的顺序
                return path;
            }

            // 尝试四个方向的移动
            for (int i = 0; i < 4; ++i) {
                int newX = x + moves[i][1];
                int newY = y + moves[i][0];

                if (newX >= 0 && newX < maprec[0].size() && newY >= 0 && newY < maprec.size() &&
                    maprec[newY][newX] && !visited[newY][newX]) {
                    q.push({newX, newY});
                    visited[newY][newX] = true;
                    prev[newY][newX] = i; // 记录移动方向
                }
            }
        }

        return std::vector <int>(); // 如果找不到路径，返回空路径
    };

    for (auto ev : screenData.visualMap.mapEvents) {
        if (ev.exist) {
            if (!ev.through) {
                if (!(ev.triggerCondition[0] == 1 && motaVariables.variables[ev.triggerCondition[1]] != ev.triggerCondition[2])) {
                    maprec[ev.y][ev.x] = false;
                }
            }
        }
    }

    auto path = findPath(screenData.actors[motaVariables.variables[0]].x, screenData.actors[motaVariables.variables[0]].y, target.first, target.second);
    screenData.searchingRoad = !path.empty();
    while (!path.empty()) {
        screenData.player.update(path.front());
        screenData.waitCount(1);
        if (Input::leftClick()) {
            screenData.searchingRoad = false;
            path.clear();
            break;
        }
        if (screenData.player.movingCount == 0) {
            path.erase(path.begin());
            if (path.empty()) {
                screenData.searchingRoad = false;
            }
        }
    }
}

void MotaScript::main() {
    auto content = readFile(motaTemp.ending ? "ref\\end.txt" : "ref\\begin.txt");
    std::string allcontent = "";
    for (const auto& ctt : content) {
        allcontent += ctt + "\n";
    }
    text.setText(&motaSystem.font, allcontent);
    text.x = (640 - text.getSize().first) / 2;
    text.y = 480;
    text.z = 3;
    motaGraphics.addText(&text);
    back.setSprite("system\\script-back.png");
    back.z = 4;
    motaGraphics.addImage(&back);
    space.setSprite("system\\script-space.png");
    space.x = 270;
    space.y = 450;
    space.z = 4;
    space.opacity = 135;
    motaGraphics.addImage(&space);

    // 执行渐变
    screenData.transition1();

    // 主循环
    while (motaSystem.scene == this && motaSystem.window.isOpen()) {
        // 更新画面
        screenData.waitCount(1);

        // 执行更新
        if (motaSystem.window.hasFocus()) {
            update();
        }
    }

    // 执行渐变
    screenData.transition2();
    motaGraphics.eraseText(&text);
    motaGraphics.eraseImage(&back);
    motaGraphics.eraseImage(&space);
}

void MotaScript::update() {
    // 更新文字位置
    text.y -= 2;
    space.opacity = abs((int)(120 - 20 * ((motaSystem.gameTime / (2 * (1 + motaSystem.frameRate))) % 7))) + 135;
    if (Input::triggerConfirm() || text.y <= 0 - text.getSize().second) {
        if (motaTemp.ending) motaSystem.scene = new MotaTitle;
        else motaSystem.scene = new MotaMap;
    }
}
