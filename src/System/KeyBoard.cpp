#include <Game/System/KeyBoard.hpp>

GameKeyBoard motaKeyBoard;

bool GameKeyBoard::press(int key) {
    if ((GetAsyncKeyState(key) & 0x8000) != 0) {
        ++hit_times;
        return true;
    }
    return false;
}

bool GameKeyBoard::repeat(int key) {
    int result = GetAsyncKeyState(key) & 0x8000;
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
        Time elapsed = key_timers[key].getElapsedTime();
        if (elapsed.asMilliseconds() >= 150) {
            key_repeat[key] = 0;
            ++hit_times;
            return true;
        }
    }
    return false;
}

bool GameKeyBoard::trigger(int key) {
    int result = GetAsyncKeyState(key) & 0x8000;
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

bool GameKeyBoard::pressConfirm() {
    return (press(KeySPACE) || press(KeyRETURN));
}

bool GameKeyBoard::repeatConfirm() {
    return (repeat(KeySPACE) || repeat(KeyRETURN));
}

bool GameKeyBoard::triggerConfirm() {
    return (trigger(KeySPACE) || trigger(KeyRETURN));
}

bool GameKeyBoard::pressCancel() {
    return (press(KeyESCAPE) || press(KeyX));
}

bool GameKeyBoard::repeatCancel() {
    return (repeat(KeyESCAPE) || repeat(KeyX));
}

bool GameKeyBoard::triggerCancel() {
    return (trigger(KeyESCAPE) || trigger(KeyX));
}

bool GameKeyBoard::doubleClick(int key) {
    int result = trigger(key);
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

int GameKeyBoard::dir4() {
    if (repeat(KeyDOWN)) return 0;
    if (repeat(KeyLEFT)) return 1;
    if (repeat(KeyRIGHT)) return 2;
    if (repeat(KeyUP)) return 3;
    return -1;
}
