#include <Game/GameData.h>

#include <utility>

GameVariables motaVariables;
GameTemp motaTemp;
GameData motaData;
ScreenData screenData;

GameEvent::GameEvent() {
    // 初始化事件
    ID = 0;
    name = "";
    file = "";
    fill(pos, pos + 2, 0);
    x = 0;
    y = 0;
    fill(triggerCondition, triggerCondition + 3, 0);
    move = false;
    through = false;
    exist = true;
    toDispose = false;
}

GameEvent::GameEvent(string name, int x, int y) : GameEvent() {
    // 初始化带坐标的事件
    this->name = std::move(name);
    this->x = x;
    this->y = y;
}

bool GameEvent::operator == (const GameEvent& ev) const {
    // 重载等于号
    return (x == ev.x && y == ev.y && exist);
}

void GameEvent::order(bool tempEV) {
    // 不存在的事件就不执行了
    if (!exist) return;
    // 临时事件将不对functionEVID赋值
    if (!tempEV) motaTemp.functionEventID = ID;
    // 未达到条件不可执行的
    if (triggerCondition[0] != 0 && motaVariables.variables[triggerCondition[1]] != triggerCondition[2]) return;
    auto namelist = split(name, "<>");
    for (const auto& evname : namelist) {
        // 将事件名按/号分割
        auto info = split(evname, "/");
        int kind, val;
        // 游戏结束awsl
        if (info[0] == "gg") {
            motaTemp.messageInfo.emplace_back(-1, "勇士", "怎能…\n倒在这里……");
            motaTemp.gameOver = true;
            motaSystem.scene->update();
            continue;
        }
        // 战斗
        if (info[0] == "monster") {
            motaTemp.battleEnemyID = stoi(motaVariables.initDialogue(info[1]));
            motaSystem.scene->update();
            continue;
        }
        // 宝石血瓶
        if (info[0] == "bonus") {
            if (!tempEV) playSE(motaSystem.getSE);
            tie(kind, val) = make_pair(stoi(motaVariables.initDialogue(info[1])), stoi(motaVariables.initDialogue(info[2])));
            if (kind == 0) screenData.actors[motaVariables.variables[0]].hp += val;
            if (kind == 1) screenData.actors[motaVariables.variables[0]].atk += val;
            if (kind == 2) screenData.actors[motaVariables.variables[0]].def += val;
            if (kind == 3) screenData.actors[motaVariables.variables[0]].mdef += val;
            if (kind == 4) screenData.actors[motaVariables.variables[0]].exp += val;
            if (kind == 5) screenData.actors[motaVariables.variables[0]].gold += val;
            if (kind == 6) {
                screenData.actors[motaVariables.variables[0]].level += val;
                screenData.actors[motaVariables.variables[0]].hp += val * 400;
                screenData.actors[motaVariables.variables[0]].atk += val * 5;
                screenData.actors[motaVariables.variables[0]].def += val * 5;
            }
            toDispose = true;
            motaSystem.scene->update();
            continue;
        }
        // 开门
        if (info[0] == "door") {
            kind = stoi(motaVariables.initDialogue(info[1]));
            if (kind == 0) {
                playSE(motaSystem.gateSE);
                openDoor();
                toDispose = true;
            }
            if (kind == 1 && screenData.actors[motaVariables.variables[0]].item[0] > 0) {
                --screenData.actors[motaVariables.variables[0]].item[0];
                playSE(motaSystem.gateSE);
                openDoor();
                toDispose = true;
            }
            if (kind == 2 && screenData.actors[motaVariables.variables[0]].item[1] > 0) {
                --screenData.actors[motaVariables.variables[0]].item[1];
                playSE(motaSystem.gateSE);
                openDoor();
                toDispose = true;
            }
            if (kind == 3 && screenData.actors[motaVariables.variables[0]].item[2] > 0) {
                --screenData.actors[motaVariables.variables[0]].item[2];
                playSE(motaSystem.gateSE);
                openDoor();
                toDispose = true;
            }
            motaSystem.scene->update();
            continue;
        }
        // 物品
        if (info[0] == "item") {
            if (!tempEV) playSE(motaSystem.getSE);
            tie(kind, val) = make_pair(stoi(motaVariables.initDialogue(info[1])), stoi(motaVariables.initDialogue(info[2])));
            if (!motaVariables.itemRecord[kind] || val != 1) {
                motaVariables.itemRecord[kind] = true;
                motaTemp.messageInfo.emplace_back(-4, to_string(kind), "");
            }
            screenData.actors[motaVariables.variables[0]].item[kind] += val;
            toDispose = true;
            motaSystem.scene->update();
            continue;
        }
        // 商店
        if (info[0] == "shop") {
            motaTemp.shopType = stoi(info[1]);
            motaTemp.addPower = split(info[2], "&");
            if (info.size() >= 4) motaTemp.initPrice = split(info[3], "&");
            else motaTemp.initPrice = { "0" };
            if (info.size() >= 5) motaTemp.rise = info[4];
            else motaTemp.rise = "0";
            motaSystem.scene->update();
            continue;
        }
        // npc
        if (info[0] == "npc") {
            kind = stoi(info[1]);
            for (auto [messID, messName, messContent] : motaData.npc[kind].npcInfo)
                motaTemp.messageInfo.emplace_back(messID, messName, insertNewLines(motaVariables.initDialogue(messContent), 42));
            if (!motaData.npc[kind].transName.empty()) {
                motaTemp.transEventName = motaData.npc[kind].transName;
                motaTemp.directlyFunction = motaData.npc[kind].directlyFunction;
            }
            toDispose = motaData.npc[kind].fade;
            motaSystem.scene->update();
            continue;
        }
        // 路障熔岩
        if (info[0] == "lava") {
            val = stoi(motaVariables.initDialogue(info[1]));
            screenData.actors[motaVariables.variables[0]].hp = max(0, screenData.actors[motaVariables.variables[0]].hp - val);
            if (screenData.actors[motaVariables.variables[0]].hp == 0) GameEvent("gg").order(true);
            motaSystem.scene->update();
            continue;
        }
        // 上楼
        if (info[0] == "up") {
            playSE(motaSystem.stairSE);
            GameImage stairImg("system\\mting.png");
            tie (stairImg.x, stairImg.y, stairImg.opacity, stairImg.z) = make_tuple(MAPX, MAPY, 0, 2);
            for (int i = 0; i < 15; ++i) {
                stairImg.opacity += 17;
                screenData.showMap(screenData.visualMap, MAPX, MAPY);
                motaGraphics.update(false);
            }
            string temp = format("{}:{}", motaVariables.variables[1], motaVariables.variables[2] + 1);
            if (int tgtmpid = motaData.searchMap(temp); tgtmpid != -1) {
                ++motaVariables.variables[2];
                screenData.actors[motaVariables.variables[0]].mapID = tgtmpid;
                if (info.size() == 3) { // 当传送有目标xy时
                    tie(kind, val) = make_pair(stoi(info[1]), stoi(info[2]));
                    screenData.actors[motaVariables.variables[0]].x = kind;
                    screenData.actors[motaVariables.variables[0]].y = val;
                }
                screenData.player.direction = 0;
                if (!vectorFind(motaVariables.floorRecord[motaVariables.variables[1]], motaVariables.variables[2])) {
                    motaVariables.floorRecord[motaVariables.variables[1]].push_back(motaVariables.variables[2]);
                    sort(motaVariables.floorRecord[motaVariables.variables[1]].begin(), motaVariables.floorRecord[motaVariables.variables[1]].end());
                }
                screenData.loadMap(tgtmpid, &screenData.visualMap);
            }
            else
                print("there's no such a map!");
            for (int i = 0; i < 15; ++i) {
                stairImg.opacity -= 17;
                screenData.showMap(screenData.visualMap, MAPX, MAPY);
                motaGraphics.update(false);
            }
            stairImg.dispose();
            motaSystem.scene->update();
            continue;
        }
        // 下楼
        if (info[0] == "down") {
            playSE(motaSystem.stairSE);
            GameImage stairImg("system\\mting.png");
            tie (stairImg.x, stairImg.y, stairImg.opacity, stairImg.z) = make_tuple(MAPX, MAPY, 0, 2);
            for (int i = 0; i < 15; ++i) {
                stairImg.opacity += 17;
                screenData.showMap(screenData.visualMap, MAPX, MAPY);
                motaGraphics.update(false);
            }
            string temp = format("{}:{}", motaVariables.variables[1], motaVariables.variables[2] - 1);
            if (int tgtmpid = motaData.searchMap(temp); tgtmpid != -1) {
                --motaVariables.variables[2];
                screenData.actors[motaVariables.variables[0]].mapID = tgtmpid;
                if (info.size() == 3) { // 当传送有目标xy时
                    tie(kind, val) = make_pair(stoi(info[1]), stoi(info[2]));
                    screenData.actors[motaVariables.variables[0]].x = kind;
                    screenData.actors[motaVariables.variables[0]].y = val;
                }
                screenData.player.direction = 0;
                if (!vectorFind(motaVariables.floorRecord[motaVariables.variables[1]], motaVariables.variables[2])) {
                    motaVariables.floorRecord[motaVariables.variables[1]].push_back(motaVariables.variables[2]);
                    sort(motaVariables.floorRecord[motaVariables.variables[1]].begin(), motaVariables.floorRecord[motaVariables.variables[1]].end());
                }
                screenData.loadMap(tgtmpid, &screenData.visualMap);
            }
            else
                print("there's no such a map!");
            for (int i = 0; i < 15; ++i) {
                stairImg.opacity -= 17;
                screenData.showMap(screenData.visualMap, MAPX, MAPY);
                motaGraphics.update(false);
            }
            stairImg.dispose();
            motaSystem.scene->update();
            continue;
        }
        // 传送
        if (info[0] == "move") {
            int mpid = stoi(info[1]), mx = stoi(info[2]), my = stoi(info[3]);
            playSE(motaSystem.stairSE);
            GameImage stairImg("system\\mting.png");
            tie (stairImg.x, stairImg.y, stairImg.opacity, stairImg.z) = make_tuple(MAPX, MAPY, 0, 2);
            for (int i = 0; i < 15; ++i) {
                stairImg.opacity += 17;
                screenData.showMap(screenData.visualMap, MAPX, MAPY);
                motaGraphics.update(false);
            }
            screenData.actors[motaVariables.variables[0]].mapID = mpid;
            screenData.actors[motaVariables.variables[0]].x = mx;
            screenData.actors[motaVariables.variables[0]].y = my;
            screenData.player.direction = 0;
            screenData.loadMap(mpid, &screenData.visualMap);
            for (int i = 0; i < 15; ++i) {
                stairImg.opacity -= 17;
                screenData.showMap(screenData.visualMap, MAPX, MAPY);
                motaGraphics.update(false);
            }
            stairImg.dispose();
            motaSystem.scene->update();
            continue;
        }
        // 修改变量
        if (info[0] == "var") {
            tie(kind, val) = make_pair(stoi(info[1]), stoi(info[2]));
            motaVariables.variables[kind] += val;
            motaSystem.scene->update();
            continue;
        }
        // 消除事件
        if (info[0] == "erase") {
            screenData.visualMap.mapEvents[motaTemp.functionEventID].toDispose = true;
            motaSystem.scene->update();
            continue;
        }
        // 返回标题画面
        if (info[0] == "back") {
            motaTemp.gameOver = true;
            motaSystem.scene->update();
            continue;
        }
        // 结局
        if (info[0] == "ending") {
            motaTemp.gameOver = true;
            motaTemp.ending = true;
            motaSystem.scene->update();
            continue;
        }

        // 下面是条件等对话相关的
        auto condName = split(motaVariables.initDialogue(evname), "~");
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
            motaSystem.scene->update();
            continue;
        }
        // 转换事件名
        if (condName[0] == "switch") {
            motaTemp.transEventName = condName[1];
            motaSystem.scene->update();
            continue;
        }
    }
}

void GameEvent::endEvent() {
    // exist设为false，且记录该事件
    exist = false;
    motaVariables.eventRecord[screenData.visualMap.mapID].insert(ID);
}

void GameEvent::openDoor() {
    for (int i = 0; i < 4; ++i) {
        ++pos[1];
        screenData.showMap(screenData.visualMap, MAPX, MAPY);
        motaGraphics.update(false);
    }
}

bool GameMap::operator==(const GameMap &mp) const {
    // 重载等于号，地图名相同即可
    return mapName == mp.mapName;
}

bool GameMap::haveAnEvent(int x, int y) {
    return ranges::any_of(mapEvents, [&](auto ev){
        return (ev.x == x && ev.y == y);
    });
}

int GameMap::checkEvent(int x, int y) {
    auto ev = ranges::find_if(mapEvents, [&](auto ev) {
        return ev.x == x && ev.y ==y;
    });
    return ev == mapEvents.end() ? -1 : ev->ID;
}

GameEvent* GameMap::EcheckEvent(int x, int y) {
    for (auto& ev : mapEvents)
        if (ev.x == x && ev.y == y)
            return &ev;
    return new GameEvent;
}

bool GameMap::passible(int x, int y) {
    // 地图外的坐标不可通行
    if (x > 10 || x < 0 || y > 10 || y < 0) return false;
    // 有不可穿透的事件时也不可通行
    if (haveAnEvent(x, y)) {
        if (auto ev = EcheckEvent(x, y); ev->exist && !ev->through) {
            // 如果还没出现就可以通行
            if (ev->triggerCondition[0] == 1 && motaVariables.variables[ev->triggerCondition[1]] != ev->triggerCondition[2]) return true;
            return false;
        }
    }
    return true;
}

vector <GameEvent*> GameMap::getLineEvents(pair<int, int> a, pair<int, int> b) {
    if (a.first != b.first && a.second != b.second) return {};
    vector <GameEvent*> result;
    if (a.first == b.first) {
        for (int i = min(a.second, b.second); i <= max(a.second, b.second); ++i) {
            if (haveAnEvent(a.first, i))
                result.push_back(EcheckEvent(a.first, i));
        }
    }
    else {
        for (int i = min(a.first, b.first); i <= max (a.first, b.first); ++i) {
            if (haveAnEvent(i, a.second))
                result.push_back(EcheckEvent(i, a.second));
        }
    }
    return result;
}

int GameActors::getAtk() const {
    // 加以衰弱影响
    return max(0, atk - motaVariables.variables[6]);
}

int GameActors::getDef() const {
    // 加以衰弱影响
    return max(0, def - motaVariables.variables[6]);
}

void GamePlayer::update() {
    // 行走中快捷键不生效
    if (movingCount > 0) return;
    // 根据四方向判定
    int dir = motaKeyBoard.dir4();
    direction = dir >= 0 ? dir : direction;
    if (dir == -1) return;
    // 根据方向判定行走
    int dirchg[4][2] = {{0, 1}, {-1, 0}, {1, 0}, {0, -1}};
    if (screenData.visualMap.passible(screenData.actors[motaVariables.variables[0]].x + dirchg[dir][0], screenData.actors[motaVariables.variables[0]].y + dirchg[dir][1])) {
        ++step;
        movingCount = movingCount < 2 ? 2 : 4;
        screenData.actors[motaVariables.variables[0]].x += dirchg[dir][0];
        screenData.actors[motaVariables.variables[0]].y += dirchg[dir][1];
        changeSteps();
        if (screenData.visualMap.haveAnEvent(screenData.actors[motaVariables.variables[0]].x, screenData.actors[motaVariables.variables[0]].y)) {
            while (movingCount > 0) {
                screenData.showMap(screenData.visualMap, MAPX, MAPY);
                motaGraphics.update(false);
            }
            screenData.visualMap.EcheckEvent(screenData.actors[motaVariables.variables[0]].x, screenData.actors[motaVariables.variables[0]].y)->order();
        }
    }
    else {
        if (screenData.visualMap.haveAnEvent(screenData.actors[motaVariables.variables[0]].x + dirchg[dir][0], screenData.actors[motaVariables.variables[0]].y + dirchg[dir][1]))
            screenData.visualMap.EcheckEvent(screenData.actors[motaVariables.variables[0]].x + dirchg[dir][0], screenData.actors[motaVariables.variables[0]].y + dirchg[dir][1])->order();
    }
}

void GamePlayer::changeSteps() {
    // 设置方位
    auto [ax, ay] = make_pair(screenData.actors[motaVariables.variables[0]].x, screenData.actors[motaVariables.variables[0]].y);
    int dir[4][2] = {{0, 1}, {0, -1}, {-1, 0}, {1, 0}};
    // 领域怪
    for (auto & i : dir) {
        if (auto [nx, ny] = make_pair(ax + i[0], ay + i[1]);screenData.visualMap.haveAnEvent(nx, ny)) {
            auto ev = screenData.visualMap.EcheckEvent(nx, ny);
            auto namelist = split(ev->name, "<>");
            for (auto evname : namelist) {
                if (auto names = split(ev->name, "/"); ev->exist && names[0] == "monster") {
                    if (auto en = motaData.enemies[stoi(names[1])]; en.getP(9)) {
                        auto dmg = max(0, en.atk - (en.getP(6) ? 0 : screenData.actors[motaVariables.variables[0]].getDef()));
                        if (dmg > 0) {
                            screenData.addEVAnimation(en.animationID, ax, ay);
                            screenData.actors[motaVariables.variables[0]].hp = max(0, screenData.actors[motaVariables.variables[0]].hp - dmg);
                            if (screenData.actors[motaVariables.variables[0]].hp == 0) GameEvent("gg").order(true);
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
                            auto dmg = max(0, en.atk - (en.getP(6) ? 0 : screenData.actors[motaVariables.variables[0]].getDef())) * 2;
                            if (dmg > 0) {
                                screenData.addEVAnimation(en.animationID, ax, ay);
                                screenData.actors[motaVariables.variables[0]].hp = max(0, screenData.actors[motaVariables.variables[0]].hp - dmg);
                                if (screenData.actors[motaVariables.variables[0]].hp == 0) GameEvent("gg").order(true);
                            }
                        }
                    }
                }
            }
        }
    }
}

bool GameEnemy::operator == (const GameEnemy& en) const {
    // 重载等于号，名字和文件名相同即可
    return (name == en.name && file == en.file);
}

int GameEnemy::getDamage(int atk, int def, int mdef) {
    // 读入怪物数据
    int ehp = this->hp, ed = this->def;
    // 读入主角数据
    if (atk == 0) atk = screenData.actors[motaVariables.variables[0]].getAtk();
    if (def == 0) def = screenData.actors[motaVariables.variables[0]].getDef();
    if (mdef == 0) mdef = screenData.actors[motaVariables.variables[0]].mdef;
    // 中毒的效果
    int poison = 100 + motaVariables.variables[5];
    // 吸血的效果
    float vampire = 0;
    // 坚固的效果
    if (getP(4)) ed = max(ed, atk - 1);
    // 先攻的效果
    bool first = getP(5) * max(conatk, 1);
    // 魔攻的效果
    if (getP(6)) def = 0;
    // 有[v]需要在战斗中使用的话，写在这里
    for (int i = 0, cnt = 1; i < element.size(); ++i) {
        if (motaData.elements[element[i]].haveBuff) {
            float buff = stof(split(name, "/")[cnt]);
            // 吸血的效果
            if (element[i] == 3)
                vampire = floor(poison / 100.f * max(this->atk - def, 0) * buff);

            cnt++;
        }
    }
    // 无法破防则返回-1
    if (atk <= ed) return -1;
    // 伤害量够不上吸血量则返回-1
    if (atk - ed <= vampire * max(1, conatk)) return -1;
    // 对方无法破防则返回0
    if (def >= this->atk) return 0;
    // 直接秒杀，仅留下先攻伤害
    if (ehp + vampire * first <= atk - ed) return max(0, (this->atk - def) * first);
    // 计算单次伤害，以及回合数
    int edam = atk - ed;
    int eatimes = max(0.f, ceil((ehp + vampire * first - edam) * 1.f / (edam - vampire * max(1, conatk))));
    int damage = poison / 100.f * max(this->atk - def, 0) * max(conatk, 1) * eatimes;
    return max(damage - mdef, 0);
}

int GameEnemy::getDef() {
    if (getP(4)) return max(def, screenData.actors[motaVariables.variables[0]].getAtk() - 1);
    return def;
}

bool GameEnemy::getP(int p) {
    return vectorFind(element, p);
}

int GameEnemy::getCrisis() {
    if (screenData.actors[motaVariables.variables[0]].getAtk() <= getDef()) return getDef() - screenData.actors[motaVariables.variables[0]].getAtk() + 1;
    if (screenData.actors[motaVariables.variables[0]].getAtk() - getDef() >= hp) return 0;
    if (getP(4)) return -1;
    return ceil(hp * 1.f / max(0.f, ceil(hp * 1.f / (screenData.actors[motaVariables.variables[0]].getAtk() - getDef())) - 1)) + getDef() - screenData.actors[motaVariables.variables[0]].getDef();
}

pair <string, string> GameEnemy::getElement(int pid, string buff) {
    // 替换buff
    string elename = motaData.elements[pid].name;
    if (getP(7)) // 连击
        elename = format("{}{}", conatk,elename);
    string desc = motaData.elements[pid].description;
    if (!buff.empty()) {
        if (getP(8)) // 重生，替换为变身的怪物名
            buff = split(motaData.enemies[stoi(buff)].name, "/")[0];
        replaceAll(desc, "[v]", buff);
    }
    return {elename, desc};
}

void GameData::init() {
    // 描绘进行中的画面
    auto processing = [](const wstring& content) {
        Text temptxt(content, motaSystem.font, 28);
        temptxt.setPosition(160 * motaSystem.resolutionRatio, 400 * motaSystem.resolutionRatio);
        motaSystem.window.clear();
        motaSystem.window.draw(temptxt);
        motaSystem.window.display();
    };
    // 加载数据
    auto processingData = [&](string subroot, const string& fext) {
        auto eachData = [&](string subroot, string filename) {
            map <string, string> info;
            if (subroot != "map") info = readData(format("data\\{}\\{}", subroot, filename));
            if (filename == "blankmap.dat") return;
            int fileid = stoi(split(split(filename, ".")[0], "_")[1]);
            // 主角数据
            if (subroot == "actor") {
                actors[fileid].ID = fileid;
                actors[fileid].name = info["name"];
                actors[fileid].file = info["file"];
                actors[fileid].level = stoi(info["level"]);
                actors[fileid].hp = stoi(info["hp"]);
                actors[fileid].atk = stoi(info["atk"]);
                actors[fileid].def = stoi(info["def"]);
                actors[fileid].mdef = stoi(info["mdef"]);
                actors[fileid].exp = 0;
                actors[fileid].gold = 0;
                actors[fileid].mapID = stoi(info["mapID"]);
                actors[fileid].x = stoi(info["x"]);
                actors[fileid].y = stoi(info["y"]);
                actors[fileid].animationID = stoi(info["animationID"]);
                memset(screenData.actors[fileid].item, 0, sizeof(screenData.actors[fileid].item));
            }
            // 动画数据
            if (subroot == "animation") {
                animations[fileid].pattern = split(info["pattern"], ",");
                animations[fileid].SEFile = info["SEFile"];
                animations[fileid].SETime = stoi(info["SETime"]);
            }
            // 属性数据
            if (subroot == "element") {
                elements[fileid].name = info["name"];
                elements[fileid].description = info["description"];
                elements[fileid].haveBuff = false;
                if (strInclude(elements[fileid].description, "[v]")) elements[fileid].haveBuff = true;
            }
            // 敌人数据
            if (subroot == "enemy") {
                enemies[fileid].ID = fileid;
                enemies[fileid].name = info["name"];
                enemies[fileid].file = info["file"];
                vector <int> tempele;
                auto elelist = split(info["element"], ",");
                for (const auto& ele : elelist)
                    tempele.push_back(stoi(ele));
                enemies[fileid].element = tempele;
                enemies[fileid].pos = stoi(info["pos"]);
                enemies[fileid].hp = stoi(info["hp"]);
                enemies[fileid].atk = stoi(info["atk"]);
                enemies[fileid].def = stoi(info["def"]);
                enemies[fileid].conatk = stoi(info["conatk"]);
                enemies[fileid].exp = stoi(info["exp"]);
                enemies[fileid].gold = stoi(info["gold"]);
                enemies[fileid].animationID = stoi(info["animationID"]);
            }
            // 物品数据
            if (subroot == "item") {
                items[fileid].ID = fileid;
                items[fileid].name = info["name"];
                items[fileid].description = info["description"];
                items[fileid].file = info["file"];
                items[fileid].pos[0] = stoi(split(info["pos"], ",")[0]);
                items[fileid].pos[1] = stoi(split(info["pos"], ",")[1]);
                items[fileid].price = stoi(info["price"]);
                items[fileid].usable = stoi(info["usable"]);
                items[fileid].cost = stoi(info["cost"]);
            }
            // 地图数据
            if (subroot == "map") {
                maps[fileid].mapID = fileid;
                auto data = split(readFile(format("data\\{}\\{}", subroot, filename))[0], ",");
                maps[fileid].mapName = data[0];
                maps[fileid].bgmFile = data[1];
                int evnumber = stoi(data[2]);
                for (int i = 0; i < evnumber; ++i) {
                    GameEvent ev;
                    ev.ID = i;
                    ev.name = data[3 + 11 * i];
                    ev.file = data[4 + 11 * i];
                    ev.pos[0] = stoi(data[5 + 11 * i]);
                    ev.pos[1] = stoi(data[6 + 11 * i]);
                    ev.x = stoi(data[7 + 11 * i]);
                    ev.y = stoi(data[8 + 11 * i]);
                    ev.triggerCondition[0] = stoi(data[9 + 11 * i]);
                    ev.triggerCondition[1] = stoi(data[10 + 11 * i]);
                    ev.triggerCondition[2] = stoi(data[11 + 11 * i]);
                    ev.move = stoi(data[12 + 11 * i]);
                    ev.through = stoi(data[13 + 11 * i]);
                    maps[fileid].mapEvents.push_back(ev);
                }
            }
            // npc数据
            if (subroot == "npc") {
                auto dialogueid = split(info["ID"], ",");
                auto namedata = info["name"];
                replaceAll(namedata, "none", "");
                auto npcname = split(namedata, ",");
                auto messagedata = info["message"];
                replaceAll(messagedata, "none", "");
                replaceAll(messagedata, "\\n", "\n");
                auto npcmessage = split(messagedata, ",");
                for (size_t i = 0; i < dialogueid.size(); ++i)
                    npc[fileid].npcInfo.emplace_back(stoi(dialogueid[i]), npcname[i], npcmessage[i]);
                npc[fileid].transName = info["transName"];
                replaceAll(npc[fileid].transName, "none", "");
                npc[fileid].fade = stoi(info["fade"]);
                npc[fileid].directlyFunction = stoi(info["directFunc"]);
            }

        };
        // 遍历文件夹
        string inPath = format("data\\{}", subroot);
        for (const auto& entry : filesystem::recursive_directory_iterator(inPath))
            if (entry.is_regular_file() && entry.path().extension() == fext)
                eachData(subroot, entry.path().filename().string());
    };
    // 初始化数据
    actors.clear();
    animations.clear();
    elements.clear();
    enemies.clear();
    items.clear();
    maps.clear();
    npc.clear();
    // 加载数据
    processing(L"正在加载角色数据...");
    processingData("actor", ".dat");
    processing(L"正在加载动画数据...");
    processingData("animation", ".dat");
    processing(L"正在加载属性数据...");
    processingData("element", ".dat");
    processing(L"正在加载敌人数据...");
    processingData("enemy", ".dat");
    processing(L"正在加载物品数据...");
    processingData("item", ".dat");
    processing(L"正在加载地图数据...");
    processingData("map", ".dat");
    processing(L"正在加载npc数据...");
    processingData("npc", ".dat");
    auto data = readData("ref\\motaName.ini");
    for (auto [key, value] : data)
        motaName[stoi(key)] = value;
}

int GameData::searchMap(const string& mapname) {
    // 搜寻同名地图
    auto result = ranges::find_if(maps, [&](auto mp) {
       return mp.second.mapName == mapname;
    });
    return result == maps.end() ? -1 : result->first;
}

void GameTemp::init() {
    // 初始化数据
    battleEnemyID = -1;
    shopType = -1;
    functionEventID = 0;
    closeMS = false;
    directlyFunction = false;
    gameOver = false;
    ending = false;
    initPrice.clear();
    rise = "";
    transEventName = "";
    messageInfo.clear();
    floorEnemies.clear();
}

void GameVariables::init() {
    // 初始化数据
    memset(variables, 0, sizeof(variables));
    if (filesystem::exists("ref\\Variables.txt")) {
        auto data = readFile("ref\\Variables.txt");
        for (auto var : data) {
            auto info = split(var, ":");
            if (info.size() == 3)
                variables[stoi(info[0])] = stoi(info[2]);
        }
    }
    floorRecord.clear();
    eventRecord.clear();
    transRecord.clear();
    itemRecord.clear();
}

string GameVariables::replaceToVar(const string& source) {
    string result = source;
    regex pattern("\\[([0-9]+)\\]"); // 匹配形如"[x]"的字符串
    smatch match;
    while (std::regex_search(result, match, pattern)) {
        if (match.size() == 2) {
            int index = std::stoi(match[1]);
            if (index >= 0 && index < sizeof(variables)) {
                string change = to_string(variables[index]);
                if (change.length() % 3 != 0)
                    for (int j = 0, len = change.length() % 3; j < 3 - len; ++j)
                        change += " ";
                result = match.prefix().str() + change + match.suffix().str();
            }
        }
    }
    return result;
}

string GameVariables::initDialogue(const string& source) {
    string result = replaceToVar(source);
    replaceAll(result, "[level]", to_string(screenData.actors[motaVariables.variables[0]].level));
    replaceAll(result, "[hp]", to_string(screenData.actors[motaVariables.variables[0]].hp));
    replaceAll(result, "[atk]", to_string(screenData.actors[motaVariables.variables[0]].atk));
    replaceAll(result, "[def]", to_string(screenData.actors[motaVariables.variables[0]].def));
    replaceAll(result, "[mdef]", to_string(screenData.actors[motaVariables.variables[0]].mdef));
    replaceAll(result, "[exp]", to_string(screenData.actors[motaVariables.variables[0]].exp));
    replaceAll(result, "[gold]", to_string(screenData.actors[motaVariables.variables[0]].gold));
    return result;
}

ScreenData::ScreenData() {
    transition.setSprite("system\\mting-trans.png");
    transition.opacity = 0;
    transition.z = 9;
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

void ScreenData::loadMap(int mapID, GameMap* gmap) {
    // 将当前地图加载进gmap
    *gmap = motaData.maps[mapID];
    // 根据记录将部分事件清除
    if (motaVariables.eventRecord.count(mapID) > 0 && !motaVariables.eventRecord[mapID].empty())
        for (auto it : motaVariables.eventRecord[mapID])
            gmap->mapEvents[it].exist = false;
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
            drawText(IntRect(16, y, 160, 24), txt);
            drawText(IntRect(16, y, 160, 24), to_string(val), 2);
            y += 24;
        }
    };
    // 绘制背景
    drawImage("system\\mapstatus.png", 0, 0);
    // 关闭状态栏时返回
    if (motaTemp.closeMS || motaVariables.variables[7]) return;
    // 绘制分割线条
    drawImage("system\\mapstatus_line.png", 0, 0);
    // 根据魔塔编号设置名字
    drawText(IntRect(224, 0, 352, 64), strInclude(visualMap.mapName, ":") ? format("{} {} 层", motaData.motaName[motaVariables.variables[1]], motaVariables.variables[2]) : visualMap.mapName, 1, 28L, true);
    // 绘制主角icon
    drawImage("character\\" + actors[motaVariables.variables[0]].file, 16, 32, IntRect(0, 0, 32, 32));
    drawText(IntRect(64, 32, 96, 32), actors[motaVariables.variables[0]].name);
    // 状态显示
    string status[] = {"", "中毒", "衰弱"}, statustxt = "";
    for (auto s : actors[motaVariables.variables[0]].status)
        statustxt += "|" + status[s];
    if (!statustxt.empty()) statustxt.erase(statustxt.begin());
    drawText(IntRect(16, 72, 160, 24), "状态");
    drawText(IntRect(16, 72, 160, 24), format("[{}]", actors[motaVariables.variables[0]].status.empty() ? "正常" : statustxt), 2);
    // 绘制状态栏
    drawStatusText({make_pair("生命", actors[motaVariables.variables[0]].hp),
                    make_pair("攻击", actors[motaVariables.variables[0]].atk),
                    make_pair("防御", actors[motaVariables.variables[0]].def),
                    make_pair("魔防", actors[motaVariables.variables[0]].mdef)}, 120);
    if (actors[motaVariables.variables[0]].status.contains(1))
        drawText(IntRect(64, 128, 96, 16), format("({})", motaVariables.variables[5]), 0, 12L, false, Color::Green);
    if (actors[motaVariables.variables[0]].status.contains(2)) {
        drawText(IntRect(64, 152, 96, 16), format("(-{})", motaVariables.variables[6]), 0, 12L, false, Color::Magenta);
        drawText(IntRect(64, 176, 96, 16), format("(-{})", motaVariables.variables[6]), 0, 12L, false, Color::Magenta);
    }
    drawStatusText({make_pair("经验", actors[motaVariables.variables[0]].exp),
                    make_pair("金币", actors[motaVariables.variables[0]].gold)}, 240);
    // 绘制钥匙
    drawImage("character\\" + motaData.items[0].file, 16, 312, IntRect(32 * motaData.items[0].pos[0], 32 * motaData.items[0].pos[1], 32, 32));
    drawImage("character\\" + motaData.items[1].file, 16, 336, IntRect(32 * motaData.items[1].pos[0], 32 * motaData.items[1].pos[1], 32, 32));
    drawImage("character\\" + motaData.items[2].file, 16, 360, IntRect(32 * motaData.items[2].pos[0], 32 * motaData.items[2].pos[1], 32, 32));
    drawText(IntRect(16, 312, 160, 24), to_string(actors[motaVariables.variables[0]].item[0]), 2);
    drawText(IntRect(16, 336, 160, 24), to_string(actors[motaVariables.variables[0]].item[1]), 2);
    drawText(IntRect(16, 360, 160, 24), to_string(actors[motaVariables.variables[0]].item[2]), 2);
    // 有ShortcutKey.txt则绘制Press L
    if (filesystem::exists("ref\\ShortcutKey.txt")) drawText(IntRect(0, 456, 624, 24), "～Press L～", 2);
}

void ScreenData::showMap(const GameMap& gmap, float x, float y, float rate, bool visible, bool clear_device) {
    // 绘制地图事件的函数
    auto drawMap = [&](Sprite &spr, int dx, int dy) {
        spr.setPosition((x + dx * rate) * motaSystem.resolutionRatio, (y + dy * rate) * motaSystem.resolutionRatio);
        spr.setScale(rate * motaSystem.resolutionRatio, rate * motaSystem.resolutionRatio);
        motaSystem.window.draw(spr);
    };
    // 清屏
    if (clear_device)
        motaSystem.window.clear();
    // 状态栏
    if (visible) mapStatus();
    // 绘制地板
    Sprite floor(motaSystem.textureCache["system\\floor.png"]);
    drawMap(floor, 0, 0);
    // 记录怪物
    motaTemp.floorEnemies.clear();
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
            if (auto names = split(evname, "/"); rate == 1.f && actors[motaVariables.variables[0]].item[3] > 0 && names[0] == "monster") {
                int eid = stoi(names[1]);
                if (!vectorFind(motaTemp.floorEnemies, motaData.enemies[eid]))
                    motaTemp.floorEnemies.push_back(motaData.enemies[eid]);
                auto en = motaData.enemies[eid];
                if (motaVariables.variables[4] % 3 >= 1) {
                    Color clr;
                    if (en.getDamage() == -1 || en.getDamage() >= actors[motaVariables.variables[0]].hp) clr = Color(169,169,169);
                    else if (en.getDamage() >= actors[motaVariables.variables[0]].hp * 3 / 4) clr = Color::Red;
                    else if (en.getDamage() >= actors[motaVariables.variables[0]].hp / 2) clr = Color::Yellow;
                    else if (en.getDamage() >= actors[motaVariables.variables[0]].hp * 1 / 4) clr = Color::Green;
                    else clr = Color::White;
                    drawText(IntRect(MAPX + 32 * ev.x + 1, MAPY + 32 * ev.y + 22 + 1, 32, 10), en.getDamage() == -1 ? "DIE" : to_string(en.getDamage()), 2, 8L, true, Color::Black);
                    drawText(IntRect(MAPX + 32 * ev.x, MAPY + 32 * ev.y + 22, 32, 10), en.getDamage() == -1 ? "DIE" : to_string(en.getDamage()), 2, 8L, true, clr);
                }
                // 第二档显伤显示临界
                if (motaVariables.variables[4] % 3 == 2) {
                    string crisis = to_string(en.getCrisis());
                    if (en.getCrisis() == -1) crisis = "？";
                    else if (en.getCrisis() == 0) crisis = "";
                    drawText(IntRect(MAPX + 32 * ev.x + 1, MAPY + 32 * ev.y + 10 + 1, 32, 10), crisis, 2, 8L, true, Color::Black);
                    drawText(IntRect(MAPX + 32 * ev.x, MAPY + 32 * ev.y + 10, 32, 10), crisis, 2, 8L, true);
                }
            }
        }
    }
    // 根据伤害排序
    if (rate == 1.f && actors[motaVariables.variables[0]].item[3] > 0)
        sort(motaTemp.floorEnemies.begin(), motaTemp.floorEnemies.end(), [](GameEnemy a, GameEnemy b){
            if (a.getDamage() == -1 && b.getDamage() != -1) return false;
            if (a.getDamage() != -1 && b.getDamage() == -1) return true;
            if (a.getDamage() == -1 && b.getDamage() == -1) return a.name < b.name;
            return a.getDamage() < b.getDamage();
        });
    // 显示角色行走图
    if (visible) {
        if (player.visible) {
            int dir[4][2] = {{0, 1}, {-1, 0}, {1, 0}, {0, -1}};
            auto [ax, ay] = make_pair(actors[motaVariables.variables[0]].x * 32 - dir[player.direction][0] * 8 * player.movingCount, actors[motaVariables.variables[0]].y * 32 - dir[player.direction][1] * 8 * player.movingCount);
            Sprite evspr(motaSystem.textureCache["character\\" + actors[motaVariables.variables[0]].file]);
            evspr.setTextureRect(IntRect(32 * max(0, player.movingCount - 1), 32 * player.direction, 32, 32));
            drawMap(evspr, ax, ay);
        }
        if (player.movingCount > 0) --player.movingCount;
        // 显示动画
        queue <SpriteAnimation> temp;
        while (!animationQueue.empty()) {
            SpriteAnimation ani = animationQueue.front();
            animationQueue.pop();
            Sprite anispr(motaSystem.textureCache["animation\\" + ani.type.pattern[ani.currentFrame]]);
            anispr.setPosition(ani.actX * motaSystem.resolutionRatio, ani.actY * motaSystem.resolutionRatio);
            anispr.setScale(motaSystem.resolutionRatio, motaSystem.resolutionRatio);
            auto siz = motaSystem.textureCache["animation\\" + ani.type.pattern[ani.currentFrame]].getSize();
            anispr.setOrigin(siz.x * motaSystem.resolutionRatio / 2, siz.y * motaSystem.resolutionRatio / 2);
            motaSystem.window.draw(anispr);
            if (ani.currentFrame == ani.type.SETime) playSE(ani.type.SEFile);
            ++ani.currentFrame;
            if (ani.currentFrame < ani.type.pattern.size()) temp.push(ani);
        }
        if (!temp.empty()) swap(animationQueue, temp);
    }
}

void ScreenData::waitCount(int times) {
    for (int i = 0; i < times; ++i) {
        if (onMap)
            showMap(screenData.visualMap, MAPX, MAPY);
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
    auto data = readFile(format("save\\save_{}.sav", fileid));
    // 读入角色数据
    auto actorData = split(data[1], ",");
    int actNumber = stoi(actorData[0]);
    for (int i = 0; i < actNumber; ++i) {
        GameActors act;
        act.ID = stoi(actorData[1 + 16 * i]);
        act.name = actorData[2 + 16 * i];
        act.file = actorData[3 + 16 * i];
        act.level = stoi(actorData[4 + 16 * i]);
        act.hp = stoi(actorData[5 + 16 * i]);
        act.atk = stoi(actorData[6 + 16 * i]);
        act.def = stoi(actorData[7 + 16 * i]);
        act.mdef = stoi(actorData[8 + 16 * i]);
        act.exp = stoi(actorData[9 + 16 * i]);
        act.gold = stoi(actorData[10 + 16 * i]);
        act.mapID = stoi(actorData[11 + 16 * i]);
        act.x = stoi(actorData[12 + 16 * i]);
        act.y = stoi(actorData[13 + 16 * i]);
        act.animationID = stoi(actorData[14 + 16 * i]);
        auto itemData = split(actorData[15 + 16 * i], "|");
        for (size_t j = 0; j < itemData.size(); ++j)
            act.item[j] = stoi(itemData[j]);
        auto statusData = split(actorData[16 + 16 * i], "|");
        auto statusNumber = stoi(statusData[0]);
        for (int j = 0; j < statusNumber; ++j)
            act.status.insert(stoi(statusData[1 + j]));
        actors[act.ID] = act;
    }
    // 读入变量数据
    auto varData = split(data[2], ",");
    for (size_t i = 0; i < varData.size(); ++i)
        motaVariables.variables[i] = stoi(varData[i]);
    // 读入楼层记录数据
    auto floorData = split(data[3], ",");
    int floorNumber = stoi(floorData[0]);
    for (int i = 0; i < floorNumber; ++i) {
        int mtid = stoi(floorData[1 + 3 * i]);
        int floorCount = stoi(floorData[2 + 3 * i]);
        auto floorList = split(floorData[3 + 3 * i], "|");
        for (int j = 0; j < floorCount; ++j)
            motaVariables.floorRecord[mtid].push_back(stoi(floorList[j]));
    }
    // 读入事件记录数据
    auto eventData = split(data[4], ",");
    int eventNumber = stoi(eventData[0]);
    for (int i = 0; i < eventNumber; ++i) {
        int mpid = stoi(eventData[1 + 3 * i]);
        int eventCount = stoi(eventData[2 + 3 * i]);
        auto eventList = split(eventData[3 + 3 * i], "|");
        for (int j = 0; j < eventCount; ++j)
            motaVariables.eventRecord[mpid].insert(stoi(eventList[j]));
    }
    // 读入事件名变换记录数据
    auto transData = split(data[5], ",");
    int transNumber = stoi(transData[0]);
    for (int i = 0; i < transNumber; ++i) {
        int mpid = stoi(transData[1 + 3 * i]);
        int transCount = stoi(transData[2 + 3 * i]);
        auto transList = split(transData[3 + 3 * i], "|");
        for (int j = 0; j < transCount; ++j){
            auto fromto = split(transList[j], "=>");
            motaVariables.transRecord[mpid][stoi(fromto[0])] = fromto[1];
        }
    }
    // 读入物品记录数据
    auto itemData = split(data[6], ",");
    int itemNumber = stoi(itemData[0]);
    for (int i = 0; i < itemNumber; ++i)
        motaVariables.itemRecord[stoi(itemData[1 + i])] = true;
}

void ScreenData::saveData(int fileid) {
    string data = "[savefile]\n";
    // 写入角色数据
    data += to_string(screenData.actors.size()) + ",";
    for (auto [actid, act] : screenData.actors) {
        data += to_string(act.ID) + "," + act.name + "," + act.file + "," + to_string(act.level) + "," + to_string(act.hp) + "," + to_string(act.atk) + "," + to_string(act.def) + "," + to_string(act.mdef) + "," + to_string(act.exp) + "," + to_string(act.gold) + "," + to_string(act.mapID) + "," + to_string(act.x) + "," + to_string(act.y) + "," + to_string(act.animationID) + "," + arrayToString(act.item, sizeof(act.item) / sizeof(int), "|") + ",";
        data += to_string(act.status.size()) + "|";
        for (auto s : act.status)
            data += to_string(s) + "/";
    }
    data.pop_back(); data += "\n";
    // 写入变量数据
    data += arrayToString(motaVariables.variables, sizeof(motaVariables.variables) / sizeof(int)) + "\n";
    // 写入楼层记录数据
    data += to_string(motaVariables.floorRecord.size()) + ",";
    for (auto [mtid, floors] : motaVariables.floorRecord)
        data += to_string(mtid) + "," + to_string(floors.size()) + "," + arrayToString(floors, "|") + ",";
    data.pop_back(); data += "\n";
    // 写入事件记录数据
    data += to_string(motaVariables.eventRecord.size()) + ",";
    for (auto [mpid, ev] : motaVariables.eventRecord)
        data += to_string(mpid) + "," + to_string(ev.size()) + "," + setToString(ev, "|") + ",";
    data.pop_back(); data += "\n";
    // 写入事件名变换记录数据
    data += to_string(motaVariables.transRecord.size()) + ",";
    for (auto [mpid, trans] : motaVariables.transRecord) {
        data += to_string(mpid) + "," + to_string(trans.size()) + ",";
        for (auto [evid, transname] : trans)
            data += to_string(evid) + "=>" + transname + "|";
    }
    data.pop_back(); data += "\n";
    // 写入物品记录数据
    data += to_string(motaVariables.itemRecord.size()) + ",";
    for (auto [itmid, _] : motaVariables.itemRecord)
        data += to_string(itmid) + ",";
    data.pop_back(); data += "\n";
    // 存档成文件
    saveFile(format("save\\save_{}.sav", fileid), data);
}

void ScreenData::doOrder(const vector<string>& lists) {
    for (const auto& od : lists)
        GameEvent(od).order(true);
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
    transition.dispose();
}
