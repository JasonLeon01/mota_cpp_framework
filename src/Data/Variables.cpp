#include <Game/Data/Variables.hpp>

Variables motaVariables;

void Variables::init() {
    // 初始化数据
    variables.clear();
    if (std::filesystem::exists("ref\\Variables.txt")) {
        auto data = readFile("ref\\Variables.txt");
        for (const auto& var : data) {
            auto info = split(var, ":");
            if (info.size() == 3) {
                variables[stoi(info[0])] = stoi(info[2]);
            }
        }
    }
    floorRecord.clear();
    eventRecord.clear();
    transRecord.clear();
    itemRecord.clear();
}
