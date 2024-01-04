#include <Game/Screen/MotaScene.hpp>

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // 对各类数据结构的初始化
    motaSystem.init();
    motaData.init();
    motaVariables.init();
    motaTemp.init();

    // 设置鼠标素材
    sf::Cursor cursor;
    cursor.loadFromPixels(motaSystem.textureCache["system\\mouse.png"].copyToImage().getPixelsPtr(), {16, 16}, {0, 0});
    motaSystem.window.setMouseCursor(cursor);

    // 设置垂直同步和帧率
    motaSystem.window.setVerticalSyncEnabled(motaSystem.verticalSync);
    motaSystem.window.setFramerateLimit(15 + motaSystem.frameRate * 15);

    // 清空屏幕，将场景定位到标题界面
    motaSystem.window.clear();
    motaSystem.scene = new MotaTitle;

    // 游戏的主循环
    while (motaSystem.scene != nullptr && motaSystem.window.isOpen()) {
        motaSystem.scene->main();
    }

    // 释放图形类型
    screenData.dispose();
    motaGraphics.dispose();
    return 0;
}
