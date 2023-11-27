#include <Game/Data/Data.hpp>

Data motaData;

void Data::init() {
    // 描绘进行中的画面
    auto processing = [](const wstring& content) {
        Text temptxt(content, motaSystem.font, 28);
        temptxt.setPosition(160, 400);
        motaSystem.window.clear();
        motaSystem.window.draw(temptxt);
        motaSystem.window.display();
    };

    // 加载数据
    auto processingData = [&](string subroot, const string& fext) {
        auto eachData = [&](string subroot, string filename) {
            if (filename == "blankmap.json") return;
            ifstream fileInfo(format("data\\{}\\{}", subroot, filename));
            nlohmann::json json;
            fileInfo >> json;
            if (!json.is_object()) {
                print("Invalid JSON format:" + filename);
                exit(0);
            }

            fileInfo.close();
            int fileid = stoi(split(split(filename, ".")[0], "_")[1]);

            // 主角数据
            if (subroot == "actor") {
                actors[fileid] = json.template get<Actor>();
                actors[fileid].ID = fileid;
                actors[fileid].exp = 0;
                actors[fileid].gold = 0;
                actors[fileid].item.clear();
            }

            // 动画数据
            if (subroot == "animation") {
                animations[fileid] = json.template get<Animation>();
            }

            // 属性数据
            if (subroot == "element") {
                elements[fileid] = json.template get<Element>();
                elements[fileid].haveBuff = false;
                if (strInclude(elements[fileid].description, "[v]")) elements[fileid].haveBuff = true;
            }

            // 敌人数据
            if (subroot == "enemy") {
                enemies[fileid] = json.template get<Enemy>();
                enemies[fileid].ID = fileid;
                if (enemies[fileid].element.empty()) {
                    enemies[fileid].element.push_back(0);
                }
            }

            // 物品数据
            if (subroot == "item") {
                items[fileid] = json.template get<Item>();
                items[fileid].ID = fileid;
            }

            // 地图数据
            if (subroot == "map") {
                maps[fileid].mapID = fileid;
                maps[fileid].mapName = json["mapName"].get<string>();
                maps[fileid].bgmFile = json["bgmFile"].get<string>();
                maps[fileid].mapEvents.clear();
                for (const auto& evinfo : json["mapEvents"]) {
                    Object tmpev = evinfo;
                    tmpev.exist = true;
                    tmpev.toDispose = false;
                    maps[fileid].mapEvents.push_back(tmpev);
                }
            }

            // npc数据
            if (subroot == "npc") {
                for (const auto& npcinfo : json["npcInfo"]) {
                    int item1 = npcinfo["Item1"].get<int>();
                    string item2 = npcinfo["Item2"].get<string>();
                    string item3 = npcinfo["Item3"].get<string>();
                    npc[fileid].npcInfo.emplace_back(item1, item2, item3);
                }
                npc[fileid].transName = json["transName"].get<string>();
                npc[fileid].fade = json["fade"].get<bool>();
                npc[fileid].directlyFunction = json["directlyFunction"].get<bool>();
            }
        };

        // 遍历文件夹
        string inPath = format("data\\{}", subroot);
        for (const auto& entry : filesystem::recursive_directory_iterator(inPath)) {
            if (entry.is_regular_file() && entry.path().extension() == fext) {
                eachData(subroot, entry.path().filename().string());
            }
        }
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
    processingData("actor", ".json");

    processing(L"正在加载动画数据...");
    processingData("animation", ".json");

    processing(L"正在加载属性数据...");
    processingData("element", ".json");

    processing(L"正在加载敌人数据...");
    processingData("enemy", ".json");

    processing(L"正在加载物品数据...");
    processingData("item", ".json");

    processing(L"正在加载地图数据...");
    processingData("map", ".json");

    processing(L"正在加载npc数据...");
    processingData("npc", ".json");

    auto data = readData("ref\\motaName.ini", "=");
    for (auto [key, value] : data) {
        motaName[stoi(key)] = value;
    }
}

int Data::searchMap(const string& mapname) {
    // 搜寻同名地图
    auto result = ranges::find_if(maps, [&](auto mp) {
        return mp.second.mapName == mapname;
    });
    return result == maps.end() ? -1 : result->first;
}
