// a doubly linked list

// 双向链表的最佳实现，表头有一个哨兵节点，整个链表形成一个循环
// 空表时，哨兵的prev和next都指向自身
// 这样的好处是，避免了对空表情况的单独讨论，而且对立功能的代码也类似（比如addFirst和addLast）

#include <cassert>
#include <iostream>

struct Node {
    int val;
    Node* next;
    Node* prev;

    Node() : val(0), next(nullptr), prev(nullptr) {}
    Node(int n, Node* nxt, Node* prv) : val(n), next(nxt), prev(prv) {}
};

class DLList {
private:
    int size;
    Node* sentinel;

public:
    DLList() : size(0) {
        sentinel = new Node();
        sentinel->next = sentinel;
        sentinel->prev = sentinel;
    }
    DLList(int first_val) : size(1) {
        sentinel = new Node();
        Node* first = new Node(first_val, sentinel, sentinel);
        sentinel->next = first;
        sentinel->prev = first;
    }
    ~DLList() {
        // 双指针析构
        Node *p = sentinel->next, *nxt;
        while (p != sentinel) {
            nxt = p->next;
            delete p;
            p = nxt;
        }
        delete sentinel;
    }
    // 禁止拷贝：链表拥有节点资源，浅拷贝会导致 double free
    DLList(const DLList&) = delete;
    DLList& operator=(const DLList&) = delete;
    void addFirst(int val) {
        Node* p = sentinel->next;
        Node* add = new Node(val, p, sentinel);
        p->prev = add;
        sentinel->next = add;
        size++;
    }
    void addLast(int val) {
        Node* p = sentinel->prev;
        Node* add = new Node(val, sentinel, p);
        p->next = add;
        sentinel->prev = add;
        size++;
    }
    void removeFirst() {
        assert(!isEmpty());
        Node* p = sentinel->next;
        sentinel->next = p->next;
        p->next->prev = sentinel;
        delete p;
        size--;
    }
    void removeLast() {
        assert(!isEmpty());
        Node* p = sentinel->prev;
        sentinel->prev = p->prev;
        p->prev->next = sentinel;
        delete p;
        size--;
    }
    bool contain(int val) const {
        Node* p = sentinel->next;
        while (p != sentinel) {
            if (p->val == val) return true;
            p = p->next;
        }
        return false;
    }

    void print() const {
        Node* p = sentinel->next;  // 跳过哨兵节点
        int cnt = 0;
        while (p != sentinel) {
            std::cout << cnt << ": " << p->val << '\n';
            p = p->next;
            cnt++;
        }
    }
    void printReverse() const {
        Node* p = sentinel->prev;
        int cnt = size - 1;
        while (p != sentinel) {
            std::cout << cnt << ": " << p->val << '\n';
            p = p->prev;
            cnt--;
        }
    }
    int getFirst() const {
        return sentinel->next->val;  // 空表返回哨兵自己的值
    }
    int getLast() const { return sentinel->prev->val; }
    int getSize() const { return size; }
    bool isEmpty() const { return sentinel->next == sentinel; }
};

int main() {
    {
        DLList list;
        list.addLast(10);
        list.addLast(20);
        list.addLast(30);

        list.removeFirst();
        assert(list.getSize() == 2);
        assert(list.getFirst() == 20);
        assert(list.getLast() == 30);

        list.removeLast();
        assert(list.getSize() == 1);
        assert(list.getFirst() == 20);
        assert(list.getLast() == 20);
    }

    {
        DLList list;
        list.addFirst(7);
        list.removeFirst();
        assert(list.isEmpty());
        assert(list.getSize() == 0);
    }

    {
        DLList list(42);
        list.addFirst(10);
        list.addLast(20);

        list.removeFirst();
        assert(list.getSize() == 2);
        assert(list.getFirst() == 42);
        assert(list.getLast() == 20);

        list.removeLast();
        assert(list.getSize() == 1);
        assert(list.getFirst() == 42);
        assert(list.getLast() == 42);
    }

    std::cout << "All DLList remove tests passed." << '\n';
    return 0;
}