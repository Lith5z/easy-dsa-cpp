/*
Trie是一个存储字符串的树
相比于哈希表，Trie的优点是能方便的进行字符串前缀的相关操作

这里用数组实现取下一个字符的操作，内存开销大
只考虑a-z的字母表，忽略空串""
好处是char作为index，天然是按照字典序访问的

这样实现的Trie是一个26叉树，所以操作和树有共同点
可以对比BinaryTree.h和BST.cpp看
*/

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

constexpr int N = 26;

// 不用存储节点的char是什么，因为next的index就是char
// index of char c = c - 'a'
struct Node {
    bool is_word = false;
    Node* next[N] = {};
};

class Trie {
private:
    Node* root;          // 哨兵
    int word_count = 0;  // 不同单词数

    // 和二叉树的析构过程相同
    void _destroy(Node* node) {
        if (node == nullptr) return;
        for (int i = 0; i < N; i++) {
            _destroy(node->next[i]);
        }
        delete node;
    }

    // 找到word的最后一个字符对应node
    // const方法，不修改对象（contain的const要求）
    const Node* findNode(const std::string& word) const {
        Node* p = root;
        // 一个个字符匹配
        for (char c : word) {
            p = p->next[c - 'a'];
            if (p == nullptr) return nullptr;
        }
        // 此时p指向最后一个字符的节点
        return p;
    }

    // 把以p为起点字符串写入res
    // 传引用让不同调用都是同一个res和path
    void _collect(std::string& path, std::vector<std::string>& res,
                  const Node* p) const {
        if (p->is_word) {
            res.push_back(path);
        }
        // 遍历p的26个子树，递归
        for (int i = 0; i < N; i++) {
            if (p->next[i] != nullptr) {
                // 每个子树追加的字符是不一样的
                path.push_back('a' + i);
                _collect(path, res, p->next[i]);
                // 考虑下一个子树前要去掉上一个子树添加的字符
                path.pop_back();
            }
        }
    }

public:
    Trie() { root = new Node(); }
    ~Trie() { _destroy(root); }

    void insert(const std::string& word) {
        if (word.empty()) return;  // 忽略""

        Node *p = root, *pre;
        for (char c : word) {
            pre = p;
            p = p->next[c - 'a'];
            if (p == nullptr) {
                p = new Node();
                pre->next[c - 'a'] = p;
            }
        }
        // 此时p指向最后一个字符的节点
        if (!p->is_word) {  // 重复插入判断
            p->is_word = true;
            word_count++;
        }
    }
    bool contain(const std::string& word) const {
        const Node* p = findNode(word);
        return p != nullptr && p->is_word;
    }

    // 按照字典序，返回存储的所有字符串
    std::vector<std::string> collect() const {
        std::vector<std::string> result;
        result.reserve(word_count);
        std::string path;
        _collect(path, result, root);
        return result;
    }
    // 按照字典序，返回以prefix开头的所有字符串
    std::vector<std::string> collectWithPrefix(
        const std::string& prefix) const {
        const Node* start = findNode(prefix);
        if (start == nullptr) return {};

        std::vector<std::string> result;
        // prefix是const引用，不能直接传给_collect
        // 这里做一次拷贝
        std::string path = prefix;
        _collect(path, result, start);
        return result;
    }

    int size() const { return word_count; }
    bool empty() const { return word_count == 0; }
};

int main() {
    using namespace std;
    Trie t;
    for (const char* w : {"a", "awls", "sad", "sam", "sap", "same"}) {
        t.insert(w);
    }
    t.insert("sam");  // 重复插入
    assert(t.size() == 6);
    assert(t.contain("sam") && t.contain("same"));
    assert(!t.contain("sa"));     // 路径存在，但不是单词
    assert(!t.contain("samen"));  // 越过叶子
    assert((t.collect() ==
            vector<string>{"a", "awls", "sad", "sam", "same", "sap"}));
    assert((t.collectWithPrefix("sam") == vector<string>{"sam", "same"}));

    for (const string& s : t.collectWithPrefix("sa")) cout << s << '\n';
}

/*
remarks:
why insert/contain不写成BST那样的递归方法

首先BST的插入查询不是只能写成递归
类似Trie，添加一个pre保存父节点，就可以写成迭代，只要添加节点之后更新pre的连接就行

Trie的插入查询更适合迭代，因为匹配的对象是一个string
递归写法要维护目前匹配到第几个字符，只能额外传一个depth，每层都写depth+1
相比之下迭代就更简单了

BST因为每层比较的都是同一个val，不需要第二个参数

真正只能递归的是_collect和_destroy，因为必须要遍历全部26个子树
*/