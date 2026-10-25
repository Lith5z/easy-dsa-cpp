/*
B-tree
代码由AI生成，B-tree的实现太麻烦，不如红黑树，所以代码部分可以略去

在BST中提到，顺序插入数据树会变得很稀疏；而b-tree对于任意顺序都保持O(logn)高度

B-tree的每个节点可以容纳至多L个元素，至多有L+1个子节点
    [15,17]
   /   |   |
[14] [16] [18,19]
分支是大小的氛围：14 < 15; 15 < 16 < 17; 17 < 18,19

超出L时，将中间（偶数取左边）位置的元素移动到父节点，剩余部分作为子树连接到父节点
如L=3，[15]                 [15,17]
      /   |       --->     /   |   |
    [14]  [16,17,18,19]  [14] [16] [18,19]
如果是root超出L，则中间元素会作为新的root

除了L，B-tree还有一个参数t，为非根的最小度数（节点的最小子树数量）
对于标准B-tree，2t=L+1，这意味着一个非根节点有[t,L]个子节点
Because of the way B-Trees are constructed, we get two nice invariants:
1. All leaves must be the same distance from the source
   因为只有root超出L才会增加树的高度，而对于所有节点都+1
2. A non-leaf node with k items must have exactly k+1 children
   k个元素分出k+1个范围
*/

#include <cassert>
#include <iostream>
#include <vector>

struct BTreeNode {
    bool leaf;
    std::vector<int> keys;  // 存储的元素
    std::vector<BTreeNode*> children;

    explicit BTreeNode(bool is_leaf) : leaf(is_leaf) {}
};
class BTree {
private:
    BTreeNode* root;
    int t;  // 非根节点的最小度数；标准B树一个节点最多有L=2t-1个元素

    void destroy(BTreeNode* node) {
        if (node == nullptr) return;
        for (BTreeNode* child : node->children) destroy(child);
        delete node;
    }

    // 从node作root的子树开始寻找
    bool _contain(const BTreeNode* node, int val) const {
        int index = 0;
        int key_count = static_cast<int>(node->keys.size());
        // 在node节点的keys中寻找，keys有序
        while (index < key_count && node->keys[index] < val) ++index;

        if (index < key_count && node->keys[index] == val) return true;
        if (node->leaf) return false;
        return _contain(node->children[index], val);
    }
    // 将parent的第index个满子节点分裂，并把中间元素移到parent
    void splitChild(BTreeNode* parent, int index) {
        BTreeNode* full = parent->children[index];
        BTreeNode* right = new BTreeNode(full->leaf);
        int middle = full->keys[t - 1];

        int key_count = static_cast<int>(full->keys.size());
        for (int i = t; i < key_count; ++i) {
            right->keys.push_back(full->keys[i]);
        }
        full->keys.resize(t - 1);

        if (!full->leaf) {
            int child_count = static_cast<int>(full->children.size());
            for (int i = t; i < child_count; ++i) {
                right->children.push_back(full->children[i]);
            }
            full->children.resize(t);
        }

        parent->keys.push_back(0);
        for (int i = parent->keys.size() - 1; i > index; --i) {
            parent->keys[i] = parent->keys[i - 1];
        }
        parent->keys[index] = middle;

        parent->children.push_back(nullptr);
        for (int i = parent->children.size() - 1; i > index + 1; --i) {
            parent->children[i] = parent->children[i - 1];
        }
        parent->children[index + 1] = right;
    }
    // node一定不是满节点
    void insertNonFull(BTreeNode* node, int val) {
        int index = 0;
        int key_count = static_cast<int>(node->keys.size());
        while (index < key_count && node->keys[index] < val) ++index;

        if (node->leaf) {
            node->keys.push_back(0);
            for (int i = node->keys.size() - 1; i > index; --i) {
                node->keys[i] = node->keys[i - 1];
            }
            node->keys[index] = val;
            return;
        }

        if (node->children[index]->keys.size() ==
            static_cast<size_t>(2 * t - 1)) {
            splitChild(node, index);
            if (val > node->keys[index]) ++index;
        }
        insertNonFull(node->children[index], val);
    }

public:
    explicit BTree(int min_degree = 2) : root(nullptr), t(min_degree) {
        assert(t >= 2);
    }
    ~BTree() { destroy(root); }

    bool contain(int val) const {
        return root != nullptr && _contain(root, val);
    }
    void insert(int val) {
        if (contain(val)) return;

        if (root == nullptr) {
            root = new BTreeNode(true);
            root->keys.push_back(val);
            return;
        }

        if (root->keys.size() == static_cast<size_t>(2 * t - 1)) {
            BTreeNode* new_root = new BTreeNode(false);
            new_root->children.push_back(root);
            splitChild(new_root, 0);
            root = new_root;
        }
        insertNonFull(root, val);
    }
};

int main() {
    BTree btree;
    for (int value : {4, 2, 6, 1, 3, 5, 7}) {
        btree.insert(value);
    }

    assert(btree.contain(1));
    assert(btree.contain(7));
    assert(!btree.contain(8));
    std::cout << "B-tree insert and contain passed\n";
}
