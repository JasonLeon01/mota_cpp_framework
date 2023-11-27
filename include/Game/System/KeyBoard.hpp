#pragma once
#include <Game/stdafx.hpp>

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
    int KeyBra = 0xDB;
    int KeyKet = 0xDD;

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
    map<int, sf::Clock> key_timers;
}; extern GameKeyBoard motaKeyBoard;
