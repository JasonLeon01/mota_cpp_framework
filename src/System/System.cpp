#include <Game/System/System.hpp>

System motaSystem;

void System::init() {
    auto processing = [&](const wstring& content) {
        Text temptxt(content, motaSystem.font, 28);
        temptxt.setPosition(160, 400);
        window.clear();
        window.draw(temptxt);
        window.display();
    };
    auto processingAssets = [&](const string& subroot, const string& fext) {
        string inPath = format("graphics\\{}", subroot);
        for (const auto& entry : filesystem::recursive_directory_iterator(inPath)) {
            if (entry.is_regular_file() && entry.path().extension() == fext) {
                auto filestr = string(subroot + "\\") + entry.path().filename().string();
                auto& cache = textureCache[filestr];
                if (cache.loadFromFile(format("graphics\\{}", filestr))) {
                    cache.setSmooth(true);
                }
                else {
                    print(format("Failed to access {}", filestr));
                }
            }
        }
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

    // 从exe文件中提取图标
    HICON hIcon = ExtractIcon(GetModuleHandle(NULL), "main.exe", 0);

    // 创建窗口
    window.create(VideoMode(640 * resolutionRatio, 480 * resolutionRatio), str2wstr(mainSet["title"]), sf::Style::Titlebar | sf::Style::Close);

    // 设置窗口图标
    HWND hwnd = window.getSystemHandle();
    SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
    SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);

    // 截取必要部分缩放
    View view(FloatRect(0, 0, 640, 480));
    window.setView(view);
    if (font.loadFromFile("font\\" + fontName)) {
        font.setSmooth(true);
    }
    else {
        print(format("Failed to access {}", fontName));
    }

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
