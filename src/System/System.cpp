#include <Game/System/System.hpp>

System motaSystem;

void System::init() {
    // 显示加载进度
    auto processing = [&](const std::wstring& content) {
        sf::Text temptxt(content, motaSystem.font, 28);
        temptxt.setPosition(160, 400);
        window.clear();
        window.draw(temptxt);
        window.display();
    };

    // 读取数据
    auto processingAssets = [&](const std::string& subroot, const std::string& fext) {
        std::string inPath = std::format("assets\\{}", subroot);
        for (const auto& entry : std::filesystem::recursive_directory_iterator(inPath)) {
            if (entry.is_regular_file() && entry.path().extension() == fext) {
                auto filestr = std::string(subroot + "\\") + entry.path().filename().string();
                auto& cache = textureCache[filestr];
                if (cache.loadFromFile(std::format("assets\\{}", filestr))) {
                    cache.setSmooth(smooth);
                }
                else {
                    print(std::format("Failed to access {}", filestr));
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
    verticalSync = stoi(configSet["verticalSync"]);
    smooth = stoi(configSet["smooth"]);

    // 从exe文件中提取图标
    HICON hIcon = ExtractIcon(GetModuleHandle(NULL), "main.exe", 0);

    // 创建窗口
    window.create(sf::VideoMode(640 * resolutionRatio, 480 * resolutionRatio), str2wstr(mainSet["title"]), sf::Style::Titlebar | sf::Style::Close);

    // 设置窗口图标
    HWND hwnd = window.getSystemHandle();
    SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
    SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);

    // 截取必要部分缩放
    sf::View view(sf::FloatRect(0, 0, 640, 480));
    window.setView(view);

    // 读取字体
    if (font.loadFromFile("assets\\font\\" + fontName)) {
        font.setSmooth(smooth);
    }
    else {
        print(std::format("Failed to access {}", fontName));
    }

    processing(L"正在加载动画资源...");
    processingAssets("animation", ".png");

    processing(L"正在加载行走图资源...");
    processingAssets("character", ".png");

    processing(L"正在加载系统资源...");
    processingAssets("system", ".png");

    processing(L"正在加载个性图片资源...");
    processingAssets("picture", ".png");
}

void System::bgmSwitch(const std::string& file) {
    bgm.stop();
    bgm.openFromFile("assets\\bgm\\" + file);
    bgm.setLoop(true);
    bgm.setVolume(BGMVolume);
    bgm.play();
}
