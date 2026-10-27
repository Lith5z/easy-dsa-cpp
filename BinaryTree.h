#pragma once

// 二叉树的通用部分，给BST复用

#include <algorithm>
#include <iostream>
#include <queue>
#include <stack>

struct Node {
    int data;
    Node* left;
    Node* right;

    Node() : data(0), left(nullptr), right(nullptr) {}
    Node(int val, Node* p_left, Node* p_right)
        : data(val), left(p_left), right(p_right) {}
};

class BinaryTree {
private:
    void destroy(Node* node) {
        if (node == nullptr) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

    void _BFprint(Node* node) const {
        if (node == nullptr) return;

        std::queue<Node*> frontier;
        frontier.push(node);
        while (!frontier.empty()) {
            // 下面三行的顺序不严格固定，只要从frontier里取出/访问就相当于删除了
            Node* p = frontier.front();
            std::cout << p->data << " ";
            frontier.pop();
            if (p->left != nullptr) frontier.push(p->left);
            if (p->right != nullptr) frontier.push(p->right);
        }
    }

    // preorder: root -> left -> right
    void _DFprint_recursion(Node* node) const {
        // 广度优先时，需要在层间遍历，但是层间并没有指针连接
        // 深度优先，遍历顺序符合树的递归性，递归实现很简单
        if (node == nullptr) return;
        std::cout << node->data << " ";
        _DFprint_recursion(node->left);
        _DFprint_recursion(node->right);
        // 中序和后序改上面三个的顺序就行
    }
    // preorder: root -> left -> right
    void _DFprint_preorder_iterative(Node* node) const {
        if (node == nullptr) return;

        std::stack<Node*> frontier;
        frontier.push(node);
        while (!frontier.empty()) {
            Node* p = frontier.top();
            // 访问root
            std::cout << p->data << " ";
            frontier.pop();
            // 由于栈是LIFO的，要前序遍历需要后进left，才能先访问left
            if (p->right != nullptr) frontier.push(p->right);
            if (p->left != nullptr) frontier.push(p->left);
        }
    }
    // inorder: left -> root -> right
    void _DFprint_inorder_iterative(Node* node) const {
        std::stack<Node*> frontier;
        Node* p = node;

        while (p != nullptr || !frontier.empty()) {
            // 先把当前节点及其所有左节点入栈
            while (p != nullptr) {
                frontier.push(p);
                p = p->left;
            }

            // 此时p == nullptr，取出栈顶，访问最左侧尚未访问的节点
            p = frontier.top();
            frontier.pop();
            std::cout << p->data << " ";

            // 转向右子树
            p = p->right;
        }
    }
    // postorder: left -> right -> root
    void _DFprint_postorder_iterative(Node* node) const {
        if (node == nullptr) return;

        std::stack<Node*> frontier;
        Node* p = node;
        Node* lastVisited = nullptr;

        while (p != nullptr || !frontier.empty()) {
            // 先沿左子树向下
            while (p != nullptr) {
                frontier.push(p);
                p = p->left;
            }

            Node* top = frontier.top();

            // 如果存在未访问的右子树，先访问右子树，继续外层循环
            if (top->right != nullptr && lastVisited != top->right) {
                p = top->right;
            }
            else {
                // 左右子树都访问完后，访问root
                std::cout << top->data << " ";
                lastVisited = top;
                frontier.pop();
            }
        }
    }

protected:
    Node* root;  // 允许在子类中访问

public:
    BinaryTree() : root(nullptr) {}
    virtual ~BinaryTree() { destroy(root); }

    // time: O(n)
    int getHeight(Node* node) const {
        if (node == nullptr) return -1;  // let height of leaf node = 0
        int leftHeight = getHeight(node->left);
        int rightHeight = getHeight(node->right);
        return std::max(leftHeight, rightHeight) + 1;
    }
    int getHeight() const { return getHeight(root); }

    // breadth first traversal print
    // time: O(n)
    // space: O(n) worst, O(1) best (list)
    void BFprint() const { _BFprint(root); }

    // depth first traversal print (recursion)
    // preorder: root -> left -> right
    // time: O(n)
    // space: O(h), where h is the tree height
    void DFprintRecursion() const { _DFprint_recursion(root); }

    // depth first traversal print (iterative)
    // preorder: root -> left -> right
    // time: O(n)
    // space: O(log n) avg, O(n) worst (list)
    void DFprintIterativePreorder() const { _DFprint_preorder_iterative(root); }
    // depth first traversal print (iterative)
    // inorder: left -> root -> right
    void DFprintIterativeInorder() const { _DFprint_inorder_iterative(root); }
    // depth first traversal print (iterative)
    // postorder: left -> right -> root
    void DFprintIterativePostorder() const {
        _DFprint_postorder_iterative(root);
    }
};