#pragma once
#include <Game/Data/Screen.hpp>

// 商店窗口
class WindowShop :public GameWindow {
public:
    std::string name, file, desc; // 商店名称、商店行走图文件名、商店描述
    int index, pos; // 当前选择项、当前位置
    std::vector <std::tuple <std::string, std::pair <int*, std::string>, std::vector <std::string> > > items; // 商品列表，tuple三个分别为商店选项描述、(需要的项（比如金币）,要求)、(购买后触发的事件)

    // 构造函数，初始化对象
    WindowShop(int x, int y);
    // 刷新
    void refresh() override;
};
