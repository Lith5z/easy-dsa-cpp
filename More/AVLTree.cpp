/*
AVL树，旋转平衡树
通过旋转操作维护树的高度，让左右树的高度差不超过1
这是我们课程的内容，不在CS61B里

具体实现基本参考了LLRB
以及 https://www.hello-algo.com/chapter_tree/avl_tree/
*/

#include <algorithm>
#include <cassert>
#include <iostream>

struct Node {
    int data;
    Node *left, *right;
    int height;  // 按照边数计算，叶节点为0，空树为-1

    Node(int val, Node* l = nullptr, Node* r = nullptr, int h = 0)
        : data(val), left(l), right(r), height(h) {}
};

class AVLTree {
private:
    Node* root = nullptr;

    // 单独写获取height和bf的方法，原因和LLRB的isRed类似
    // 避免对nullptr的检查
    static int height(const Node* node) {
        if (node == nullptr)
            return -1;
        else
            return node->height;
    }
    // 平衡因子bf，注意有的教材是左减右
    static int balanceFactor(const Node* node) {
        if (node == nullptr)
            return 0;
        else
            return height(node->right) - height(node->left);
    }
    // 只重算node自己的高度，前提是孩子的高度已经是对的
    static void updateHeight(Node* node) {
        node->height = std::max(height(node->left), height(node->right)) + 1;
    }

    // 左单旋转的操作，参数root为bf失衡的节点
    Node* rotateLeft(Node* root) {
        assert(root && root->right);
        Node* p = root->right;
        root->right = p->left;
        p->left = root;
        // 旋转只改变这两个节点的子树成员，其它节点的子树没变，所以只更新它们俩
        // 顺序不能反，updateHeight的前提是孩子的高度是对的
        // p的高度依赖root的新高度，反过来会用到错误值
        updateHeight(root);
        updateHeight(p);
        return p;
    }
    // 参数root为bf失衡的节点；left/right互换就行
    Node* rotateRight(Node* root) {
        assert(root && root->left);
        Node* p = root->left;
        root->left = p->right;
        p->right = root;
        updateHeight(root);
        updateHeight(p);
        return p;
    }
    // 国内教材会把双旋单独拆成RotateLR/RotateRL，其实双旋就是"先转孩子、再转自己"
    Node* rotate(Node* node) {
        int bf = balanceFactor(node);
        if (bf > 1) {
            if (balanceFactor(node->right) >= 0)  // 直链，注意等于，见后
                return rotateLeft(node);
            else {  // 折线，先把右孩子转直
                node->right = rotateRight(node->right);
                return rotateLeft(node);
            }
        }
        else if (bf < -1) {
            if (balanceFactor(node->left) <= 0)
                return rotateRight(node);
            else {
                node->left = rotateLeft(node->left);
                return rotateRight(node);
            }
        }
        else
            return node;
    }

    // 和BST完全一样
    bool _contain(const Node* node, int val) const {
        if (node == nullptr) return false;
        if (node->data == val) return true;
        if (val < node->data) return _contain(node->left, val);
        return _contain(node->right, val);
    }
    // 类似LLRB，只在结尾部分有所变动，调整高度
    Node* _insert(Node* node, int val) {
        if (node == nullptr) return new Node(val);

        if (val < node->data)
            node->left = _insert(node->left, val);
        else if (val > node->data)
            node->right = _insert(node->right, val);
        else
            return node;  // 不重复插入
        // 完成插入之后调整高度
        updateHeight(node);
        node = rotate(node);
        return node;
    }
    // 删除也是同样的，在结尾进行高度调整，这里略去

public:
    bool contain(int val) const { return _contain(root, val); }
    void insert(int val) { root = _insert(root, val); }
};

/*
等于：

- 在插入时，等于的情况是不会出现的，可以自己画图反证法试试
失衡节点的高的那边，必然就是刚插入的那条路径，孩子刚长高1却仍然平衡，说明孩子的高度变为1/-1
- 等于的情况出现在删除的时候：()为高度
删除B的时候，hA变为2，hC还是0
这时候对A左旋一次就可以得到平衡，不需要按照折线的情况双旋转

    A(1)                         C(-1)
   /   \                        /    \
  B    C(0)      删除B并左旋A   A(1)  E
      / \                        \
     D   E                        D
*/