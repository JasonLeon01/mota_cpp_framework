#pragma once
#include "stdafx.h"
#include "GameSystem.h"

// 地图事件
class GameEvent {
public:
    string name, file; // 事件名字，文件名
    int ID, pos[2], x, y, triggerCondition[3]; // 事件编号，位置，出现触发条件
    bool move, through, exist, toDispose; // 是否可移动，是否可穿透，是否存在，是否消除

    // 构造函数，初始化对象
    GameEvent();
    GameEvent(string name, int x = 0, int y = 0);
    // 比较，用于find等函数
    bool operator == (const GameEvent& ev) const;
    // 事件执行的操作
    void order(bool tempEV = false);
    // 事件结束的操作
    void endEvent();
private:
    void openDoor();
};

class GameMap {
public:
    int mapID; // 地图编号
    string mapName, bgmFile; // 地图名字，BGM文件名
    vector <GameEvent> mapEvents; // 地图事件

    // 判断两地图是否相等
    bool operator == (const GameMap& mp) const;
    // 在(x,y)是否有事件
    bool haveAnEvent(int x, int y);
    // 返回在(x,y)处的事件编号
    int checkEvent(int x, int y);
    // 返回在(x,y)处的事件指针
    GameEvent* EcheckEvent(int x, int y);
    // 判断在(x,y)处是否可通行
    bool passible(int x, int y);
};

// 游戏角色
class GameActors {
public:
    string name, file; // 角色名字，文件名
    set <int> status; // 角色状态
    int ID, level, hp, atk, def, mdef, exp, gold, mapID, x, y, item[99], animationID; // 角色编号，等级，生命值，攻击力，防御力，魔防，经验值，金钱，所在地图，坐标，道具，动画编号

    // 获取实际攻防（算上衰弱效果）
    [[nodiscard]] int getAtk() const;
    [[nodiscard]] int getDef() const;
};

// 玩家数据
class GamePlayer {
    friend class ScreenData;
public:
    int direction, step;// 方向，步数
    bool visible; // 是否可见

    // 更新
    void update();
    // 改变步数
    void changeSteps();
private:
    int movingCount;
};

// 敌人数据
class GameEnemy {
public:
    string name, file; // 敌人名字，文件名
    vector <int> element; // 敌人属性
    int ID, pos, hp, atk, def, conatk, exp, gold, animationID; // 敌人编号，位置，生命值，攻击力，防御力，连击攻击力，经验值，金钱，动画编号

    // 重载等于等于号
    bool operator == (const GameEnemy& en) const;
    // 获取伤害
    int getDamage(int atk = 0, int def = 0, int mdef = 0);
    // 获取真实防御
    int getDef();
    // 是否拥有某属性
    bool getP(int p);
    // 获取临界
    int getCrisis();
    // 获取属性名字和描述
    pair <string, string> getElement(int pid, string buff = "");
};

// 属性名字和描述
struct GameElement {
    string name, description; // 属性名字，描述
    bool haveBuff; // 是否有buff
};

// 物品数据
struct GameItem {
    string name, description, file; // 物品名字，描述，文件名
    int ID, pos[2], price; // 物品编号，位置，价格
    bool usable, cost; // 是否可用，是否消耗
};

// 动画数据
struct GameAnimation {
    vector <string> pattern; // 动画帧
    string SEFile; // 音效文件名
    int SETime; // 音效播放的时间
};

// 动画播放器的数据
struct SpriteAnimation {
    int actX, actY, currentFrame; // 动画的x，y坐标，当前帧
    GameAnimation type; // 动画类型
};

// npc信息
struct GameNPC {
    vector <tuple <int, string, string> > npcInfo; // npc信息
    string transName; // 转换后的名字
    bool fade, directlyFunction; // 是否淡入淡出，转换后直接触发
};

// 一切数据的存储器
class GameData {
public:
    map <int, GameActors> actors; // 角色
    map <int, GameAnimation> animations; // 动画
    map <int, GameElement> elements; // 属性
    map <int, GameEnemy> enemies; // 敌人
    map <int, GameItem> items; // 道具
    map <int, GameMap> maps; // 地图
    map <int, GameNPC> npc; // npc
    map <int, string> motaName; // 魔塔名

    // 初始化
    void init();
    // 搜索地图
    int searchMap(const string& mapname);
}; extern GameData motaData;

// 临时变量的存储
class GameTemp {
public:
    int battleEnemyID; // 战斗的敌人ID
    int shopType; // 商店类型
    int shopID; // 商店编号
    int functionEventID; // 正在处理的事件ID
    bool closeMS; // 关闭状态栏
    bool directlyFunction; // 转换后直接触发
    bool gameOver; // 游戏结束
    string transEventName; // 转换的事件名
    vector <tuple <int, string, string> > messageInfo; // 消息相关
    vector <GameEnemy> floorEnemies; // 记录当前楼层怪物

    // 初始化
    void init();
}; extern GameTemp motaTemp;

// 游戏变量的存储
class GameVariables {
public:
    int variables[100]; // 变量
    map <int, vector <int> > floorRecord; // 楼层记录
    map <int, set <int> > eventRecord; // 记录处理过的事件
    map <int, map <int, string> > transRecord; // 记录变换过名字的事件
    map <int, bool> itemRecord; // 记录获得过的道具

    // 初始化
    void init();
    // 将形如[x]的字符串替换为其对应序号变量
    string replaceToVar(const string& source);
    // 初始化对话字符串，按对话框里面的全部替换
    string initDialogue(const string& source);
}; extern GameVariables motaVariables;

// 游戏屏幕上显示数据的存储
class ScreenData {
public:
    map <int, GameActors> actors; // 角色
    GamePlayer player; // 玩家
    GameMap visualMap; // 显示的地图
    bool onMap; // 判断是否在地图上，关闭之后将不再显示地图

    // 构造函数，初始化transition
    ScreenData();
    // 初始化
    void init();
    // 读取地图
    void loadMap(int mapID, GameMap* gmap);
    // 显示状态栏
    void mapStatus();
    // 显示地图（可缩放）
    void showMap(const GameMap& gmap, float x, float y, float rate = 1.f, bool visible = true, bool clear_device = true);
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
    void doOrder(const vector <string>& lists);
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
