#include <Game/stdafx.hpp>

sf::Texture tempTexture;
std::queue <std::tuple <sf::SoundBuffer*, sf::Sound*, bool> > SE;

void playSE(const std::string& file, float volume) {
    // 如果文件不存在，直接返回
    if (!std::filesystem::exists("assets\\se\\" + file)) {
        print(std::format("Haven't found file: {}", file));
        return;
    }

    // 将音效读取至buffer
    auto* buffer = new sf::SoundBuffer;
    auto* se = new sf::Sound;
    if (!buffer->loadFromFile("assets\\se\\" + file)) {
        print(std::format("Failed to access {}", file));
    }

    // 设置音效的音量
    se->setBuffer(*buffer);
    se->setVolume(volume);
    SE.emplace(buffer, se, false);
}

void drawText(sf::RenderWindow* window, sf::Font* font, sf::IntRect rect, const std::string& content, int pos, LONG size, bool bond, sf::Color colour, float angle) {
    // 读取至Text对象
    sf::Text temptxt(str2wstr(content), *font, size);

    // 设置文字的颜色、粗体、旋转角度
    temptxt.setFillColor(colour);
    if (bond) temptxt.setStyle(sf::Text::Bold);
    temptxt.setRotation(angle);

    // 设置文字位置
    auto txtsize = temptxt.getGlobalBounds();
    auto [wx, wy] = std::make_pair(rect.left, rect.top);
    pos = std::max(std::min(pos, 2), 0);
    wx += (rect.width - txtsize.width) / 2 * pos;
    wy += (rect.height - txtsize.height) / 2 * (pos == 1);
    temptxt.setPosition(wx, wy);
    window->draw(temptxt);
}

void drawImage(sf::RenderWindow* window, sf::Texture* texture, float x, float y, int opacity, std::pair <float, float> scale) {
    // 读取至Sprite对象
    sf::Sprite tempspr(*texture);

    // 设置图片的位置、缩放、不透明度
    tempspr.setPosition(x, y);
    tempspr.setScale(scale.first, scale.second);
    auto cl = tempspr.getColor();
    cl.a = opacity;
    tempspr.setColor(cl);
    window->draw(tempspr);
}

void drawImage(sf::RenderWindow* window, sf::Texture* texture, float x, float y, sf::IntRect rect, int opacity, std::pair <float, float> scale) {
    sf::Sprite tempspr(*texture);
    tempspr.setTextureRect(sf::IntRect(rect.left, rect.top, rect.width, rect.height));
    tempspr.setPosition(x, y);
    tempspr.setScale(scale.first, scale.second);
    auto cl = tempspr.getColor();
    cl.a = opacity;
    tempspr.setColor(cl);
    window->draw(tempspr);
}

void drawOutterImage(sf::RenderWindow* window, const std::string& file, float x, float y, sf::IntRect rect, int opacity, std::pair <float, float> scale) {
    if (!tempTexture.loadFromFile(file)) {
        print(std::format("Failed to access {}", file));
    }
    sf::Sprite tempspr(tempTexture);
    tempspr.setTextureRect(sf::IntRect(rect.left, rect.top, rect.width, rect.height));
    tempspr.setPosition(x, y);
    tempspr.setScale(scale.first, scale.second);
    auto cl = tempspr.getColor();
    cl.a = opacity;
    tempspr.setColor(cl);
    window->draw(tempspr);
}

int quickPow(int num, int n) {
    // 快速幂
    int res = 1;
    while (n) {
        if (n & 1) res = res * num;
        num = num * num;
        n >>= 1;
    }
    return res;
}

void print(const std::string& content) {
    MessageBoxA(NULL, content.c_str(), "Hint", MB_SYSTEMMODAL | MB_OK);
}

int ynPrint(const std::string& content) {
    return MessageBoxA(NULL, content.c_str(), "Confirmation", MB_SYSTEMMODAL | MB_ICONEXCLAMATION | MB_YESNO);
}

std::wstring str2wstr(const std::string& str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.from_bytes(str);
}

std::string wstr2str(const std::wstring& wstr)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.to_bytes(wstr);
}

bool strInclude(const std::string& source, const std::string& target) {
    return (source.find(target) != -1);
}

int strIncludeNum(const std::string& source, const std::string& target) {
    int cnt = 0, pos = 0;
    while ((pos = source.find(target, pos)) != std::string::npos) {
        ++cnt;
        pos += target.length();
    }
    return cnt;
}

void replaceAll(std::string& source, const std::string& target, const std::string& replacement) {
    // KMP算法
    int n = source.length(), m = target.length();

    auto buildPartialMatchTable = [&target]() {
        int m = target.length();
        std::vector<int> table(m, 0);
        int len = 0, i = 1;
        while (i < m) {
            if (target[i] == target[len]) {
                len++;
                table[i++] = len;
            }
            else {
                if (len != 0) len = table[len - 1];
                else table[i++] = 0;
            }
        }
        return table;
    };

    std::vector<int> table = buildPartialMatchTable();
    int i = 0;
    while (i < n) {
        int j = 0;
        while (j < m && source[i] == target[j]) {
            i++;
            j++;
        }
        if (j == m) {
            source.replace(i - m, m, replacement);
            i -= m - 1;
        }
        else if (j > 0) {
            i -= table[j - 1];
        }
        else {
            i++;
        }
    }
}

std::vector <int> allToInt(std::vector <std::string> strArray) {
    std::vector <int> intArray(strArray.size());
    std::ranges::transform(strArray, intArray.begin(), [](const std::string& str) {
        return stoi(str);
    });
    return intArray;
}

std::string insertNewLines(const std::string& input, int lineMax) {
    std::wstring_convert <std::codecvt_utf8 <wchar_t> > converter;
    auto wideInput = str2wstr(input);
    std::string result = "";
    int cnt = 0;
    bool stopCounting = false;
    for (int i = 0; i < wideInput.length(); ++i) {
        auto c = wideInput[i];
        if (c == L'\n') {
            result += converter.to_bytes(c);
            cnt = 0;
            stopCounting = false;
            continue;
        }
        if (stopCounting) {
            result += converter.to_bytes(c);
            continue;
        }
        if (c == L'[' && wideInput.substr(i, 3) == L"[s]") {
            stopCounting = true;
        }
        if (isascii(c)) cnt += 1;
        else cnt += 2;
        result += converter.to_bytes(c);
        if (cnt >= lineMax) {
            result += "\n";
            cnt = 0;
        }
    }
    return result;
}

std::vector <std::string> split(const std::string& s, const std::string& separator) {
    std::vector <std::string> result;
    std::string::size_type start = 0, end = s.find(separator);
    while (end != std::string::npos) {
        result.push_back(s.substr(start, end - start));
        start = end + separator.length();
        end = s.find(separator, start);
    }
    result.push_back(s.substr(start));
    return result;
}

std::string arrayToString(int* arr, int len, std::string splt) {
    std::string result = "";
    for (auto i = 0; i < len; ++i) {
        result += std::to_string(*(arr + i)) + splt;
    }
    result.pop_back();
    return result;
}

std::string arrayToString(std::vector <int> arr, std::string splt) {
    std::string result = "";
    for (auto i : arr) {
        result += std::to_string(i) + splt;
    }
    result.pop_back();
    return result;
}

std::string setToString(std::set <int> arr, std::string splt) {
    std::string result = "";
    for (auto i : arr) {
        result += std::to_string(i) + splt;
    }
    if (!result.empty()) result.pop_back();
    return result;
}

std::vector <std::string> readFile(const std::string& file) {
    std::fstream File(file);
    std::vector <std::string> lines;
    if (File.is_open()) {
        std::string line;
        while (getline(File, line)) {
            while (!line.empty() && (line.back() == '\r' || line.back() == '\n')) {
                line.pop_back();
            }
            if (!line.empty()) lines.push_back(line);
        }
    }
    File.close();
    return lines;
}

std::map <std::string, std::string> readData(const std::string& file, const std::string& splt) {
    std::map <std::string, std::string> ret;
    auto data = readFile(file);
    for (auto f : data) {
        if (auto kv = split(f, splt); kv.size() > 1) {
            ret.insert(std::make_pair(kv[0], kv[1]));
        }
    }
    return ret;
}

void saveFile(const std::string& file, const std::string& content) {
    std::ofstream File(file);
    if (File.is_open()) {
        File << content << std::endl;
    }
    File.close();
}
