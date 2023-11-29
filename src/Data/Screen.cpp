#include <Game/Data/Screen.hpp>

ScreenData screenData;

Interpreter::Interpreter(string order) {
    this->order = std::move(order);
}

void Interpreter::execute(Object* obj) {
    // 渐变动画
    GameImage stairImg("system\\mting.png");
    auto move1 = [&]() {
        playSE(motaSystem.stairSE, motaSystem.SEVolume);
        motaGraphics.addImage(&stairImg);
        tie (stairImg.x, stairImg.y, stairImg.opacity, stairImg.z) = make_tuple(MAPX, MAPY, 0, 2);
        for (int i = 0; i < 15; ++i) {
            stairImg.opacity += 17;
            screenData.waitCount(1);
        }
    };
    auto move2 = [&]() {
        for (int i = 0; i < 15; ++i) {
            stairImg.opacity -= 17;
            screenData.waitCount(1);
        }
        motaGraphics.eraseImage(&stairImg);
    };

    // 在这里设置各种事件指令
    // 提前复制角色信息，简化代码
    auto& act = screenData.actors[motaVariables.variables[0]];

    // 将事件名按/号分割
    auto info = split(order, "/");
    int kind = 0, val = 0;

    // 游戏结束awsl
    if (info[0] == "gg") {
        motaTemp.messageInfo.emplace_back(-1, "勇士", "怎能…\n倒在这里……");
        motaTemp.gameOver = true;
        motaTemp.nextMove = true;
    }

    // 战斗
    if (info[0] == "monster") {
        motaTemp.battleEnemyID = stoi(initDialogue(info[1]));
        motaTemp.nextMove = true;
    }

    // 宝石血瓶
    if (info[0] == "bonus") {
        if (motaTemp.functionEventID != -1) playSE(motaSystem.getSE, motaSystem.SEVolume);
        tie(kind, val) = make_pair(stoi(initDialogue(info[1])), stoi(initDialogue(info[2])));
        if (kind == 0) act.hp += val;
        if (kind == 1) act.atk += val;
        if (kind == 2) act.def += val;
        if (kind == 3) act.mdef += val;
        if (kind == 4) act.exp += val;
        if (kind == 5) act.gold += val;
        if (kind == 6) {
            act.level += val;
            act.hp += val * 400;
            act.atk += val * 5;
            act.def += val * 5;
        }
        if (obj != nullptr) obj->toDispose = true;
        motaTemp.nextMove = true;
    }

    // 开门
    if (info[0] == "door") {
        auto openDoor = [&]() {
            for (int i = 0; i < 3; ++i) {
                ++obj->pos[1];
                screenData.waitCount(1);
            }
        };

        kind = stoi(initDialogue(info[1]));
        if (kind == 0) {
            playSE(motaSystem.gateSE, motaSystem.SEVolume);
            openDoor();
            if (obj != nullptr) obj->toDispose = true;
        }
        if (kind == 1 && act.item[0] > 0) {
            --act.item[0];
            playSE(motaSystem.gateSE, motaSystem.SEVolume);
            openDoor();
            if (obj != nullptr) obj->toDispose = true;
        }
        if (kind == 2 && act.item[1] > 0) {
            --act.item[1];
            playSE(motaSystem.gateSE, motaSystem.SEVolume);
            openDoor();
            if (obj != nullptr) obj->toDispose = true;
        }
        if (kind == 3 && act.item[2] > 0) {
            --act.item[2];
            playSE(motaSystem.gateSE, motaSystem.SEVolume);
            openDoor();
            if (obj != nullptr) obj->toDispose = true;
        }
        motaTemp.nextMove = true;
    }

    // 物品
    if (info[0] == "item") {
        if (motaTemp.functionEventID != -1) playSE(motaSystem.getSE, motaSystem.SEVolume);
        tie(kind, val) = make_pair(stoi(initDialogue(info[1])), stoi(initDialogue(info[2])));
        if (!motaVariables.itemRecord[kind] || val != 1) {
            motaVariables.itemRecord[kind] = true;
            motaTemp.messageInfo.emplace_back(-4, to_string(kind), "");
        }
        act.item[kind] += val;
        if (obj != nullptr) obj->toDispose = true;
        motaTemp.nextMove = true;
    }

    // 商店
    if (info[0] == "shop") {
        motaTemp.shopType = stoi(info[1]);
        motaTemp.addPower = split(info[2], "&");
        if (info.size() >= 4) motaTemp.initPrice = split(info[3], "&");
        else motaTemp.initPrice = { "0" };
        if (info.size() >= 5) motaTemp.rise = info[4];
        else motaTemp.rise = "0";
        motaTemp.nextMove = true;
    }

    // npc
    if (info[0] == "npc") {
        kind = stoi(info[1]);
        for (auto [messID, messName, messContent] : motaData.npc[kind].npcInfo) {
            motaTemp.messageInfo.emplace_back(messID, messName, insertNewLines(initDialogue(messContent), 28));
        }
        if (!motaData.npc[kind].transName.empty()) {
            motaTemp.transEventName = motaData.npc[kind].transName;
            motaTemp.directlyFunction = motaData.npc[kind].directlyFunction;
        }
        if (obj != nullptr) obj->toDispose = motaData.npc[kind].fade;
        motaTemp.nextMove = true;
    }

    // 路障熔岩
    if (info[0] == "lava") {
        val = stoi(initDialogue(info[1]));
        act.hp = max(0, act.hp - val);
        if (act.hp == 0) Interpreter("gg").execute();
        motaTemp.nextMove = true;
    }

    // 上楼
    if (info[0] == "up") {
        move1();
        string temp = format("{}:{}", motaVariables.variables[1], motaVariables.variables[2] + 1);
        if (int tgtmpid = motaData.searchMap(temp); tgtmpid != -1) {
            ++motaVariables.variables[2];
            act.mapID = tgtmpid;
            if (info.size() == 3) { // 当传送有目标xy时
                tie(kind, val) = make_pair(stoi(info[1]), stoi(info[2]));
                act.x = kind;
                act.y = val;
            }
            screenData.player.direction = 0;
            if (!vectorFind(motaVariables.floorRecord[motaVariables.variables[1]], motaVariables.variables[2])) {
                motaVariables.floorRecord[motaVariables.variables[1]].push_back(motaVariables.variables[2]);
                sort(motaVariables.floorRecord[motaVariables.variables[1]].begin(), motaVariables.floorRecord[motaVariables.variables[1]].end());
            }
            screenData.loadMap(tgtmpid, &screenData.visualMap);
        }
        else {
            print("there's no such a map!");
        }
        move2();
        motaTemp.nextMove = true;
    }

    // 下楼
    if (info[0] == "down") {
        move1();
        string temp = format("{}:{}", motaVariables.variables[1], motaVariables.variables[2] - 1);
        if (int tgtmpid = motaData.searchMap(temp); tgtmpid != -1) {
            --motaVariables.variables[2];
            act.mapID = tgtmpid;
            if (info.size() == 3) { // 当传送有目标xy时
                tie(kind, val) = make_pair(stoi(info[1]), stoi(info[2]));
                act.x = kind;
                act.y = val;
            }
            screenData.player.direction = 0;
            if (!vectorFind(motaVariables.floorRecord[motaVariables.variables[1]], motaVariables.variables[2])) {
                motaVariables.floorRecord[motaVariables.variables[1]].push_back(motaVariables.variables[2]);
                sort(motaVariables.floorRecord[motaVariables.variables[1]].begin(), motaVariables.floorRecord[motaVariables.variables[1]].end());
            }
            screenData.loadMap(tgtmpid, &screenData.visualMap);
        }
        else {
            print("there's no such a map!");
        }
        move2();
        motaTemp.nextMove = true;
    }

    // 传送
    if (info[0] == "move") {
        move1();
        int mpid = stoi(info[1]), mx = stoi(info[2]), my = stoi(info[3]);
        act.mapID = mpid;
        act.x = mx;
        act.y = my;
        screenData.player.direction = 0;
        screenData.loadMap(mpid, &screenData.visualMap);
        move2();
        motaTemp.nextMove = true;
    }

    // 修改变量
    if (info[0] == "var") {
        tie(kind, val) = make_pair(stoi(info[1]), stoi(info[2]));
        motaVariables.variables[kind] += val;
        motaTemp.nextMove = true;
    }

    // 消除事件
    if (info[0] == "erase") {
        screenData.visualMap.mapEvents[motaTemp.functionEventID].toDispose = true;
        motaTemp.nextMove = true;
    }

    // 返回标题画面
    if (info[0] == "back") {
        motaTemp.gameOver = true;
        motaTemp.nextMove = true;
    }

    // 结局
    if (info[0] == "ending") {
        motaTemp.ending = true;
        motaTemp.nextMove = true;
    }

    // 下面是条件等对话相关的
    auto condName = split(initDialogue(order), "~");
    auto judgecond = [&](int a, int b, const string& s) {
        bool result = false;
        if (s == "==") result = (a == b);
        if (s == "!=") result = (a != b);
        if (s == ">") result = (a > b);
        if (s == "<") result = (a < b);
        if (s == ">=") result = (a >= b);
        if (s == "<=") result = (a <= b);
        motaSystem.scene->update();
        return result;
    };

    // 条件
    if (condName[0] == "condition") {
        auto [x, y, z, p, q] = make_tuple(stoi(condName[1]), condName[2], stoi(condName[3]), split(condName[4], "->"), split(condName[5], "->"));
        if (judgecond(x, z, y)) screenData.doOrder(p);
        else screenData.doOrder(q);
        motaTemp.nextMove = true;
    }

    // 转换事件名
    if (condName[0] == "switch") {
        motaTemp.transEventName = condName[1];
        motaTemp.nextMove = true;
    }
}

string Interpreter::replaceToVar(const string& source) {
    string result = source;
    regex pattern("\\[([0-9]+)\\]"); // 匹配形如"[x]"的字符串
    smatch match;
    while (regex_search(result, match, pattern)) {
        if (match.size() == 2) {
            int index = stoi(match[1]);
            string change;
            if (motaVariables.variables.contains(index)) {
                change = to_string(motaVariables.variables[index]);
            }
            else{
                change = "0";
            }
            result = match.prefix().str() + change + match.suffix().str();
        }
    }
    return result;
}

string Interpreter::initDialogue(const string& source) {
    string result = replaceToVar(source);
    auto& act = screenData.actors[motaVariables.variables[0]];
    replaceAll(result, "[hp]", to_string(act.hp));
    replaceAll(result, "[atk]", to_string(act.atk));
    replaceAll(result, "[def]", to_string(act.def));
    replaceAll(result, "[mdef]", to_string(act.mdef));
    replaceAll(result, "[exp]", to_string(act.exp));
    replaceAll(result, "[gold]", to_string(act.gold));
    return result;
}

void Player::update() {
    // 达到条件自动执行的事件
    for (auto& ev : screenData.visualMap.mapEvents) {
        if (!ev.exist) continue;
        if (ev.triggerCondition[0] == 3 && motaVariables.variables[ev.triggerCondition[1]] == ev.triggerCondition[2]) {
            triggerEvent(&ev);
        }
    }

    // 行走中快捷键不生效
    if (movingCount > 0) return;

    // 根据四方向判定
    int dir = motaKeyBoard.dir4();
    direction = dir >= 0 ? dir : direction;
    if (dir == -1) return;

    // 根据方向判定行走
    int dirchg[4][2] = {{0, 1}, {-1, 0}, {1, 0}, {0, -1}};
    auto& act = screenData.actors[motaVariables.variables[0]];
    if (screenData.visualMap.passible(act.x + dirchg[dir][0], act.y + dirchg[dir][1])) {
        ++step;
        if (movingCount == 0) movingCount = 4;
        act.x += dirchg[dir][0];
        act.y += dirchg[dir][1];
        changeSteps();
        if (screenData.visualMap.haveAnEvent(act.x, act.y)) {
            while (movingCount > 0) {
                screenData.waitCount(1);
            }
            triggerEvent(screenData.visualMap.EcheckEvent(act.x, act.y));
        }
    }
    else {
        if (screenData.visualMap.haveAnEvent(act.x + dirchg[dir][0], act.y + dirchg[dir][1])) {
            triggerEvent(screenData.visualMap.EcheckEvent(act.x + dirchg[dir][0], act.y + dirchg[dir][1]));
        }
    }
}

void Player::changeSteps() {
    // 设置方位
    auto& act = screenData.actors[motaVariables.variables[0]];
    auto [ax, ay] = make_pair(act.x, act.y);
    int dir[4][2] = {{0, 1}, {0, -1}, {-1, 0}, {1, 0}};

    // 领域怪
    for (auto & i : dir) {
        if (auto [nx, ny] = make_pair(ax + i[0], ay + i[1]);screenData.visualMap.haveAnEvent(nx, ny)) {
            auto ev = screenData.visualMap.EcheckEvent(nx, ny);
            auto namelist = split(ev->name, "<>");
            for (auto evname : namelist) {
                if (auto names = split(ev->name, "/"); ev->exist && names[0] == "monster") {
                    if (auto en = motaData.enemies[stoi(names[1])]; en.getP(9)) {
                        if (int dmg = en.getSingleDamage(act.getDef()) * (100 + motaVariables.variables[5]) / 100.f; dmg > 0) {
                            screenData.addEVAnimation(en.animationID, ax, ay);
                            act.hp = max(0, act.hp - dmg);
                            if (act.hp == 0) {
                                Interpreter("gg").execute();
                            }
                        }
                    }
                }
            }
        }
    }

    // 夹击怪
    for (int i = 0; i < 2; ++i) {
        if (auto [nx1, ny1, nx2, ny2] = make_tuple(ax + dir[2 * i][0], ay + dir[2 * i][1], ax + dir[2 * i + 1][0], ay + dir[2 * i + 1][1]); screenData.visualMap.haveAnEvent(nx1, ny1) && screenData.visualMap.haveAnEvent(nx2, ny2)) {
            auto ev1 = screenData.visualMap.EcheckEvent(nx1, ny1), ev2 = screenData.visualMap.EcheckEvent(nx2, ny2);
            auto namelist1 = split(ev1->name, "<>"), namelist2 = split(ev2->name, "<>");
            for (const auto& evname1 : namelist1) {
                for (const auto& evname2 : namelist2) {
                    if (auto names1 = split(evname1, "/"), names2 = split(evname2, "/"); ev1->exist && ev2->exist && names1[0] == "monster" && names2[0] == "monster" && names1 == names2) {
                        if (auto en = motaData.enemies[stoi(names1[1])]; en.getP(10)) {
                            if (int dmg = en.getSingleDamage(act.getDef()) * 2 * (100 + motaVariables.variables[5]) / 100.f; dmg > 0) {
                                screenData.addEVAnimation(en.animationID, ax, ay);
                                act.hp = max(0, act.hp - dmg);
                                if (act.hp == 0) {
                                    Interpreter("gg").execute();
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void Player::triggerEvent(Object* ev) {
    // 不存在的事件就不执行了
    if (!ev->exist) return;

    // 未达到条件不可执行的
    if (ev->triggerCondition[0] != 0 && motaVariables.variables[ev->triggerCondition[1]] != ev->triggerCondition[2]) return;

    // 记录ID
    motaTemp.functionEventID = ev->ID;

    // 分裂名字，执行事件指令
    screenData.doOrder(split(ev->name, "<>"), ev);
};

ScreenData::ScreenData() {
    transition.setSprite("system\\mting-trans.png");
    transition.opacity = 0;
    transition.z = 9;
    motaGraphics.addImage(&transition);
    onMap = false;
}

void ScreenData::init() {
    // 初始化数据
    player.movingCount = 0;
    player.direction = 0;
    player.step = 0;
    player.visible = true;
    actors.clear();
    actors = motaData.actors;
}

void ScreenData::loadMap(int mapID, Map* gmap) {
    // 将当前地图加载进gmap
    *gmap = motaData.maps[mapID];

    // 根据记录将部分事件清除
    if (motaVariables.eventRecord.contains(mapID) && !motaVariables.eventRecord[mapID].empty()) {
        for (auto it: motaVariables.eventRecord[mapID]) {
            gmap->mapEvents[it].exist = false;
        }
    }

    // 根据记录将部分事件名进行转换
    if (motaVariables.transRecord.count(mapID) > 0 && !motaVariables.transRecord[mapID].empty()) {
        for (auto [evid, evname] : motaVariables.transRecord[mapID]) {
            gmap->mapEvents[evid].name = evname;
            auto namelist = split(evname, "<>");
            for (const auto& eachname : namelist) {
                auto namesplt = split(eachname, "/");
                if (namesplt[0] == "monster") {
                    gmap->mapEvents[evid].file = motaData.enemies[stoi(namesplt[1])].file;
                    gmap->mapEvents[evid].pos[1] = motaData.enemies[stoi(namesplt[1])].pos;
                    gmap->mapEvents[evid].move = true;
                    gmap->mapEvents[evid].through = false;
                }

            }
        }
    }
}

void ScreenData::mapStatus() {
    // 绘制状态栏的Lambda表达式
    auto drawStatusText = [](const vector <pair <string, int> >& text, int y) {
        for (auto [txt, val] : text) {
            drawText(&motaSystem.window, &motaSystem.font, IntRect(16, y, 160, 24), txt);
            drawText(&motaSystem.window, &motaSystem.font, IntRect(16, y, 160, 24), to_string(val), 2);
            y += 24;
        }
    };

    // 提前复制主角信息
    auto& act = actors[motaVariables.variables[0]];

    // 绘制背景
    drawImage(&motaSystem.window, &motaSystem.textureCache["system\\mapstatus.png"], 0, 0);

    // 关闭状态栏时返回
    if (motaTemp.closeMS || motaVariables.variables[7]) return;

    // 绘制分割线条
    drawImage(&motaSystem.window, &motaSystem.textureCache["system\\mapstatus_line.png"], 0, 0);

    // 根据魔塔编号设置名字
    drawText(&motaSystem.window, &motaSystem.font, IntRect(224, 0, 352, 64), strInclude(visualMap.mapName, ":") ? format("{} {} 层", motaData.motaName[motaVariables.variables[1]], motaVariables.variables[2]) : visualMap.mapName, 1, 28L, true);

    // 绘制主角icon
    drawImage(&motaSystem.window, &motaSystem.textureCache["character\\" + act.file], 16, 32, IntRect(0, 0, 32, 32));
    drawText(&motaSystem.window, &motaSystem.font, IntRect(64, 32, 96, 32), act.name);
    drawText(&motaSystem.window, &motaSystem.font, IntRect(96, 32, 96, 32), format("Lv. {}", act.level), 2);

    // 状态显示
    string status[] = {"", "中毒", "衰弱"}, statustxt = "";
    for (auto s : act.status)
        statustxt += "|" + status[s];
    if (!statustxt.empty()) statustxt.erase(statustxt.begin());
    drawText(&motaSystem.window, &motaSystem.font, IntRect(16, 72, 160, 24), "状态");
    drawText(&motaSystem.window, &motaSystem.font, IntRect(16, 72, 160, 24), format("[{}]", act.status.empty() ? "正常" : statustxt), 2);

    // 绘制状态栏
    drawStatusText({make_pair("生命", act.hp),
                    make_pair("攻击", act.atk),
                    make_pair("防御", act.def),
                    make_pair("魔防", act.mdef)}, 120);
    if (act.status.contains(1)) {
        drawText(&motaSystem.window, &motaSystem.font, IntRect(64, 128, 96, 16), format("({})", motaVariables.variables[5]), 0, 12L, false, Color::Green);
    }
    if (act.status.contains(2)) {
        drawText(&motaSystem.window, &motaSystem.font, IntRect(64, 152, 96, 16), format("(-{})", motaVariables.variables[6]), 0, 12L, false, Color::Magenta);
        drawText(&motaSystem.window, &motaSystem.font, IntRect(64, 176, 96, 16), format("(-{})", motaVariables.variables[6]), 0, 12L, false, Color::Magenta);
    }
    drawStatusText({make_pair("经验", act.exp),
                    make_pair("金币", act.gold)}, 240);

    // 绘制钥匙
    drawImage(&motaSystem.window, &motaSystem.textureCache["character\\" + motaData.items[0].file], 16, 312, IntRect(32 * motaData.items[0].pos[0], 32 * motaData.items[0].pos[1], 32, 32));
    drawImage(&motaSystem.window, &motaSystem.textureCache["character\\" + motaData.items[1].file], 16, 336, IntRect(32 * motaData.items[1].pos[0], 32 * motaData.items[1].pos[1], 32, 32));
    drawImage(&motaSystem.window, &motaSystem.textureCache["character\\" + motaData.items[2].file], 16, 360, IntRect(32 * motaData.items[2].pos[0], 32 * motaData.items[2].pos[1], 32, 32));
    drawText(&motaSystem.window, &motaSystem.font, IntRect(16, 312, 160, 24), to_string(act.item[0]), 2);
    drawText(&motaSystem.window, &motaSystem.font, IntRect(16, 336, 160, 24), to_string(act.item[1]), 2);
    drawText(&motaSystem.window, &motaSystem.font, IntRect(16, 360, 160, 24), to_string(act.item[2]), 2);

    // 有ShortcutKey.txt则绘制Press L
    if (filesystem::exists("ref\\ShortcutKey.txt")) drawText(&motaSystem.window, &motaSystem.font, IntRect(0, 456, 624, 24), "～Press L～", 2);
}

void ScreenData::showMap(const Map& gmap, float x, float y, float rate, bool visible, bool clear_device) {
    // 绘制地图事件的函数
    auto drawMap = [&](Sprite &spr, int dx, int dy) {
        spr.setPosition(x + dx * rate, y + dy * rate);
        spr.setScale(rate, rate);
        motaSystem.window.draw(spr);
    };

    // 清屏
    if (clear_device) motaSystem.window.clear();

    // 状态栏
    if (visible) mapStatus();

    // 绘制地板
    Sprite floor(motaSystem.textureCache["system\\floor.png"]);
    drawMap(floor, 0, 0);

    // 记录怪物
    motaTemp.floorEnemies.clear();

    // 复制角色信息
    auto& act = actors[motaVariables.variables[0]];

    // 绘制事件
    for (const auto& ev : gmap.mapEvents) {
        if (!ev.exist) continue;
        if (ev.triggerCondition[0] == 1 && motaVariables.variables[ev.triggerCondition[1]] != ev.triggerCondition[2]) continue;
        auto [dx, dy] = make_pair(ev.x * 32, ev.y * 32);
        Sprite evspr(motaSystem.textureCache["character\\" + ev.file]);
        evspr.setTextureRect(IntRect(32 * ((ev.pos[0] + ev.move * motaSystem.gameTime / 4 ) % 4), 32 * ev.pos[1], 32, 32));
        drawMap(evspr, dx, dy);

        // 地图显示伤害
        auto namelist = split(ev.name, "<>");
        for (const auto& evname : namelist) {
            if (auto names = split(evname, "/"); rate == 1.f && act.item[3] > 0 && names[0] == "monster") {
                int eid = stoi(names[1]);
                if (!vectorFind(motaTemp.floorEnemies, motaData.enemies[eid])) {
                    motaTemp.floorEnemies.push_back(motaData.enemies[eid]);
                }
                auto en = motaData.enemies[eid];
                if (motaVariables.variables[4] % 3 >= 1) {
                    Color clr;
                    auto dmg = en.getDamage(&act, &motaData.elements);
                    if (dmg == -1 || dmg >= act.hp) clr = Color(169,169,169);
                    else if (dmg >= act.hp * 3 / 4) clr = Color::Red;
                    else if (dmg >= act.hp / 2) clr = Color::Yellow;
                    else if (dmg >= act.hp * 1 / 4) clr = Color::Green;
                    else clr = Color::White;
                    drawText(&motaSystem.window, &motaSystem.font, IntRect(MAPX + 32 * ev.x + 1, MAPY + 32 * ev.y + 22 + 1, 32, 10), dmg == -1 ? "DIE" : to_string(dmg), 2, 8L, true, Color::Black);
                    drawText(&motaSystem.window, &motaSystem.font, IntRect(MAPX + 32 * ev.x, MAPY + 32 * ev.y + 22, 32, 10), dmg == -1 ? "DIE" : to_string(dmg), 2, 8L, true, clr);
                }

                // 第二档显伤显示临界
                if (motaVariables.variables[4] % 3 == 2) {
                    int crs = en.getCrisis(&act);
                    string crisis = to_string(crs);
                    if (crs == -1) crisis = "？";
                    else if (crs == 0) crisis = "";
                    drawText(&motaSystem.window, &motaSystem.font, IntRect(MAPX + 32 * ev.x + 1, MAPY + 32 * ev.y + 10 + 1, 32, 10), crisis, 2, 8L, true, Color::Black);
                    drawText(&motaSystem.window, &motaSystem.font, IntRect(MAPX + 32 * ev.x, MAPY + 32 * ev.y + 10, 32, 10), crisis, 2, 8L, true);
                }
            }
        }
    }

    // 根据伤害排序
    if (rate == 1.f && act.item[3] > 0)
        sort(motaTemp.floorEnemies.begin(), motaTemp.floorEnemies.end(), [&](Enemy a, Enemy b){
            int admg = a.getDamage(&act, &motaData.elements), bdmg = b.getDamage(&act, &motaData.elements);
            if (admg == -1 && bdmg != -1) return false;
            if (admg != -1 && bdmg == -1) return true;
            if (admg == -1 && bdmg == -1) return a.name < b.name;
            return admg < bdmg;
        });

    // 显示角色行走图
    if (visible) {
        if (player.visible) {
            int dir[4][2] = {{0, 1}, {-1, 0}, {1, 0}, {0, -1}};
            auto [ax, ay] = make_pair(act.x * 32 - dir[player.direction][0] * 8 * player.movingCount, act.y * 32 - dir[player.direction][1] * 8 * player.movingCount);
            Sprite evspr(motaSystem.textureCache["character\\" + act.file]);
            evspr.setTextureRect(IntRect(32 * max(0, player.movingCount - 1), 32 * player.direction, 32, 32));
            drawMap(evspr, ax, ay);
        }
        if (player.movingCount > 0) --player.movingCount;

        // 显示动画
        queue <SpriteAnimation> temp;
        while (!animationQueue.empty()) {
            SpriteAnimation ani = animationQueue.front();
            animationQueue.pop();
            Sprite anispr(motaSystem.textureCache["animation\\" + ani.type.patterns[ani.currentFrame]]);
            anispr.setPosition(ani.actX, ani.actY);
            auto siz = motaSystem.textureCache["animation\\" + ani.type.patterns[ani.currentFrame]].getSize();
            anispr.setOrigin(siz.x / 2, siz.y / 2);
            motaSystem.window.draw(anispr);
            if (ani.currentFrame == ani.type.SETime) playSE(ani.type.SEFile, motaSystem.SEVolume);
            ++ani.currentFrame;
            if (ani.currentFrame < ani.type.patterns.size()) temp.push(ani);
        }
        if (!temp.empty()) swap(animationQueue, temp);
    }
}

void ScreenData::waitCount(int times) {
    for (int i = 0; i < times; ++i) {
        Event event;
        while (motaSystem.window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                motaSystem.window.close();
                exit(0);
            }
        }

        // 刷新画面
        if (onMap) showMap(screenData.visualMap, MAPX, MAPY);
        motaGraphics.update(!onMap);
    }
}

void ScreenData::addAnimation(int id, int x, int y) {
    animationQueue.push({x, y, 0, motaData.animations[id]});
}

void ScreenData::addEVAnimation(int id, int x, int y) {
    animationQueue.push({MAPX + x * 32 + 16, MAPY + y * 32 + 16, 0, motaData.animations[id]});
}

void ScreenData::loadData(int fileid) {
    // 存档文件名
    string filename = format("save\\save_{}.json", fileid);
    // 读取文件
    ifstream infile(filename);
    // 创建json对象
    nlohmann::json json;
    // 读入数据
    infile >> json;
    // 读入角色数据
    actors = json["actors"];
    // 读入变量数据
    motaVariables = json["variables"];
    infile.close();
}

void ScreenData::saveData(int fileid) {
    // 存档文件名
    string filename = format("save\\save_{}.json", fileid);
    // 创建json对象
    nlohmann::json json;
    // 写入角色数据
    json["actors"] = actors;
    // 写入变量数据
    json["variables"] = motaVariables;
    // 存档成文件
    ofstream outfile(filename);
    outfile << json.dump(4);
    outfile.close();
}

void ScreenData::doOrder(const vector<string>& lists, Object* obj) {
    for (const auto& od : lists) {
        Interpreter(od).execute(obj);

        // 执行完事件才到下一步
        do {
            screenData.waitCount(1);
            motaSystem.scene->update();
            if (motaTemp.gameOver || motaTemp.ending) return; // 中断函数
        } while (!motaTemp.nextMove);

        // 重置nextMove
        motaTemp.nextMove = false;
    }
}

void ScreenData::transition1(int time) {
    if (transition.opacity == 0) return;
    int once = ceil(255.f / time);
    for (int i = 0; i < time; ++i) {
        transition.opacity = max(0, transition.opacity - once);
        waitCount(1);
    }
}

void ScreenData::transition2(int time) {
    if (transition.opacity == 255) return;
    int once = ceil(255.f / time);
    for (int i = 0; i < time; ++i) {
        transition.opacity = min(255, transition.opacity + once);
        waitCount(1);
    }
    waitCount(5);
}

void ScreenData::dispose() {
    motaGraphics.eraseImage(&transition);
}
