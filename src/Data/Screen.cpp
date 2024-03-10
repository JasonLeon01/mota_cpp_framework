#include <Game/Data/Screen.hpp>

ScreenData screenData;

Interpreter::Interpreter(std::string order) {
    this->order = std::move(order);
}

void Interpreter::execute(Object* obj) {
    // 渐变动画
    auto move1 = [&](std::string picname, int picid = -1) {
        screenData.pictures[picid].setSprite("picture\\" + picname);
        screenData.pictures[picid].opacity = 0;
        if (picid >= 0) {
            auto siz = motaSystem.textureCache["picture\\" + picname].getSize();
            screenData.pictures[picid].origin_x = siz.x / 2;
            screenData.pictures[picid].origin_y = siz.y / 2;
            screenData.pictures[picid].x = 320;
            screenData.pictures[picid].y = 240;
        }
        else {
            screenData.pictures[picid].x = MAPX;
            screenData.pictures[picid].y = MAPY;
        }
        screenData.pictures[picid].z = picid < 0 ? 2 : picid;
        motaGraphics.addImage(&screenData.pictures[picid]);
        for (int i = 0, chg = 10 * (1 + motaSystem.frameRate); i < chg; ++i) {
            screenData.pictures[picid].opacity = std::min(screenData.pictures[picid].opacity + (int)ceil(255.0 / chg), 255);
            screenData.waitCount(1);
        }
    };
    auto move2 = [&](int picid = -1) {
        for (int i = 0, chg = 10 * (1 + motaSystem.frameRate); i < chg; ++i) {
            screenData.pictures[picid].opacity = std::max(screenData.pictures[picid].opacity - (int)ceil(255.0 / chg), 0);
            screenData.waitCount(1);
        }
        motaGraphics.eraseImage(&screenData.pictures[picid]);
    };

    // 在这里设置各种事件指令
    // 提前复制角色信息，简化代码
    auto& act = screenData.actors[motaVariables.variables[0]];

    // 将事件名按/号分割
    auto info = initOrder(order);
    int kind = 0, val = 0;

    // 游戏结束awsl
    if (info[0] == "gg") {
        motaTemp.messageInfo.emplace_back(-1, "勇士", "怎能…\n倒在这里……");
        motaTemp.gameOver = true;
    }

    // 战斗
    if (info[0] == "monster") {
        motaTemp.battleEnemyID = stoi(initDialogue(info[1]));
        motaTemp.pause = true;
    }

    // 宝石血瓶
    if (info[0] == "bonus") {
        if (motaTemp.functionEventID != -1) playSE(motaSystem.getSE, motaSystem.SEVolume);
        std::tie(kind, val) = std::make_pair(stoi(initDialogue(info[1])), stoi(initDialogue(info[2])));
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
    }

    // 开门
    if (info[0] == "door") {
        kind = stoi(initDialogue(info[1]));
        if (kind == 0) {
            playSE(motaSystem.gateSE, motaSystem.SEVolume);
            openDoor(obj);
            if (obj != nullptr) obj->toDispose = true;
        }
        if (kind == 1 && act.item[0] > 0) {
            --act.item[0];
            playSE(motaSystem.gateSE, motaSystem.SEVolume);
            openDoor(obj);
            if (obj != nullptr) obj->toDispose = true;
        }
        if (kind == 2 && act.item[1] > 0) {
            --act.item[1];
            playSE(motaSystem.gateSE, motaSystem.SEVolume);
            openDoor(obj);
            if (obj != nullptr) obj->toDispose = true;
        }
        if (kind == 3 && act.item[2] > 0) {
            --act.item[2];
            playSE(motaSystem.gateSE, motaSystem.SEVolume);
            openDoor(obj);
            if (obj != nullptr) obj->toDispose = true;
        }
    }

    // 物品
    if (info[0] == "item") {
        if (motaTemp.functionEventID != -1) playSE(motaSystem.getSE, motaSystem.SEVolume);
        std::tie(kind, val) = std::make_pair(stoi(initDialogue(info[1])), stoi(initDialogue(info[2])));
        if (!motaVariables.itemRecord[kind] || val != 1) {
            motaVariables.itemRecord[kind] = true;
            motaTemp.messageInfo.emplace_back(-4, std::to_string(kind), "");
        }
        act.item[kind] += val;
        if (obj != nullptr) obj->toDispose = true;
    }

    // 商店
    if (info[0] == "shop") {
        motaTemp.shopType = stoi(info[1]);
        motaTemp.addPower = split(info[2], "&");
        if (info.size() >= 4) motaTemp.initPrice = split(info[3], "&");
        else motaTemp.initPrice = { "0" };
        if (info.size() >= 5) motaTemp.rise = info[4];
        else motaTemp.rise = "0";
    }

    // npc
    if (info[0] == "npc") {
        kind = stoi(info[1]);
        motaTemp.voiceInfo = std::make_pair(kind, 0);
        motaTemp.voice.stop();
        if (std::filesystem::exists(std::format("assets\\voice\\npc_{}_{}.ogg", motaTemp.voiceInfo.first, motaTemp.voiceInfo.second))) {
            motaTemp.voiceBuffer.loadFromFile(std::format("assets\\voice\\npc_{}_{}.ogg", motaTemp.voiceInfo.first, motaTemp.voiceInfo.second));
            motaTemp.voice.setBuffer(motaTemp.voiceBuffer);
            motaTemp.voice.setVolume(motaSystem.SEVolume);
            motaTemp.voice.play();
        }
        for (auto [messID, messName, messContent] : motaData.npc[kind].npcInfo) {
            motaTemp.messageInfo.emplace_back(messID, messName, insertNewLines(initDialogue(messContent), 28));
        }
        if (!motaData.npc[kind].transName.empty()) {
            motaTemp.transEventName = motaData.npc[kind].transName;
            motaTemp.directlyFunction = motaData.npc[kind].directlyFunction;
        }
        if (obj != nullptr) obj->toDispose = motaData.npc[kind].fade;
        motaTemp.pause = true;
    }

    // 路障熔岩
    if (info[0] == "lava") {
        val = stoi(initDialogue(info[1]));
        act.hp = std::max(0, act.hp - val);
        if (act.hp == 0) Interpreter("gg").execute();
    }

    // 上楼
    if (info[0] == "up") {
        playSE(motaSystem.stairSE, motaSystem.SEVolume);
        move1("mting.png");
        std::string temp = std::format("{}:{}", motaVariables.variables[1], motaVariables.variables[2] + 1);
        if (int tgtmpid = motaData.searchMap(temp); tgtmpid != -1) {
            ++motaVariables.variables[2];
            act.mapID = tgtmpid;
            if (info.size() == 3) { // 当传送有目标xy时
                std::tie(kind, val) = std::make_pair(stoi(info[1]), stoi(info[2]));
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
    }

    // 下楼
    if (info[0] == "down") {
        playSE(motaSystem.stairSE, motaSystem.SEVolume);
        move1("mting.png");
        std::string temp = std::format("{}:{}", motaVariables.variables[1], motaVariables.variables[2] - 1);
        if (int tgtmpid = motaData.searchMap(temp); tgtmpid != -1) {
            --motaVariables.variables[2];
            act.mapID = tgtmpid;
            if (info.size() == 3) { // 当传送有目标xy时
                std::tie(kind, val) = std::make_pair(stoi(info[1]), stoi(info[2]));
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
    }

    // 传送
    if (info[0] == "move") {
        playSE(motaSystem.stairSE, motaSystem.SEVolume);
        move1("mting.png");
        int mpid = stoi(info[1]), mx = stoi(info[2]), my = stoi(info[3]);
        act.mapID = mpid;
        act.x = mx;
        act.y = my;
        screenData.player.direction = 0;
        screenData.loadMap(mpid, &screenData.visualMap);
        move2();
    }

    // 显示图片
    if (info[0] == "picon") {
        kind = stoi(initDialogue(info[1]));
        move1(info[2], kind);
    }

    // 移除图片
    if (info[0] == "picoff") {
        kind = stoi(initDialogue(info[1]));
        move2(kind);
    }

    // 修改变量
    if (info[0] == "var") {
        std::tie(kind, val) = std::make_pair(stoi(info[1]), stoi(info[3]));
        std::string Operator = info[2];
        if (Operator == "=") {
            motaVariables.variables[kind] = val;
        }
        else if (Operator == "+") {
            motaVariables.variables[kind] += val;
        }
        else if (Operator == "-") {
            motaVariables.variables[kind] -= val;
        }
        else if (Operator == "*") {
            motaVariables.variables[kind] *= val;
        }
        else if (Operator == "/") {
            motaVariables.variables[kind] /= val;
        }
        else if (Operator == "%") {
            motaVariables.variables[kind] %= val;
        }
    }

    // 消除事件
    if (info[0] == "erase") {
        screenData.visualMap.mapEvents[motaTemp.functionEventID].toDispose = true;
    }

    // 返回标题画面
    if (info[0] == "back") {
        motaTemp.gameOver = true;
    }

    // 结局
    if (info[0] == "ending") {
        motaTemp.ending = true;
    }

    // 切换bgm
    if (info[0] == "bgmswitch") {
        screenData.visualMap.bgmFile = info[1];
    }

    // 播放动画
    if (info[0] == "animate") {
        screenData.addEVAnimation(stoi(info[0]), screenData.visualMap.mapEvents[stoi(info[1])].x, screenData.visualMap.mapEvents[stoi(info[1])].y);
    }

    // 播放视频
    if (info[0] == "playvideo") {

    }

    // 下面是条件等对话相关的
    auto judgecond = [&](int a, int b, const std::string& s) {
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
    if (info[0] == "condition") {
        auto [x, y, z, p, q] = std::make_tuple(stoi(info[1]), info[2], stoi(info[3]), split(info[4], ";"), split(info[5], ";"));
        if (judgecond(x, z, y)) screenData.doOrder(p);
        else screenData.doOrder(q);
    }

    // 转换事件名
    if (info[0] == "switch") {
        motaTemp.transEventName = info[1];
    }
}

void Interpreter::openDoor(Object* obj) {
    for (int i = 0; i < 3; ++i) {
        ++obj->pos[1];
        screenData.waitCount(2);
    }
};

std::string Interpreter::replaceToVar(const std::string& source) {
    std::string result = source;
    auto replaceRegularExpression = [](std::string result, std::string expression, std::map <int, int>& valueHash) {
        std::regex pattern(expression); // 匹配形如expression的字符串
        std::smatch match;
        while (regex_search(result, match, pattern)) {
            if (match.size() == 2) {
                int index = stoi(match[1]);
                std::string change;
                if (valueHash.contains(index)) {
                    change = std::to_string(valueHash[index]);
                }
                else{
                    change = "0";
                }
                result = match.prefix().str() + change + match.suffix().str();
            }
        }
        return result;
    };
    result = replaceRegularExpression(result, R"(\[v(\d+)\])", motaVariables.variables); // 匹配形如[vx]的字符串
    result = replaceRegularExpression(result, R"(\[i(\d+)\])", screenData.actors[motaVariables.variables[0]].item); // 匹配形如[ix]的字符串
    return result;
}

std::string Interpreter::initDialogue(const std::string& source) {
    std::string result = replaceToVar(source);
    auto& act = screenData.actors[motaVariables.variables[0]];
    replaceAll(result, "[hp]", std::to_string(act.hp));
    replaceAll(result, "[atk]", std::to_string(act.atk));
    replaceAll(result, "[def]", std::to_string(act.def));
    replaceAll(result, "[mdef]", std::to_string(act.mdef));
    replaceAll(result, "[exp]", std::to_string(act.exp));
    replaceAll(result, "[gold]", std::to_string(act.gold));
    replaceAll(result, "[lv]", std::to_string(act.level));
    return result;
}

std::vector <std::string> Interpreter::initEventName(const std::string& source) {
    // 定义正则表达式模式，匹配命令字符串中的子命令
    std::regex pattern(R"(([^;(]+)(?:\(([^;]*)\))?;)");
    // 存储分割后的子命令
    std::vector<std::string> commands;

    // 使用迭代器进行匹配
    auto wordsBegin = std::sregex_iterator(source.begin(), source.end(), pattern);
    auto wordsEnd = std::sregex_iterator();

    // 遍历匹配结果，将每个命令存储到commands向量中
    for (std::sregex_iterator i = wordsBegin; i != wordsEnd; ++i) {
        std::smatch match = *i;
        std::string command = match.str();
        command.pop_back(); // 移除分号
        commands.push_back(command);
    }

    return commands; // 返回分割后的命令数组
}

std::vector <std::string> Interpreter::initOrder(const std::string &source) {
    if (!strInclude(source, "(")) {
        return {source};
    }

    // 找第一个和最后一个括号
    size_t leftBracketPos = source.find('(');
    size_t rightBracketPos = source.rfind(')');

    // 提取命令标识符和括号内的内容
    std::string commandIdentifier = source.substr(0, leftBracketPos);
    std::string contentInsideBrackets = source.substr(leftBracketPos + 1, rightBracketPos - leftBracketPos - 1);

    std::vector <std::string> result = {commandIdentifier};
    std::vector <std::string> para;
    size_t start = 0, level = 0;
    for (size_t i = 0; i < contentInsideBrackets.length(); ++i) {
        if (contentInsideBrackets[i] == '(') {
            ++level;
        }
        else if (contentInsideBrackets[i] == ')') {
            --level;
        }
        else if (contentInsideBrackets[i] == ',' && level == 0) {
            para.push_back(contentInsideBrackets.substr(start, i - start));
            start = i + 1;
        }
    }
    para.push_back(contentInsideBrackets.substr(start));
    result.insert(result.end(), para.begin(), para.end());
    return result;
}

void Player::update(int autoDirection) {
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
    int dir = autoDirection >= 0 ? autoDirection : Input::dir4();
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
    auto [ax, ay] = std::make_pair(act.x, act.y);
    int dir[4][2] = {{0, 1}, {0, -1}, {-1, 0}, {1, 0}};

    // 领域怪
    for (auto & i : dir) {
        if (auto [nx, ny] = std::make_pair(ax + i[0], ay + i[1]);screenData.visualMap.haveAnEvent(nx, ny)) {
            auto ev = screenData.visualMap.EcheckEvent(nx, ny);
            auto namelist = Interpreter::initEventName(ev->name);
            for (auto evname : namelist) {
                if (auto names = Interpreter::initOrder(evname); ev->exist && names[0] == "monster") {
                    if (auto en = motaData.enemies[stoi(names[1])]; en.getP(9)) {
                        if (int dmg = en.getSingleDamage(act.getDef()) * (100 + motaVariables.variables[5]) / 100.f; dmg > 0) {
                            screenData.addEVAnimation(en.animationID, ax, ay);
                            act.hp = std::max(0, act.hp - dmg);
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
        if (auto [nx1, ny1, nx2, ny2] = std::make_tuple(ax + dir[2 * i][0], ay + dir[2 * i][1], ax + dir[2 * i + 1][0], ay + dir[2 * i + 1][1]); screenData.visualMap.haveAnEvent(nx1, ny1) && screenData.visualMap.haveAnEvent(nx2, ny2)) {
            auto ev1 = screenData.visualMap.EcheckEvent(nx1, ny1), ev2 = screenData.visualMap.EcheckEvent(nx2, ny2);
            auto namelist1 = Interpreter::initEventName(ev1->name), namelist2 = Interpreter::initEventName(ev2->name);;
            for (const auto& evname1 : namelist1) {
                for (const auto& evname2 : namelist2) {
                    if (auto names1 = Interpreter::initOrder(evname1), names2 = Interpreter::initOrder(evname2); ev1->exist && ev2->exist && names1[0] == "monster" && names2[0] == "monster" && names1 == names2) {
                        if (auto en = motaData.enemies[stoi(names1[1])]; en.getP(10)) {
                            if (int dmg = en.getSingleDamage(act.getDef()) * 2 * (100 + motaVariables.variables[5]) / 100.f; dmg > 0) {
                                screenData.addEVAnimation(en.animationID, ax, ay);
                                act.hp = std::max(0, act.hp - dmg);
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
    screenData.doOrder(Interpreter::initEventName(ev->name), ev);
};

ScreenData::ScreenData() {
    transition.setSprite("system\\mting-trans.png");
    transition.opacity = 0;
    transition.z = 99;
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
    searchingRoad = false;
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
            auto namelist = Interpreter::initEventName(evname);
            for (const auto& eachname : namelist) {
                auto namesplt = Interpreter::initOrder(eachname);
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
    auto drawStatusText = [](const std::vector <std::pair <std::string, int> >& text, int y) {
        for (auto [txt, val] : text) {
            drawText(&motaSystem.window, &motaSystem.font, sf::IntRect(16, y, 160, 24), txt);
            drawText(&motaSystem.window, &motaSystem.font, sf::IntRect(16, y, 160, 24), std::to_string(val), 2);
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
    drawText(&motaSystem.window, &motaSystem.font, sf::IntRect(224, 0, 352, 64), strInclude(visualMap.mapName, ":") ? std::format("{} {} 层", motaData.motaName[motaVariables.variables[1]], motaVariables.variables[2]) : visualMap.mapName, 1, 28L, true);

    // 绘制主角icon
    drawImage(&motaSystem.window, &motaSystem.textureCache["character\\" + act.file], 16, 32, sf::IntRect(0, 0, 32, 32));
    drawText(&motaSystem.window, &motaSystem.font, sf::IntRect(64, 32, 96, 32), act.name);
    drawText(&motaSystem.window, &motaSystem.font, sf::IntRect(96, 32, 96, 32), std::format("Lv. {}", act.level), 2);

    // 状态显示
    std::string status[] = {"", "中毒", "衰弱"}, statustxt = "";
    for (auto s : act.status)
        statustxt += "|" + status[s];
    if (!statustxt.empty()) statustxt.erase(statustxt.begin());
    drawText(&motaSystem.window, &motaSystem.font, sf::IntRect(16, 72, 160, 24), "状态");
    drawText(&motaSystem.window, &motaSystem.font, sf::IntRect(16, 72, 160, 24), std::format("[{}]", act.status.empty() ? "正常" : statustxt), 2);

    // 绘制状态栏
    drawStatusText({std::make_pair("生命", act.hp),
                    std::make_pair("攻击", act.atk),
                    std::make_pair("防御", act.def),
                    std::make_pair("魔防", act.mdef)}, 120);
    if (act.status.contains(1)) {
        drawText(&motaSystem.window, &motaSystem.font, sf::IntRect(64, 128, 96, 16), std::format("({})", motaVariables.variables[5]), 0, 12L, false, sf::Color::Green);
    }
    if (act.status.contains(2)) {
        drawText(&motaSystem.window, &motaSystem.font, sf::IntRect(64, 152, 96, 16), std::format("(-{})", motaVariables.variables[6]), 0, 12L, false, sf::Color::Magenta);
        drawText(&motaSystem.window, &motaSystem.font, sf::IntRect(64, 176, 96, 16), std::format("(-{})", motaVariables.variables[6]), 0, 12L, false, sf::Color::Magenta);
    }
    drawStatusText({std::make_pair("经验", act.exp),
                    std::make_pair("金币", act.gold)}, 240);

    // 绘制钥匙
    drawImage(&motaSystem.window, &motaSystem.textureCache["character\\" + motaData.items[0].file], 16, 312, sf::IntRect(32 * motaData.items[0].pos[0], 32 * motaData.items[0].pos[1], 32, 32));
    drawImage(&motaSystem.window, &motaSystem.textureCache["character\\" + motaData.items[1].file], 16, 336, sf::IntRect(32 * motaData.items[1].pos[0], 32 * motaData.items[1].pos[1], 32, 32));
    drawImage(&motaSystem.window, &motaSystem.textureCache["character\\" + motaData.items[2].file], 16, 360, sf::IntRect(32 * motaData.items[2].pos[0], 32 * motaData.items[2].pos[1], 32, 32));
    drawText(&motaSystem.window, &motaSystem.font, sf::IntRect(16, 312, 160, 24), std::to_string(act.item[0]), 2);
    drawText(&motaSystem.window, &motaSystem.font, sf::IntRect(16, 336, 160, 24), std::to_string(act.item[1]), 2);
    drawText(&motaSystem.window, &motaSystem.font, sf::IntRect(16, 360, 160, 24), std::to_string(act.item[2]), 2);

    // 有ShortcutKey.txt则绘制Press L
    if (std::filesystem::exists("ref\\ShortcutKey.txt")) drawText(&motaSystem.window, &motaSystem.font, sf::IntRect(0, 456, 624, 24), "～Press L～", 2);
}

void ScreenData::showMap(const Map& gmap, float x, float y, float rate, bool visible, bool clear_device) {
    // 绘制地图事件的函数
    auto drawMap = [&](sf::Sprite &spr, int dx, int dy) {
        spr.setPosition(x + dx * rate, y + dy * rate);
        spr.setScale(rate, rate);
        motaSystem.window.draw(spr);
    };

    // 清屏
    if (clear_device) motaSystem.window.clear();

    // 状态栏
    if (visible) mapStatus();

    // 绘制地板
    sf::Sprite floor(motaSystem.textureCache["system\\floor.png"]);
    drawMap(floor, 0, 0);

    // 记录怪物
    motaTemp.floorEnemies.clear();

    // 复制角色信息
    auto& act = actors[motaVariables.variables[0]];

    // 绘制事件
    for (const auto& ev : gmap.mapEvents) {
        if (!ev.exist) continue;
        if (ev.triggerCondition[0] == 1 && motaVariables.variables[ev.triggerCondition[1]] != ev.triggerCondition[2]) continue;
        auto [dx, dy] = std::make_pair(ev.x * 32, ev.y * 32);
        sf::Sprite evspr(motaSystem.textureCache["character\\" + ev.file]);
        evspr.setTextureRect(sf::IntRect(32 * ((ev.pos[0] + ev.move * motaSystem.gameTime / (2 * (1 + motaSystem.frameRate)) ) % 4), 32 * ev.pos[1], 32, 32));
        drawMap(evspr, dx, dy);

        // 地图显示伤害
        auto namelist = Interpreter::initEventName(ev.name);
        for (const auto& evname : namelist) {
            if (auto names = Interpreter::initOrder(evname); rate == 1.f && act.item[3] > 0 && names[0] == "monster") {
                int eid = stoi(names[1]);
                if (!vectorFind(motaTemp.floorEnemies, motaData.enemies[eid])) {
                    motaTemp.floorEnemies.push_back(motaData.enemies[eid]);
                }
                auto en = motaData.enemies[eid];
                if (motaVariables.variables[4] % 3 >= 1) {
                    sf::Color clr;
                    auto dmg = en.getDamage(&act, &motaData.elements);
                    if (dmg == -1 || dmg >= act.hp) clr = sf::Color(169,169,169);
                    else if (dmg >= act.hp * 3 / 4) clr = sf::Color::Red;
                    else if (dmg >= act.hp / 2) clr = sf::Color::Yellow;
                    else if (dmg >= act.hp * 1 / 4) clr = sf::Color::Green;
                    else clr = sf::Color::White;
                    drawText(&motaSystem.window, &motaSystem.font, sf::IntRect(MAPX + 32 * ev.x + 1, MAPY + 32 * ev.y + 22 + 1, 32, 10), dmg == -1 ? "DIE" : std::to_string(dmg), 2, 8L, true, sf::Color::Black);
                    drawText(&motaSystem.window, &motaSystem.font, sf::IntRect(MAPX + 32 * ev.x, MAPY + 32 * ev.y + 22, 32, 10), dmg == -1 ? "DIE" : std::to_string(dmg), 2, 8L, true, clr);
                }

                // 第二档显伤显示临界
                if (motaVariables.variables[4] % 3 == 2) {
                    int crs = en.getCrisis(&act);
                    std::string crisis = std::to_string(crs);
                    if (crs == -1) crisis = "？";
                    else if (crs == 0) crisis = "";
                    drawText(&motaSystem.window, &motaSystem.font, sf::IntRect(MAPX + 32 * ev.x + 1, MAPY + 32 * ev.y + 10 + 1, 32, 10), crisis, 2, 8L, true, sf::Color::Black);
                    drawText(&motaSystem.window, &motaSystem.font, sf::IntRect(MAPX + 32 * ev.x, MAPY + 32 * ev.y + 10, 32, 10), crisis, 2, 8L, true);
                }
            }
        }
    }

    // 根据伤害排序
    if (rate == 1.f && act.item[3] > 0) {
        sort(motaTemp.floorEnemies.begin(), motaTemp.floorEnemies.end(), [&](Enemy a, Enemy b){
            int admg = a.getDamage(&act, &motaData.elements), bdmg = b.getDamage(&act, &motaData.elements);
            if (admg == -1 && bdmg != -1) return false;
            if (admg != -1 && bdmg == -1) return true;
            if (admg == -1 && bdmg == -1) return a.name < b.name;
            return admg < bdmg;
        });
    }

    // 显示角色行走图
    if (visible) {
        if (player.visible) {
            int dir[4][2] = {{0, 1}, {-1, 0}, {1, 0}, {0, -1}};
            auto [ax, ay] = std::make_pair(act.x * 32 - dir[player.direction][0] * 8 * player.movingCount, act.y * 32 - dir[player.direction][1] * 8 * player.movingCount);
            sf::Sprite evspr(motaSystem.textureCache["character\\" + act.file]);
            evspr.setTextureRect(sf::IntRect(32 * std::max(0, player.movingCount - 1), 32 * player.direction, 32, 32));
            drawMap(evspr, ax, ay);
        }
        if (player.movingCount > 0) --player.movingCount;

        // 显示动画
        std::queue <SpriteAnimation> temp;
        while (!animationQueue.empty()) {
            SpriteAnimation ani = animationQueue.front();
            animationQueue.pop();
            sf::Sprite anispr(motaSystem.textureCache["animation\\" + (ani.type.patterns[ani.currentFrame / (1 + motaSystem.frameRate)])]);
            anispr.setPosition(ani.actX, ani.actY);
            auto siz = motaSystem.textureCache["animation\\" + ani.type.patterns[ani.currentFrame / (1 + motaSystem.frameRate)]].getSize();
            anispr.setOrigin(siz.x / 2, siz.y / 2);
            motaSystem.window.draw(anispr);
            if (ani.currentFrame % (1 + motaSystem.frameRate) == 0 && ani.currentFrame / (1 + motaSystem.frameRate) == ani.type.SETime) {
                playSE(ani.type.SEFile, motaSystem.SEVolume);
            }
            ++ani.currentFrame;
            if (ani.currentFrame / (1 + motaSystem.frameRate) < ani.type.patterns.size()) {
                temp.push(ani);
            }
        }
        if (!temp.empty()) {
            swap(animationQueue, temp);
        }

        if (searchingRoad) {
            auto drawRect = [](sf::IntRect rect) {
                drawImage(&motaSystem.window, &motaSystem.textureCache[motaSystem.windowskinName], rect.left, rect.top, sf::IntRect(128, 64, 1, 32), abs((int)(135 - 15 * ((motaSystem.gameTime / (2 * (1 + motaSystem.frameRate))) % 16))) + 105, std::make_pair(1.f, rect.height / 32.0));
                drawImage(&motaSystem.window, &motaSystem.textureCache[motaSystem.windowskinName], rect.left + rect.width - 1, rect.top, sf::IntRect(159, 64, 1, 32), abs((int)(135 - 15 * ((motaSystem.gameTime / (2 * (1 + motaSystem.frameRate))) % 16))) + 105, std::make_pair(1.f, rect.height / 32.0));
                drawImage(&motaSystem.window, &motaSystem.textureCache[motaSystem.windowskinName], rect.left + 1, rect.top, sf::IntRect(129, 64, 30, 1), abs((int)(135 - 15 * ((motaSystem.gameTime / (2 * (1 + motaSystem.frameRate))) % 16))) + 105, std::make_pair((rect.width - 2) / 30.0, 1.f));
                drawImage(&motaSystem.window, &motaSystem.textureCache[motaSystem.windowskinName], rect.left + 1, rect.top + rect.height - 1, sf::IntRect(129, 95, 30, 1), abs((int)(135 - 15 * ((motaSystem.gameTime / (2 * (1 + motaSystem.frameRate))) % 16))) + 105, std::make_pair((rect.width - 2) / 30.0, 1.f));
                drawImage(&motaSystem.window, &motaSystem.textureCache[motaSystem.windowskinName], rect.left + 1, rect.top + 1, sf::IntRect(129, 65, 30, 30), abs((int)(135 - 15 * ((motaSystem.gameTime / (2 * (1 + motaSystem.frameRate))) % 16))) + 105, std::make_pair((rect.width - 2) / 30.0, (rect.height - 2) / 30.0));
            };
            drawRect(sf::IntRect(x + targetPos.first * 32, y + targetPos.second * 32, 32, 32));
        }
    }
}

void ScreenData::waitCount(int times) {
    if (times == 0) return;
    if (times != 1) {
        times *= (1 + motaSystem.frameRate);
    }
    for (int i = 0; i < times; ++i) {
        sf::Event event;
        while (motaSystem.window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                motaSystem.window.close();
                exit(0);
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                if (Input::mouseJudge[Input::NeedLeft]) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        Input::mouseJudge[Input::LeftClick] = true;
                        Input::mouseJudge[Input::NeedLeft] = false;
                    }
                }
                if (Input::mouseJudge[Input::NeedRight]) {
                    if (event.mouseButton.button == sf::Mouse::Right) {
                        Input::mouseJudge[Input::RightClick] = true;
                        Input::mouseJudge[Input::NeedRight] = false;
                    }
                }
            }
            if (event.type == sf::Event::MouseWheelScrolled) {
                if (Input::mouseJudge[Input::NeedScroll]) {
                    if (event.mouseWheelScroll.delta > 0) {
                        Input::mouseJudge[Input::ScrollUp] = true;
                        Input::mouseJudge[Input::NeedScroll] = false;
                    }
                    else if (event.mouseWheelScroll.delta < 0) {
                        Input::mouseJudge[Input::ScrollDown] = true;
                        Input::mouseJudge[Input::NeedScroll] = false;
                    }
                }
            }
        }
        Input::mouseJudge[Input::NeedLeft] = false;
        Input::mouseJudge[Input::NeedRight] = false;
        Input::mouseJudge[Input::NeedScroll] = false;
        // 刷新画面
        if (onMap) {
            showMap(screenData.visualMap, MAPX, MAPY);
        }
        motaGraphics.update(!onMap);
    }
}

void ScreenData::addAnimation(int id, int x, int y) {
    animationQueue.push({x, y, 0, motaData.animations[id]});
}

void ScreenData::addEVAnimation(int id, int x, int y) {
    addAnimation(id, MAPX + x * 32 + 16, MAPY + y * 32 + 16);
}

void ScreenData::loadData(int fileid) {
    // 存档文件名
    std::string filename = std::format("save\\save_{}.json", fileid);
    // 读取文件
    std::ifstream infile(filename);
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
    std::string filename = std::format("save\\save_{}.json", fileid);
    // 创建json对象
    nlohmann::json json;
    // 写入角色数据
    json["actors"] = actors;
    // 写入变量数据
    json["variables"] = motaVariables;
    // 存档成文件
    std::ofstream outfile(filename);
    outfile << json.dump();
    outfile.close();
}

void ScreenData::doOrder(const std::vector<std::string>& lists, Object* obj) {
    for (const auto& od : lists) {
        Interpreter(od).execute(obj);

        // 执行完事件才到下一步
        do {
            screenData.waitCount(1);
            motaSystem.scene->update();
            if (motaTemp.gameOver || motaTemp.ending) return; // 中断函数
        } while (motaTemp.pause && obj != nullptr);
    }
}

void ScreenData::transition1(int time) {
    if (transition.opacity == 0) return;
    int once = ceil(255.f / (time * (1 + motaSystem.frameRate)));
    for (int i = 0; i < time * (1 + motaSystem.frameRate); ++i) {
        transition.opacity = std::max(0, transition.opacity - once);
        waitCount(1);
    }
}

void ScreenData::transition2(int time) {
    if (transition.opacity == 255) return;
    int once = ceil(255.f / (time * (1 + motaSystem.frameRate)));
    for (int i = 0; i < time * (1 + motaSystem.frameRate); ++i) {
        transition.opacity = std::min(255, transition.opacity + once);
        waitCount(1);
    }
    waitCount(5);
}

void ScreenData::dispose() {
    motaGraphics.eraseImage(&transition);
}
