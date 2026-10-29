/*
binary search tree

背景：设想一个有序链表
head->[0]->[1]->[2]->[3]->[4]->[5]->[6]->[7]->[8]，我们知道查找是O(n)
如果把head指针移动到中间，指向[4]，同时0-3的指针反向，从而能从4的左右两侧访问，左侧比4小，右侧大
这样就降低到0.5n，如果我们不断做下去，0-3和5-8视作子链表，继续把head指针（来自[4]）指向中间元素
这相当于二分查找，所以查找操作降低到O(logn)，最坏情况（树大大偏离平衡，退化为链表）下是O(n)

可以证明，BST对随机的插入和删除操作，高度H都是O(log n)；
但是只要出现1 2 3 4 5这样顺序的插入，就会逐渐偏离平衡
解决的办法是使用B-tree

为什么要使用带node参数的私有方法实现递归：自己试试不用就知道了
1. insert/contain/remove这些作为类的方法，返回值是void/bool
2. 这些方法的参数列表，都是int val
而递归调用需要传递参数，往往是通过返回值和传入参数实现的
类的方法没有合适的返回值和参数列表，根本就没法写成递归
3. 另外，利用返回值可以更新父节点（调用者）的连接
二叉树的节点没有prev前驱指针，用这个方法可以更新
*/

#include <cassert>

// BinaryTree 的实现不在本文件里，它的顺序排在 BST 前面，建议先读 BinaryTree.h
// 再读这里
#include "BinaryTree.h"

// insert/contain/remove使用带node参数的私有方法，递归实现
// 三个方法都是O(log n), worst O(n)
class BST : public BinaryTree {
private:
    bool _contain(const Node* node, int val) const {
        // 两个base case
        if (node == nullptr) return false;
        if (node->data == val) return true;
        // 否则在左树或者右树查找
        if (val < node->data) return _contain(node->left, val);
        return _contain(node->right, val);
    }
    // 把val插入以node为根的子树，并返回插入完成后这棵子树的根节点
    Node* _insert(Node* node, int val) {
        // 返回新节点，给上一步的node->left/right赋值
        if (node == nullptr) return new Node(val, nullptr, nullptr);

        // 递归调用，可能更新当前的连接（如果node->left为空）
        if (val < node->data)
            node->left = _insert(node->left, val);
        else if (val > node->data)
            node->right = _insert(node->right, val);

        // val == node->data的时候，直接返回当前node*，不重复插入
        return node;
    }
    // 返回删除后的树的root，返回值仅用于递归更新调用者的连接
    Node* _remove(Node* root, int val) {
        if (root == nullptr) {
            // i.e. return nullptr
        }
        else if (val < root->data)
            // _remove返回值是删除后左子树的root，赋值给当前root
            root->left = _remove(root->left, val);
        else if (val > root->data)
            root->right = _remove(root->right, val);
        else {
            // 此时root即需要删除的节点，按子节点情况分类
            if (root->left == nullptr && root->right == nullptr) {  // leaf
                delete root;
                root = nullptr;  // 最后统一返回root
            }
            else if (root->left == nullptr) {
                Node* temp = root;
                root = root->right;
                // 删除后新子树的根是root->right，返回这个值
                delete temp;
            }
            else if (root->right == nullptr) {
                Node* temp = root;
                root = root->left;
                delete temp;
            }
            else {
                // 有两个子节点
                // 把此节点的值改成左子树的max或者右子树的min，BST仍成立
                // （这三个值是相邻的，左max < this < 右min）
                // 然后删掉原本的max/min节点即可
                Node* temp = getMinNode(root->right);
                root->data = temp->data;
                // 修改了root的右子树
                root->right = _remove(root->right, temp->data);
            }
        }
        return root;  // 所有情况都要返回给调用者（父节点）修改连接
    }
    Node* getMinNode(Node* start) const {
        assert(start != nullptr);
        Node* p = start;
        while (p->left != nullptr) {
            p = p->left;
        }
        return p;
    }
    Node* getMaxNode(Node* start) const {
        assert(start != nullptr);
        Node* p = start;
        while (p->right != nullptr) {
            p = p->right;
        }
        return p;
    }

public:
    bool contain(int val) const { return _contain(root, val); }
    void insert(int val) { root = _insert(root, val); }
    void remove(int val) { root = _remove(root, val); }

    int getMin() const { return getMinNode(root)->data; }
    int getMax() const { return getMaxNode(root)->data; }
};

int main() {
    BST bst;
    for (int value : {4, 2, 6, 1, 3, 5, 7}) {
        bst.insert(value);
    }

    bst.DFprintIterativeInorder();
    std::cout << std::endl;
}