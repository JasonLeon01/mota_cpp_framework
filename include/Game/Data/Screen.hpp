#pragma once
#include <Game/System/Graphics.hpp>
#include <Game/Data/Data.hpp>

class Interpreter {
public:
    // 构造函数，初始化对象
    Interpreter(string order);
    // 执行命令
    void execute(Object* obj = nullptr);
private:
    string order;
};

// 玩家数据
class Player {
    friend class ScreenData;
public:
    int direction, step;// 方向，步数
    bool visible; // 是否可见

    // 更新
    void update();
    // 改变步数
    void changeSteps();
    // 执行事件指令
    void triggerEvent(Object* ev);
private:
    int movingCount;
};

// 游戏屏幕上显示数据的存储
class ScreenData {
public:
    map <int, Actor> actors; // 角色
    Player player; // 玩家
    Map visualMap; // 显示的地图
    bool onMap; // 判断是否在地图上，关闭之后将不再显示地图

    // 构造函数，初始化transition
    ScreenData();
    // 初始化
    void init();
    // 读取地图
    void loadMap(int mapID, Map* gmap);
    // 显示状态栏
    void mapStatus();
    // 显示地图（可缩放）
    void showMap(const Map& gmap, float x, float y, float rate = 1.f, bool visible = true, bool clear_device = true);
    // 等待
    void waitCount(int times);
    // 显示动画
    void addAnimation(int id, int x, int y);
    void addEVAnimation(int id, int x, int y);
    // 读档
    void loadData(int fileid);
    // 存档
    void saveData(int fileid);
    // 执行命令
    void doOrder(const vector <string>& lists, Object* obj = nullptr);
    // 执行渐变1（消失）
    void transition1(int time = 10);
    // 执行渐变2（黑屏）
    void transition2(int time = 10);
    // 消除画面
    void dispose();
private:
    queue <SpriteAnimation> animationQueue;
    GameImage transition;
}; extern ScreenData screenData;
