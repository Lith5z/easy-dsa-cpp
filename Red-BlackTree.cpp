/*
B-tree对于任意的插入顺序都是log n高度，但是代码上不好实现
解决这个问题的是红黑树，下面以LLRB/left-leaning red-black BST，左倾红黑树为例

先要介绍旋转操作，旋转可以将同一批数据调整成不同的树结构

左旋节点 C：找到 C 的右孩子 x，让 x 取代 C 在原树中的位置，
再将 x 的左子树交给 C 作为右子树

旋转前：                         左旋 C 后：
        F                                F
       / \                              / \
      D   C                            D   x
         / \                              / \
        E   x                            C   G
           / \                          / \
          H   G                        E   H

为了方便手画旋转，可以想象C和【右】孩子x先合并，然后C向下向【左】移动
右旋的操作类似

LLRB树就是增加了红连接（必须是左侧连接）的BST，每个2-3树都可以一一对应一个LLRB树
- 若2-3树高H，则LLRB高最大是2H+1，这就确保了LLRB树操作都是log n
- 2-3树是等高的，因而LLRB的黑边高度是相同的（红边只来自2-3树多元素的节点）
通过以下方法可以维护一个LLRB，同时保持和2-3树的等价性
- 插入时与BST相同，默认使用红连接
- 遇到不符合情况，作额外操作：
  - Right red link -> rotate left
  - Two consecutive left links -> rotate right
  - Red left and red right -> color flip

标准的红黑树比LLRB更加复杂，效率也更高，下面实现只带insert和contain的LLRB

上面仅挑选了跟代码实现相关的重点，具体内容推荐看课程原文
或者 https://algs4.cs.princeton.edu/33balanced/
代码参考 https://algs4.cs.princeton.edu/33balanced/RedBlackLiteBST.java.html
*/

#include <cassert>
#include <iostream>
#include <utility>

struct Node {
    int data;
    Node *left, *right;
    bool isRed;  // color of the parent link

    Node(int val, Node* l, Node* r, bool color)
        : data(val), left(l), right(r), isRed(color) {}
};

class LLRB {
private:
    Node* root = nullptr;

    // 单独写一个isRed()方法
    // 因为fix这里需要有大量的颜色检查，一个个判断nullptr很麻烦
    // 这里默认nullptr是黑边，因为红边必然指向插入的节点，不可能指向nullptr
    static bool isRed(const Node* node) {
        return node != nullptr && node->isRed;
    }

    // 左旋，并返回新的根
    Node* rotateLeft(Node* root) {
        assert(root && root->right);
        Node* p = root->right;
        root->right = p->left;
        p->left = root;
        // root.father-root和root-p的边颜色保留，只是节点交换位置
        std::swap(p->isRed, root->isRed);
        return p;
        // 返回以更新链接，让p取代原来的root
    }
    // 右旋，并返回新的根
    Node* rotateRight(Node* root) {
        assert(root && root->left);
        Node* p = root->left;
        root->left = p->right;
        p->right = root;
        std::swap(p->isRed, root->isRed);
        return p;
    }
    void flipColors(Node* p) {
        assert(p && p->left && p->right);
        p->isRed = !p->isRed;
        p->left->isRed = !p->left->isRed;
        p->right->isRed = !p->right->isRed;
    }

    // 修正三种错误情况
    // - Right red link -> rotate left
    // - Two consecutive left links -> rotate right
    // - Red left and red right -> color flip
    Node* fix(Node* root) {
        if (isRed(root->right) && !isRed(root->left)) root = rotateLeft(root);
        if (isRed(root->left) && isRed(root->left->left))
            // 短路求值的原因，条件里不需要检查root->left是否为空
            root = rotateRight(root);
        if (isRed(root->left) && isRed(root->right)) flipColors(root);
        return root;
    }

    // insert的私有方法，返回Node*以改变前驱链接
    Node* _insert(Node* root, int val) {
        if (root == nullptr) {
            // new node always use red
            return new Node(val, nullptr, nullptr, true);
        }

        if (val < root->data) {
            root->left = _insert(root->left, val);
        }
        else if (val > root->data) {
            root->right = _insert(root->right, val);
        }
        else {
            return root;  // 重复就可以直接返回了，不用下面的fix
        }

        return fix(root);
    }

    bool _contain(const Node* node, int val) const {
        if (node == nullptr) return false;
        if (node->data == val) return true;

        if (val < node->data) return _contain(node->left, val);
        return _contain(node->right, val);
    }

public:
    bool contain(int val) const { return _contain(root, val); }
    void insert(int val) {
        root = _insert(root, val);
        root->isRed = false;
    }
};

int main() {}