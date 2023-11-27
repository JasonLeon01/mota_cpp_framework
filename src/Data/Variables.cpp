#include <Game/Data/Variables.hpp>

Variables motaVariables;

void Variables::init() {
    // 初始化数据
    variables.clear();
    if (filesystem::exists("ref\\Variables.txt")) {
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

string Variables::replaceToVar(const string& source) {
    string result = source;
    regex pattern("\\[([0-9]+)\\]"); // 匹配形如"[x]"的字符串
    smatch match;
    while (regex_search(result, match, pattern)) {
        if (match.size() == 2) {
            int index = stoi(match[1]);
            string change;
            if (variables.contains(index)) {
                change = to_string(variables[index]);
            }
            else{
                change = "0";
            }
            result = match.prefix().str() + change + match.suffix().str();
        }
    }
    return result;
}

string Variables::initDialogue(const string& source) {
    string result = replaceToVar(source);
    return result;
}
