#include <Game/stdafx.hpp>

Texture tempTexture;
queue <tuple <SoundBuffer*, Sound*, bool> > SE;

void playSE(const string& file, float volume) {
    if (!filesystem::exists("sound\\" + file)) {
        print(format("Haven't found file: {}", file));
        return;
    }
    SoundBuffer* buffer = new SoundBuffer;
    Sound* se = new Sound;
    buffer->loadFromFile("sound\\" + file);
    se->setBuffer(*buffer);
    se->setVolume(volume);
    SE.emplace(buffer, se, false);
}

void drawText(RenderWindow* window, Font* font, IntRect rect, const string& content, int pos, LONG size, bool bond, Color colour, float angle) {
    Text temptxt(str2wstr(content), *font, size);
    temptxt.setFillColor(colour);
    if (bond) temptxt.setStyle(Text::Bold);
    temptxt.setRotation(angle);
    auto txtsize = temptxt.getGlobalBounds();
    auto [wx, wy] = make_pair(rect.left, rect.top);
    pos = max(min(pos, 2), 0);
    wx += (rect.width - txtsize.width) / 2 * pos;
    wy += (rect.height - txtsize.height) / 2 * (pos == 1);
    temptxt.setPosition(wx, wy);
    window->draw(temptxt);
}

void drawImage(RenderWindow* window, Texture* texture, float x, float y, int opacity, pair <float, float> scale) {
    Sprite tempspr(*texture);
    tempspr.setPosition(x, y);
    tempspr.setScale(scale.first, scale.second);
    auto cl = tempspr.getColor();
    cl.a = opacity;
    tempspr.setColor(cl);
    window->draw(tempspr);
}

void drawImage(RenderWindow* window, Texture* texture, float x, float y, IntRect rect, int opacity, pair <float, float> scale) {
    Sprite tempspr(*texture);
    tempspr.setTextureRect(IntRect(rect.left, rect.top, rect.width, rect.height));
    tempspr.setPosition(x, y);
    tempspr.setScale(scale.first, scale.second);
    auto cl = tempspr.getColor();
    cl.a = opacity;
    tempspr.setColor(cl);
    window->draw(tempspr);
}

void drawOutterImage(RenderWindow* window, const string& file, float x, float y, IntRect rect, int opacity, pair <float, float> scale) {
    tempTexture.loadFromFile(file);
    Sprite tempspr(tempTexture);
    tempspr.setTextureRect(IntRect(rect.left, rect.top, rect.width, rect.height));
    tempspr.setPosition(x, y);
    tempspr.setScale(scale.first, scale.second);
    auto cl = tempspr.getColor();
    cl.a = opacity;
    tempspr.setColor(cl);
    window->draw(tempspr);
}

int quickPow(int num, int n) {
    int res = 1;
    while (n) {
        if (n & 1) res = res * num;
        num = num * num;
        n >>= 1;
    }
    return res;
}

void print(const string& content) {
    MessageBoxA(NULL, content.c_str(), "Hint", MB_SYSTEMMODAL | MB_OK);
}

int ynPrint(const string& content) {
    return MessageBoxA(NULL, content.c_str(), "Confirmation", MB_SYSTEMMODAL | MB_ICONEXCLAMATION | MB_YESNO);
}

wstring str2wstr(const string& str)
{
    wstring_convert<codecvt_utf8<wchar_t>> converter;
    return converter.from_bytes(str);
}

string wstr2str(const wstring& wstr)
{
    wstring_convert<codecvt_utf8<wchar_t>> converter;
    return converter.to_bytes(wstr);
}

bool strInclude(const string& source, const string& target) {
    return (source.find(target) != -1);
}

int strIncludeNum(const string& source, const string& target) {
    int cnt = 0, pos = 0;
    while ((pos = source.find(target, pos)) != string::npos) {
        ++cnt;
        pos += target.length();
    }
    return cnt;
}

void replaceAll(string& source, const string& target, const string& replacement) {
    int n = source.length(), m = target.length();

    auto buildPartialMatchTable = [&target]() {
        int m = target.length();
        vector<int> table(m, 0);
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

    vector<int> table = buildPartialMatchTable();
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

vector <int> allToInt(vector <string> strArray) {
    vector <int> intArray(strArray.size());
    ranges::transform(strArray, intArray.begin(), [](const string& str) {
        return stoi(str);
    });
    return intArray;
}

string insertNewLines(const string& input, int lineMax) {
    wstring_convert <codecvt_utf8 <wchar_t> > converter;
    auto wideInput = str2wstr(input);
    string result = "";
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

vector <string> split(const string& s, const string& separator) {
    vector <string> result;
    string::size_type start = 0, end = s.find(separator);
    while (end != string::npos) {
        result.push_back(s.substr(start, end - start));
        start = end + separator.length();
        end = s.find(separator, start);
    }
    result.push_back(s.substr(start));
    return result;
}

string arrayToString(int* arr, int len, string splt) {
    string result = "";
    for (auto i = 0; i < len; ++i) {
        result += to_string(*(arr + i)) + splt;
    }
    result.pop_back();
    return result;
}

string arrayToString(vector <int> arr, string splt) {
    string result = "";
    for (auto i : arr) {
        result += to_string(i) + splt;
    }
    result.pop_back();
    return result;
}

string setToString(set <int> arr, string splt) {
    string result = "";
    for (auto i : arr) {
        result += to_string(i) + splt;
    }
    if (!result.empty()) result.pop_back();
    return result;
}

vector <string> readFile(const string& file) {
    fstream File(file);
    vector <string> lines;
    if (File.is_open()) {
        string line;
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

map <string, string> readData(const string& file, const string& splt) {
    map <string, string> ret;
    auto data = readFile(file);
    for (auto f : data) {
        if (auto kv = split(f, splt); kv.size() > 1) {
            ret.insert(make_pair(kv[0], kv[1]));
        }
    }
    return ret;
}

void saveFile(const string& file, const string& content) {
    ofstream File(file);
    if (File.is_open()) {
        File << content << endl;
    }
    File.close();
}
