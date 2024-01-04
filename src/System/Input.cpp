#include <Game/System/Input.hpp>

namespace Input {
    std::map <int, bool> mouseJudge;
}

bool Input::press(int key) {
    if ((GetAsyncKeyState(key) & 0x8000) != 0) {
        ++hit_times;
        return true;
    }
    return false;
}

bool Input::repeat(int key) {
    int result = GetAsyncKeyState(key) & 0x8000;

    // 如果按下的键不在 key_repeat 映射中，则将其添加到映射中并启动计时器
    if (result != 0) {
        ++hit_times;
        if (!key_repeat.contains(key)) {
            key_repeat[key] = 0;
            key_timers[key].restart();
            return true;
        }
        key_repeat[key]++;
    }
    else {
        key_repeat.erase(key);
        key_timers.erase(key);
        key_hash[key] = 0;
    }
    if (key_repeat.contains(key) && key_repeat[key] >= 2) {
        sf::Time elapsed = key_timers[key].getElapsedTime();
        if (elapsed.asMilliseconds() >= 100) {
            key_repeat[key] = 0;
            ++hit_times;
            return true;
        }
    }
    return false;
}

bool Input::trigger(int key) {
    int result = GetAsyncKeyState(key) & 0x8000;

    // 如果按下的键不在 key_hash 映射中，则将其添加到映射中
    if (key_hash[key] == 1 && result != 0) return false;
    if (result != 0) {
        ++hit_times;
        key_hash[key] = 1;
        return true;
    }
    else {
        key_hash[key] = 0;
        return false;
    }
}

bool Input::pressConfirm() {
    return (press(KeySPACE) || press(KeyRETURN)) || leftClick();
}

bool Input::repeatConfirm() {
    return (repeat(KeySPACE) || repeat(KeyRETURN)) || leftClick();
}

bool Input::triggerConfirm() {
    return (trigger(KeySPACE) || trigger(KeyRETURN)) || leftClick();
}

bool Input::pressCancel() {
    return (press(KeyESCAPE) || press(KeyX)) || rightClick();
}

bool Input::repeatCancel() {
    return (repeat(KeyESCAPE) || repeat(KeyX)) || rightClick();
}

bool Input::triggerCancel() {
    return (trigger(KeyESCAPE) || trigger(KeyX)) || rightClick();
}

bool Input::doubleClick(int key) {
    int result = trigger(key);

    // 如果按下的键不在 double_click 映射中，则将其添加到映射中
    if (result == 0) return false;
    ++hit_times;
    if (double_click[key] == 0) {
        double_click[key] = clock();
        return false;
    }
    else {
        if (clock() - double_click[key] <= 500) {
            double_click[key] = 0;
            return true;
        }
        else {
            double_click[key] = clock();
            return false;
        }
    }
}

int Input::dir4() {
    if (repeat(KeyDOWN)) return 0;
    if (repeat(KeyLEFT)) return 1;
    if (repeat(KeyRIGHT)) return 2;
    if (repeat(KeyUP)) return 3;
    return -1;
}

bool Input::leftClick() {
    mouseJudge[NeedLeft] = true;
    if (mouseJudge[LeftClick]) {
        mouseJudge[LeftClick] = false;
        return true;
    }
    return false;
}

bool Input::rightClick() {
    mouseJudge[NeedRight] = true;
    if (mouseJudge[RightClick]) {
        mouseJudge[RightClick] = false;
        return true;
    }
    return false;
}

bool Input::scrollUp() {
    mouseJudge[NeedScroll] = true;
    if (mouseJudge[ScrollUp]) {
        mouseJudge[ScrollUp] = false;
        return true;
    }
    return false;
}

bool Input::scrollDown() {
    mouseJudge[NeedScroll] = true;
    if (mouseJudge[ScrollDown]) {
        mouseJudge[ScrollDown] = false;
        return true;
    }
    return false;
}
