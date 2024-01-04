#pragma once
#include <Game/Screen/WindowMessage.hpp>
#include <Game/Screen/WindowHint.hpp>
#include <Game/Screen/WindowShop.hpp>
#include <Game/Screen/WindowItem.hpp>
#include <Game/Screen/WindowEnemyBook.hpp>
#include <Game/Screen/WindowEnemyEncyclopedia.hpp>
#include <Game/Screen/WindowFly.hpp>
#include <Game/Screen/WindowLoadSave.hpp>

// 处理标题画面
class MotaTitle : public SceneBase {
public:
    // 主函数
    void main() override;
    // 更新
    void update() override;

private:
    GameImage spr; // 标题背景精灵
    GameSelectWindow slcWindow = GameSelectWindow({"新游戏", "继续", "退出"}, 32, 240); // 选择命令窗口
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
    std::string bgmFile; // BGM文件名
    sf::Image saveImage; // 画面截图
    GameSelectWindow menuWindow = GameSelectWindow({"物品道具", "存档", "读档", "游戏设置", "回到标题"}, 32, 128); // 菜单窗口
    GameSelectWindow configWindow = GameSelectWindow({"窗口大小：", "BGM音量：", "SE音量："}, 32, 344); // 设置窗口
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
    // 更新设置窗口
    void updateConfig();
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
    // 自动寻路
    void autoSearching(std::pair <int, int> target);
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
