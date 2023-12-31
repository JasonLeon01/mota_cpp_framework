#pragma once
#include <windows.h>
#include <tchar.h>
#include <codecvt>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <cmath>
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <set>
#include <any>
#include <utility>
#include <format>
#include <regex>
#include <filesystem>
#include <thread>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <nlohmann/json.hpp>

using namespace std;
using namespace sf;

#define as(i, a, b) if (i >= a && i <= b)
#define MAPX 224
#define MAPY 64
#define MAXSAVE 1000

extern Texture tempTexture;
extern queue <tuple <SoundBuffer*, Sound*, bool> > SE;

// 播放SE
void playSE(const string& file, float volume);

// 绘制文字
void drawText(RenderWindow* window, Font* font, IntRect rect, const string& content, int pos = 0, LONG size = 20, bool bond = false, Color colour = Color::White, float angle = 0);

// 直接绘制图像，不保留
void drawImage(RenderWindow* window, Texture* texture, float x, float y, int opacity = 255, pair <float, float> scale = make_pair(1.0, 1.0));

void drawImage(RenderWindow* window, Texture* texture, float x, float y, IntRect rect, int opacity = 255, pair <float, float> scale = make_pair(1.0, 1.0));

void drawOutterImage(RenderWindow* window, const string& file, float x, float y, IntRect rect, int opacity = 255, pair <float, float> scale = make_pair(1.0, 1.0));

// 快速幂
int quickPow(int num, int n);

// 弹出消息框
void print(const string& content);

// 弹出确认框
int ynPrint(const string& content);

// 字符串转宽字符串
wstring str2wstr(const string& str);

// 宽字符串转字符串
string wstr2str(const wstring& wstr);

// vector中是否包含某对象
template<typename T>
bool vectorFind(vector<T> source, T target) {
    return (find(source.begin(), source.end(), target) != source.end());
}

// 字符串是否包含某子串
bool strInclude(const string& source, const string& target);

// 字符串包含多少个某子串
int strIncludeNum(const string& source, const string& target);

// 字符串全部替换
void replaceAll(string& source, const string& target, const string& replacement);

// 字符串数组全部转换为int数组
vector <int> allToInt(vector <string> strArray);

// 给字符串每隔一定数量字符插入换行符
string insertNewLines(const string& input, int lineMax);

// 字符串分割
vector <string> split(const string& s, const string& separator);

// 将数组转化为字符串
string arrayToString(int* arr, int len, string splt = ",");

string arrayToString(vector <int> arr, string splt = ",");

string setToString(set <int> arr, string splt = ",");

// 读取文件（按行分隔）
vector <string> readFile(const string& file);

// 读取数据文件（固定格式）
map <string, string> readData(const string& file, const string& splt = ":");

// 存储文件（单行）
void saveFile(const string& file, const string& content);
