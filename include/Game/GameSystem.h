#pragma once
#include "Game/stdafx.h"

// 播放SE
void playSE(const string& file);

// 绘制文字
void drawText(float dx, float dy, const string &content, LONG size = 20, bool bond = false, Color colour = Color::White, float angle = 0);

void drawText(IntRect rect, const string& content, int pos = 0, LONG size = 20, bool bond = false, Color colour = Color::White, float angle = 0);

// 直接绘制图像，不保留
void drawImage(const string& file, float x, float y, int opacity = 255, pair <float, float> scale = make_pair(1.0, 1.0));

void drawImage(const string& file, float x, float y, IntRect rect, int opacity = 255, pair <float, float> scale = make_pair(1.0, 1.0));

void drawOutterImage(const string& file, float x, float y, IntRect rect, int opacity = 255, pair <float, float> scale = make_pair(1.0, 1.0));

// 所有场景类的基类，仅包含纯虚函数
class SceneBase {
public:
    virtual void main() = 0;
    virtual void update() = 0;
};

// Image类，用于填充图像
class GameImage {
    friend class GameWindow;
    friend class GameGraphics;
public:
    float x, y, origin_x, origin_y, scale_x, scale_y, angle; // 坐标，原点，缩放，角度
    int z, width, height, sx, sy, opacity; // 层级，宽高，源坐标，不透明度
    bool visible; // 是否可见

    // 构造函数，初始化对象
    GameImage();
    GameImage(string file, float x = 0, float y = 0, int width = 0, int height = 0, int sx = 0, int sy = 0);
    // 设置图像并加入到列表
    void setSprite(string file, float x = 0, float y = 0, int width = 0, int height = 0, int sx = 0, int sy = 0);
    // 消除自身
    void dispose();
private:
    string imgFile;

    // 展示精灵
    void show();
};

//系统类，用来储存各种参数
class System {
public:
    RenderWindow window; // 游戏窗口
    Font font; // 游戏字体
    Music bgm; // 游戏音乐
    SceneBase* scene; // 所有场景的基类
    unsigned int gameTime; // 游戏时间
    short frameRate; // 刷新频率
    short windowOpacity; // 窗口不透明度
    float resolutionRatio; // 分辨率
    float BGMVolume; // BGM音量
    float SEVolume; // SE音量
    string windowskinName; // 窗口文件名
    string titleName; // 标题名
    string fontName; // 字体名
    string titleBGM; // 标题背景音乐
    string cursorSE; // 选择SE
    string decisionSE; // 确认SE
    string cancelSE; // 取消SE
    string buzzerSE; // 警告SE
    string shopSE; // 商店SE
    string saveSE; // 存档SE
    string loadSE; // 读档SE
    string gateSE; // 开门SE
    string stairSE; // 上下楼SE
    string getSE; // 获取道具SE
    map <string, Texture> textureCache; // 纹理缓存

    // 初始化函数
    void init();
    // 更改游戏bgm
    void bgmSwitch(const string& file);
}; extern System motaSystem;

//处理游戏按键的类
class GameKeyBoard {
public:
    // 全键盘按键列表
    int KeyBACK = 0x08;
    int KeyTAB = 0x09;
    int KeyRETURN = 0x0D;
    int KeySHIFT = 0x10;
    int KeyCTRL = 0x11;
    int KeyALT = 0x12;
    int KeyPAUSE = 0x13;
    int KeyCAPITAL = 0x14;
    int KeyESCAPE = 0x1B;
    int KeySPACE = 0x20;
    int KeyPRIOR = 0x21;
    int KeyNEXT = 0x22;
    int KeyEND = 0x23;
    int KeyHOME = 0x24;
    int KeyLEFT = 0x25;
    int KeyUP = 0x26;
    int KeyRIGHT = 0x27;
    int KeyDOWN = 0x28;
    int KeySELECT = 0x29;
    int KeyPRINT = 0x2A;
    int KeySNAPSHOT = 0x2C;
    int INSERT = 0x2D;
    int KeyDELETE;
    int Key0 = 0x30;
    int Key1 = 0x31;
    int Key2 = 0x32;
    int Key3 = 0x33;
    int Key4 = 0x34;
    int Key5 = 0x35;
    int Key6 = 0x36;
    int Key7 = 0x37;
    int Key8 = 0x38;
    int Key9 = 0x39;
    int KeyA = 0x41;
    int KeyB = 0x42;
    int KeyC = 0x43;
    int KeyD = 0x44;
    int KeyE = 0x45;
    int KeyF = 0x46;
    int KeyG = 0x47;
    int KeyH = 0x48;
    int KeyI = 0x49;
    int KeyJ = 0x4A;
    int KeyK = 0x4B;
    int KeyL = 0x4C;
    int KeyM = 0x4D;
    int KeyN = 0x4E;
    int KeyO = 0x4F;
    int KeyP = 0x50;
    int KeyQ = 0x51;
    int KeyR = 0x52;
    int KeyS = 0x53;
    int KeyT = 0x54;
    int KeyU = 0x55;
    int KeyV = 0x56;
    int KeyW = 0x57;
    int KeyX = 0x58;
    int KeyY = 0x59;
    int KeyZ = 0x5A;

    // 按键次数（用于判断是否双击）
    int hit_times = 0;
    // 按下
    bool press(int key);
    // 重新按下
    bool repeat(int key);
    // 按住
    bool trigger(int key);
    // 按下确定
    bool pressConfirm();
    // 重新按下确定
    bool repeatConfirm();
    // 按住确定
    bool triggerConfirm();
    // 按下取消
    bool pressCancel();
    // 重新按下取消
    bool repeatCancel();
    // 按住取消
    bool triggerCancel();
    // 双击
    bool doubleClick(int key);
    // 四方向判定
    int dir4();
private:
    map <int, int> key_hash, key_repeat, double_click;
}; extern GameKeyBoard motaKeyBoard;

// 所有游戏窗口的基类，包括纯虚函数Refresh，通过此类可以获取窗口属性，绘制矩形以及在窗口内描绘文字和图像
class GameWindow {
    friend class GameGraphics;
public:
    float x, y; // 坐标
    int z, width, height, opacity; // 层级，宽高，不透明度
    bool haveFunction, visible; // 是否有功能，是否可见
    virtual void refresh() = 0; // 刷新(纯虚函数)

    // 构造函数，初始化对象
    GameWindow();
    GameWindow(IntRect rect, int opacity = motaSystem.windowOpacity);
    // 绘制矩形
    void drawRect(IntRect rect);
    // 绘制窗口文字（xy为0时文字在窗口左上角）
    void drawWText(float x, float y, const string& content, LONG size = 20, bool bond = false, Color colour = Color::White) const;
    void drawWText(IntRect rect, const string& content, int pos = 0, LONG size = 20, bool bond = false, Color colour = Color::White) const;
    void drawWTextRotate(IntRect rect, const string& content, int pos = 0, LONG size = 20, bool bond = false, Color colour = Color::White, float angle = 0) const;
    void drawWTextSelect(IntRect rect, const string& content, int pos = 0, LONG size = 20, bool bond = false, Color colour = Color::White) const;
    // 绘制窗口图形（xy为0时文字在窗口左上角）
    void windowBitmap(const string& file, float x, float y, int opacity = 255) const;
    void windowBitmap(const string& file, float x, float y, IntRect rect, int opacity = 255) const;
    void windowBitmapSelect(const string& file, float x, float y, IntRect rect, int opacity = 255);
    // 绘制箭头
    void showArrow(IntRect rect, int now, int page, int type = 0, string pagestr = "");
    // 释放
    void dispose();
protected:
    Texture windowBackground;
    void showBackground(int x, int y, int width, int height);
};

// 处理带选择项窗口的类
class GameSelectWindow :public GameWindow {
public:
    int index, rectHeight;
    bool active;
    vector <string> items;

    // 构造函数，初始化对象
    GameSelectWindow();
    GameSelectWindow(int width, int rectheight, const vector <string>& item, bool visible = true);
    // 刷新
    void refresh() override;
    // 绘制项目
    void drawItem(int idx, Color colour);
};

class GameGraphics {
public:
    // 刷新画面
    void update(bool clear_device = true);
    // 插入图片
    void addImage(GameImage* obj);
    // 消除图片
    void eraseImage(GameImage* obj);
    // 插入窗口
    void addWindow(GameWindow* obj);
    // 消除窗口
    void eraseWindow(GameWindow* obj);
    // 消除画面
    void dispose();
private:
    vector <GameImage*> patterns;
    vector <GameWindow*> windows;
}; extern GameGraphics motaGraphics;
