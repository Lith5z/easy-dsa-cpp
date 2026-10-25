// an array and list based stack

#include "Stack.h"

#include <cassert>
#include <iostream>

using namespace std;

// top point to the current top element
// e.g. top = -1 -> empty, push to the [top+1]
class ArrayStack : public Stack {
private:
    int top;
    int capacity;
    int* data;

    void resize(int new_size) {
        int* new_data = new int[new_size];
        memcpy(new_data, data, (top + 1) * sizeof(int));
        delete data;
        data = new_data;
        capacity = new_size;
    }

public:
    ArrayStack(int size) : top(-1) {
        assert(size > 0);
        capacity = size;
        data = new int[size];
    }
    ~ArrayStack() { delete data; }

    bool isEmpty() const override { return top == -1; }
    bool isFull() const override { return top == capacity - 1; }
    int getTop() const override {
        assert(!isEmpty());
        return data[top];
    }
    int getSize() const override { return top + 1; }
    void push(int val) override {
        if (isFull()) {
            resize(capacity * 2);  // 翻倍，避免频繁扩容
        }
        data[++top] = val;
    }
    void pop() override {
        assert(!isEmpty());
        top--;
    }
};

struct Node {
    int val;
    Node* next;

    Node() : val(0), next(nullptr) {}
    Node(int n, Node* nxt) : val(n), next(nxt) {}
};
// 这里重写了很多链表的内容，其实直接用SLList.cpp就行
class ListStack : public Stack {
private:
    Node* top;

    ListStack() : top(nullptr) {}
    ~ListStack() {
        Node *p = top, *nxt;
        while (p != nullptr) {
            nxt = p->next;
            delete p;
            p = nxt;
        }
    }

public:
    bool isEmpty() const override { return top == nullptr; }
    bool isFull() const override { return false; }
    int getTop() const override {
        assert(!isEmpty());
        return top->val;
    }
    int getSize() const override {
        int size = 0;
        for (Node* p = top; p != nullptr; p = p->next) {
            size++;
        }
        return size;
    }
    void push(int val) override {
        Node* add = new Node(val, top);
        top = add;
    }
    void pop() override {
        assert(!isEmpty());
        Node* p = top;
        top = top->next;
        delete p;
    }
};