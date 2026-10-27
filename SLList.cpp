// a singly linked list

// 单链表在add/remove时要分别处理空表与非空的情况，因为当列表为空时，访问first->next是非法的
// 下面用的是表头带哨兵节点sentinel的方法，即使是空表
// 插入时：sentinel也指向哨兵，保证sentinel->next合法
// 删除时：保证前驱指针prev永远非空

#include <iostream>

struct Node {
    int val;
    Node* next;

    Node() : val(0), next(nullptr) {}
    Node(int n, Node* nxt) : val(n), next(nxt) {}
};

class SLList {
private:
    Node* sentinel;
    int size;

    // 给递归反转使用的私有方法
    // 返回翻转结果的新head
    Node* reverse(Node* head) {
        if (head == nullptr || head->next == nullptr) return head;
        Node* new_head = reverse(head->next);
        head->next->next = head;
        head->next = nullptr;  // 旧head是翻转后的tail
        return new_head;  // 翻转以head->next和head为开头的链表，新头都是原本的tail
    }

public:
    SLList(int first_val) : size(1) {
        sentinel = new Node();  // 表头哨兵节点
        sentinel->next = new Node(first_val, nullptr);
    }
    SLList() : size(0) {
        sentinel = new Node();  // 表头哨兵节点
    }
    ~SLList() {
        Node* p = sentinel->next;
        while (p != nullptr) {
            Node* nxt = p->next;
            delete p;
            p = nxt;
        }
        delete sentinel;
    }
    void print() const {
        Node* p = sentinel->next;  // 跳过哨兵节点
        int cnt = 0;
        while (p != nullptr) {
            std::cout << cnt << ": " << p->val << '\n';
            p = p->next;
            cnt++;
        }
    }
    void addFirst(int val) {
        sentinel->next = new Node(val, sentinel->next);
        size++;
    }
    void addLast(int val) {
        Node* p = sentinel;  // 从哨兵出发，空表时循环直接不执行
        while (p->next != nullptr) {
            p = p->next;
        }
        p->next = new Node(val, nullptr);
        size++;
    }
    bool contain(int val) const {
        Node* p = sentinel->next;
        while (p != nullptr) {
            if (p->val == val) return true;
            p = p->next;
        }
        return false;
    }
    void remove(int val) {
        Node *p = sentinel->next, *prev = sentinel;
        // always have: prev != nullptr
        while (p != nullptr) {
            if (p->val == val) break;
            prev = p;
            p = p->next;
        }
        if (p == nullptr) return;  // no val in list
        prev->next = p->next;
        delete p;
        size--;
    }

    // 原地翻转
    void reverse_iteration() {
        Node *cur = sentinel->next, *nxt = nullptr, *prev = nullptr;
        while (cur != nullptr) {
            nxt = cur->next;   // 保存下一个
            cur->next = prev;  // 反向链接
            prev = cur;        // 前进
            cur = nxt;
        }
        sentinel->next = prev;
    }
    // 原地翻转
    void reverse_recursion() { sentinel->next = reverse(sentinel->next); }
    int getFirst() const {
        return sentinel->next->val;  // 注意：空表时空指针解引用
    }
    int getSize() const { return size; }
};

int main() {
    SLList a = SLList(123);
    a.addLast(666);
    a.addLast(888);
    a.addLast(999);
    a.print();

    // return 0;
    a.reverse_recursion();
    a.print();
}