/* KMP算法，推导可见oi-wiki */

#include <print>
#include <string>
#include <vector>

std::vector<int> prefixFunction(const std::string& s) {
    int n = (int)s.length();
    std::vector<int> pi(n);
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
std::vector<int> kmpFind(const std::string& text, const std::string& pattern) {
    std::string concat = pattern + '#' + text;
    int textSize = text.size(), patternSize = pattern.size();
    std::vector<int> index;
    std::vector<int> prefixArr = prefixFunction(concat);
    for (int i = patternSize + 1; i <= textSize + patternSize; i++) {
        if (prefixArr[i] == patternSize) index.push_back(i - 2 * patternSize);
    }
    return index;
}

// 传统的next数组可以由 next[j] = pi[j - 1] 得到
// pi:    0 1 2 2 0
// nxt -1 0 1 2 2 0
// 表示pattern的j位失配后，j需要回退到哪里
std::vector<int> getNext(const std::string& pattern) {
    std::vector<int> pi = prefixFunction(pattern);
    int patternSize = (int)pattern.size();
    std::vector<int> next(patternSize + 1);

    next[0] = -1;
    for (int j = 1; j <= patternSize; j++) {
        next[j] = pi[j - 1];
    }

    return next;
}

std::vector<int> kmpFindWithNext(const std::string& text,
                                 const std::string& pattern) {
    std::vector<int> index;
    if (pattern.empty() || text.empty() || pattern.size() > text.size())
        return index;

    std::vector<int> next = getNext(pattern);
    int textSize = text.size(), patternSize = pattern.size();
    int i = 0, j = 0;  // 主串指针i 模式串指针j
    while (i < textSize) {
        if (j == -1 || text[i] == pattern[j]) {
            // 回退到起点next[0] or 当前位置匹配
            i++;
            j++;
            if (j == patternSize) {
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

int main() {
    using namespace std;

    println("{}", kmpFindWithNext("abcacababc", "abc"));
}
