// array and list based queue

// 基于数组实现，需要用头尾两个指针处理头部删除、尾部插入
// 最简单的方法是线性的，但一旦头部删除了元素即front++，数组开头的空间就浪费了
// 故更好的方法是循环数组队列，允许tail在抵达数组结尾的时候回到数组开头
// 这样，基本操作的时间复杂度都是O(1)，但是空间复杂度是O(n)

// 第二类的实现是基于链表，但是如果只使用一个head指针，访问另一端开销是O(n)
// 因而需要持有头尾两个节点的指针

// 国内的教材里一般用rear代替tail，而且指向结尾元素的下一位
// 我这里指向结尾元素本身，双端队列的头尾操作就统一了

#include "Queue.h"

#include <cassert>
#include <iostream>

using namespace std;

// e.g. [] [(front)2] [3] [6] [(tail)8] [next push] []
class LinearArrayQueue : public Queue {
private:
    int* data;
    int front;
    int tail;
    int capacity;

public:
    LinearArrayQueue(int size) : front(-1), tail(-1), capacity(size) {
        assert(size > 0);
        data = new int[size];
    }
    ~LinearArrayQueue() { delete[] data; }

    bool isEmpty() const override {
        return front == -1 && tail == -1;
        // front == tail : one element, not empty
    }
    bool isFull() const override { return tail == capacity - 1; }
    int getFront() const override {
        assert(!isEmpty());
        return data[front];
    }
    void push(int x) override {
        assert(!isFull());
        if (isEmpty()) front = 0;
        data[++tail] = x;
    }
    void pop() override {
        assert(!isEmpty());
        if (front == tail) {
            // only one element, pop = set empty
            front = tail = -1;
        }
        else
            front++;
    }
};

// current pos = i, next pos = i+1 if linear
// in circle, next pos = (i+1) % n
// replace all `i+1` to `i = (i+1) % n`
// size = (tail-front+1+n) % n
// 遇到减法，+n以确保不为负数
class CircularArrayQueue : public Queue {
private:
    int* data;
    int front;
    int tail;
    int capacity;

public:
    CircularArrayQueue(int size) : front(-1), tail(-1), capacity(size) {
        assert(size > 0);
        data = new int[size];
    }
    ~CircularArrayQueue() { delete[] data; }

    bool isEmpty() const override { return front == -1 && tail == -1; }
    bool isFull() const override {
        // 由于tail可以回到数组开头，不能用和capacity比较判断
        // if next tail pos = front -> full
        return (tail + 1) % capacity == front;
    }
    int getFront() const override {
        assert(!isEmpty());
        return data[front];
    }
    void push(int x) override {
        assert(!isFull());
        if (isEmpty()) front = 0;
        tail = (tail + 1) % capacity;  // tail move to next pos
        data[tail] = x;
    }
    void pop() override {
        assert(!isEmpty());
        if (front == tail)
            front = tail = -1;
        else
            front = (front + 1) % capacity;
    }
};

struct Node {
    int val;
    Node* next;

    Node() : val(0), next(nullptr) {}
    Node(int n, Node* nxt) : val(n), next(nxt) {}
};
class ListQueue : public Queue {
private:
    Node* front;
    Node* tail;

public:
    ListQueue() : front(nullptr), tail(nullptr) {}
    ~ListQueue() {
        Node* p = front;
        Node* temp;
        while (p != nullptr) {
            temp = p;
            p = p->next;
            delete temp;
        }
    }

    bool isEmpty() const override {
        return front == nullptr && tail == nullptr;
    }
    bool isFull() const override { return false; }  // 没有容量问题
    int getFront() const override {
        assert(front != nullptr);
        return front->val;
    }
    void push(int x) override {
        Node* add = new Node(x, nullptr);
        if (isEmpty()) {
            front = tail = add;
        }
        else {
            tail->next = add;
            tail = add;
        }
    }
    void pop() override {
        Node* temp = front;
        assert(!isEmpty());
        if (front == tail) {
            delete front;
            front = tail = nullptr;
        }
        else {
            front = front->next;
            delete temp;
        }
    }
};

void testQueue(Queue& queue) {
    assert(queue.isEmpty());
    assert(!queue.isFull());

    queue.push(10);
    queue.push(20);
    assert(!queue.isEmpty());
    assert(!queue.isFull());
    assert(queue.getFront() == 10);

    queue.pop();
    assert(queue.getFront() == 20);
    queue.pop();
    assert(queue.isEmpty());
    assert(!queue.isFull());

    queue.push(40);
    assert(!queue.isEmpty());
    assert(queue.getFront() == 40);
    queue.pop();
    assert(queue.isEmpty());
}

int main() {
    LinearArrayQueue linearQueue(3);
    CircularArrayQueue circularQueue(3);
    ListQueue listQueue;

    testQueue(linearQueue);
    testQueue(circularQueue);
    testQueue(listQueue);

    cout << "All queue tests passed.\n";
    return 0;
}