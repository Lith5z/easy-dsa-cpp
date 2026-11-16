/* an array and list based stack */

#include "Stack.h"

#include <cassert>
#include <cstring>
#include <iostream>

// top point to the current top element
// e.g. top = -1 -> empty, push to the [top+1]
class ArrayStack : public Stack {
private:
    int top;
    int capacity;
    int* data;

    void resize(int newSize) {
        int* newData = new int[newSize];
        memcpy(newData, data, (top + 1) * sizeof(int));
        delete[] data;
        data = newData;
        capacity = newSize;
    }

public:
    ArrayStack(int size) : top(-1), capacity(size), data(new int[size]) {
        assert(size > 0);
    }
    ~ArrayStack() { delete[] data; }

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

public:
    ListStack() : top(nullptr) {}
    ~ListStack() {
        Node *p = top, *nxt;
        while (p != nullptr) {
            nxt = p->next;
            delete p;
            p = nxt;
        }
    }

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