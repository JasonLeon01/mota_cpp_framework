#include <Game/stdafx.h>

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
    wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.from_bytes(str);
}

string wstr2str(const wstring& wstr)
{
    wstring_convert<std::codecvt_utf8<wchar_t>> converter;
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

void replaceAll(std::string& source, const std::string& target, const std::string& replacement) {
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
        else if (j > 0)
            i -= table[j - 1];
        else
            i++;
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
        if (c == L'[' && wideInput.substr(i, 3) == L"[s]")
            stopCounting = true;
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
    for (auto i = 0; i < len; ++i)
        result += to_string(*(arr + i)) + splt;
    result.pop_back();
    return result;
}

string arrayToString(vector <int> arr, string splt) {
    string result = "";
    for (auto i : arr)
        result += to_string(i) + splt;
    result.pop_back();
    return result;
}

string setToString(set <int> arr, string splt) {
    string result = "";
    for (auto i : arr)
        result += to_string(i) + splt;
    if (!result.empty()) result.pop_back();
    return result;
}

vector <string> readFile(const string& file) {
    fstream File(file);
    vector <string> lines;
    if (File.is_open()) {
        string line;
        while (getline(File, line)) {
            while (!line.empty() && (line.back() == '\r' || line.back() == '\n'))
                line.pop_back();
            if (!line.empty()) lines.push_back(line);
        }
    }
    File.close();
    return lines;
}

map <string, string> readData(const string& file, const string& splt) {
    map <string, string> ret;
    auto data = readFile(file);
    for (auto f : data)
        if (auto kv = split(f, splt); kv.size() > 1)
            ret.insert(make_pair(kv[0], kv[1]));
    return ret;
}

void saveFile(const string& file, const string& content) {
    ofstream File(file);
    if (File.is_open()) {
        File << content << endl;
    }
    File.close();
}
