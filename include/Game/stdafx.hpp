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

#define as(i, a, b) if (i >= a && i <= b)
#define MAPX 224
#define MAPY 64
#define MAXSAVE 1000

extern sf::Texture tempTexture;
extern std::queue <std::tuple <sf::SoundBuffer*, sf::Sound*, bool> > SE;

// 播放SE
void playSE(const std::string& file, float volume);

// 绘制文字
void drawText(sf::RenderWindow* window, sf::Font* font, sf::IntRect rect, const std::string& content, int pos = 0, LONG size = 20, bool bond = false, sf::Color colour = sf::Color::White, float angle = 0);

// 直接绘制图像，不保留
void drawImage(sf::RenderWindow* window, sf::Texture* texture, float x, float y, int opacity = 255, std::pair <float, float> scale = std::make_pair(1.0, 1.0));

void drawImage(sf::RenderWindow* window, sf::Texture* texture, float x, float y, sf::IntRect rect, int opacity = 255, std::pair <float, float> scale = std::make_pair(1.0, 1.0));

void drawOutterImage(sf::RenderWindow* window, const std::string& file, float x, float y, sf::IntRect rect, int opacity = 255, std::pair <float, float> scale = std::make_pair(1.0, 1.0));

// 快速幂
int quickPow(int num, int n);

// 弹出消息框
void print(const std::string& content);

// 弹出确认框
int ynPrint(const std::string& content);

// 字符串转宽字符串
std::wstring str2wstr(const std::string& str);

// 宽字符串转字符串
std::string wstr2str(const std::wstring& wstr);

// vector中是否包含某对象
template<typename T>
bool vectorFind(std::vector<T> source, T target) {
    return (find(source.begin(), source.end(), target) != source.end());
}

// 字符串是否包含某子串
bool strInclude(const std::string& source, const std::string& target);

// 字符串包含多少个某子串
int strIncludeNum(const std::string& source, const std::string& target);

// 字符串全部替换
void replaceAll(std::string& source, const std::string& target, const std::string& replacement);

// 字符串数组全部转换为int数组
std::vector <int> allToInt(std::vector <std::string> strArray);

// 给字符串每隔一定数量字符插入换行符
std::string insertNewLines(const std::string& input, int lineMax);

// 字符串分割
std::vector <std::string> split(const std::string& s, const std::string& separator);

// 将数组转化为字符串
std::string arrayToString(int* arr, int len, std::string splt = ",");

std::string arrayToString(std::vector <int> arr, std::string splt = ",");

std::string setToString(std::set <int> arr, std::string splt = ",");

// 读取文件（按行分隔）
std::vector <std::string> readFile(const std::string& file);

// 读取数据文件（固定格式）
std::map <std::string, std::string> readData(const std::string& file, const std::string& splt = ":");

// 存储文件（单行）
void saveFile(const std::string& file, const std::string& content);
