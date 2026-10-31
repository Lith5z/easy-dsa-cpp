// KMP算法，推导可见oi-wiki

#include <print>
#include <string>
#include <vector>

using namespace std;

vector<int> prefix_function(const string& s) {
    int n = (int)s.length();
    vector<int> pi(n);
    // pi[0] = 0
    for (int i = 1; i < n; i++) {
        // pi[i] <= pi[i-1] + 1，所以子串s[0...i]前后缀相同的最大长度是pi[i-1]=j
        // 对应的前缀是 s[0...j-1]，所以 j 正好指向要比较的下一位
        int j = pi[i - 1];
        // 一直匹配j，如果 s[i]==s[j]，前缀是s[0...j]，长度是 j+1 赋值给pi[i]
        while (j > 0 && s[i] != s[j]) j = pi[j - 1];
        // while的退出有两种可能
        // 1. j > 0 && s[i] == s[j] 这时候下一句if必然触发，j++然后赋值给pi[i]
        // 2. j == 0 这时候还需要检查s[i]==s[0]，如果是，pi[i]=1，也需要j++
        if (s[i] == s[j]) j++;
        pi[i] = j;
    }
    return pi;
}

// 前提是两个string里都没有`#`
vector<int> KMP_find(const string& text, const string& pattern) {
    string concat = pattern + '#' + text;
    int text_size = text.size(), pattern_size = pattern.size();
    vector<int> index;
    vector<int> prefix_arr = prefix_function(concat);
    for (int i = pattern_size + 1; i <= text_size + pattern_size; i++) {
        if (prefix_arr[i] == pattern_size)
            index.push_back(i - 2 * pattern_size);
    }
    return index;
}

// 传统的next数组可以由 next[j] = pi[j - 1] 得到
// pi:    0 1 2 2 0
// nxt -1 0 1 2 2 0
// 表示pattern的j位失配后，j需要回退到哪里
vector<int> get_next(const string& pattern) {
    vector<int> pi = prefix_function(pattern);
    int pattern_size = (int)pattern.size();
    vector<int> next(pattern_size + 1);

    next[0] = -1;
    for (int j = 1; j <= pattern_size; j++) {
        next[j] = pi[j - 1];
    }

    return next;
}

vector<int> KMP_find_with_next(const string& text, const string& pattern) {
    vector<int> index;
    if (pattern.empty() || text.empty() || pattern.size() > text.size())
        return index;

    vector<int> next = get_next(pattern);
    int text_size = text.size(), pattern_size = pattern.size();
    int i = 0, j = 0;  // 主串指针i 模式串指针j
    while (i < text_size) {
        if (j == -1 || text[i] == pattern[j]) {
            // 回退到起点next[0] or 当前位置匹配
            i++;
            j++;
            if (j == pattern_size) {
                index.push_back(i - j);  // 本次匹配在主串中的起始位置
                j = next[j];             // 保留最长相同前后缀，继续检查重叠匹配
            }
        }
        else {
            // 匹配失败使用next回退j指针
            j = next[j];
        }
    }

    return index;
}

int main() { println("{}", KMP_find_with_next("abcacababc", "abc")); }