#pragma once
#include <Game/System/System.hpp>

//处理游戏按键的类
namespace Input {
    // 全键盘按键列表
    const int KeyBACK = 0x08;
    const int KeyTAB = 0x09;
    const int KeyRETURN = 0x0D;
    const int KeySHIFT = 0x10;
    const int KeyCTRL = 0x11;
    const int KeyALT = 0x12;
    const int KeyPAUSE = 0x13;
    const int KeyCAPITAL = 0x14;
    const int KeyESCAPE = 0x1B;
    const int KeySPACE = 0x20;
    const int KeyPRIOR = 0x21;
    const int KeyNEXT = 0x22;
    const int KeyEND = 0x23;
    const int KeyHOME = 0x24;
    const int KeyLEFT = 0x25;
    const int KeyUP = 0x26;
    const int KeyRIGHT = 0x27;
    const int KeyDOWN = 0x28;
    const int KeySELECT = 0x29;
    const int KeyPRINT = 0x2A;
    const int KeySNAPSHOT = 0x2C;
    const int INSERT = 0x2D;
    const int KeyDELETE = 0x7F;
    const int Key0 = 0x30;
    const int Key1 = 0x31;
    const int Key2 = 0x32;
    const int Key3 = 0x33;
    const int Key4 = 0x34;
    const int Key5 = 0x35;
    const int Key6 = 0x36;
    const int Key7 = 0x37;
    const int Key8 = 0x38;
    const int Key9 = 0x39;
    const int KeyA = 0x41;
    const int KeyB = 0x42;
    const int KeyC = 0x43;
    const int KeyD = 0x44;
    const int KeyE = 0x45;
    const int KeyF = 0x46;
    const int KeyG = 0x47;
    const int KeyH = 0x48;
    const int KeyI = 0x49;
    const int KeyJ = 0x4A;
    const int KeyK = 0x4B;
    const int KeyL = 0x4C;
    const int KeyM = 0x4D;
    const int KeyN = 0x4E;
    const int KeyO = 0x4F;
    const int KeyP = 0x50;
    const int KeyQ = 0x51;
    const int KeyR = 0x52;
    const int KeyS = 0x53;
    const int KeyT = 0x54;
    const int KeyU = 0x55;
    const int KeyV = 0x56;
    const int KeyW = 0x57;
    const int KeyX = 0x58;
    const int KeyY = 0x59;
    const int KeyZ = 0x5A;
    const int KeyBra = 0xDB;
    const int KeyKet = 0xDD;

    // 鼠标按键枚举，第0位代表是否需要拉取
    enum mouseFunction {
        NeedLeft, LeftClick, NeedRight, RightClick, NeedScroll, ScrollUp, ScrollDown
    };
    // 鼠标事件map
    extern std::map <int, bool> mouseJudge;
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
    // 鼠标左点击
    bool leftClick();
    // 鼠标右点击
    bool rightClick();
    // 鼠标滚轮上滚
    bool scrollUp();
    // 鼠标滚轮下滚
    bool scrollDown();

    namespace {
        static int hit_times;
        std::map<int, int> key_hash, key_repeat, double_click;
        std::map<int, sf::Clock> key_timers;
    }
}
