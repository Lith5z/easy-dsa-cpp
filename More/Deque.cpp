// 双端队列

// 基于循环数组实现
// 对于front和tail的操作基本是高度对称的

#include <cassert>
#include <iostream>

using namespace std;

class Deque {
private:
    int* data;
    int front, tail;
    int size;
    int capacity;

public:
    Deque(int size) : front(-1), tail(-1), size(0), capacity(size) {
        assert(size > 0);
        data = new int[size];
    }
    ~Deque() { delete[] data; }

    bool isEmpty() const { return size == 0; }
    bool isFull() const { return size == capacity; }
    int getFront() const {
        assert(!isEmpty());
        return data[front];
    }
    int getTail() const {
        assert(!isEmpty());
        return data[tail];
    }

    void pushFront(int x) {
        // front前移，补上capacity以避免负数，正数同余忽略
        assert(!isFull());
        if (isEmpty())
            front = tail = 0;
        else
            front = (front - 1 + capacity) % capacity;
        data[front] = x;
        size++;
    }
    void pushTail(int x) {
        // tail后移，同CircularArrayQueue
        assert(!isFull());
        if (isEmpty())
            front = tail = 0;
        else
            tail = (tail + 1) % capacity;
        data[tail] = x;
        size++;
    }
    void popFront() {
        // front后移
        assert(!isEmpty());
        if (size == 1)
            front = tail = -1;
        else
            front = (front + 1) % capacity;
        size--;
    }
    void popTail() {
        // tail前移
        assert(!isEmpty());
        if (size == 1)
            front = tail = -1;
        else
            tail = (tail - 1 + capacity) % capacity;
        size--;
    }
};

int main() {
    {
        Deque deque(4);

        assert(deque.isEmpty());
        assert(!deque.isFull());

        deque.pushTail(20);
        deque.pushTail(30);
        deque.pushFront(10);
        deque.pushFront(0);

        assert(deque.getFront() == 0);
        assert(deque.getTail() == 30);
        assert(deque.isFull());

        deque.popFront();
        assert(deque.getFront() == 10);
        deque.popTail();
        assert(deque.getTail() == 20);
        deque.popFront();
        deque.popTail();

        assert(deque.isEmpty());
    }

    {
        // 测试尾部和头部经过数组边界后的循环使用
        Deque deque(4);

        deque.pushTail(1);
        deque.pushTail(2);
        deque.pushTail(3);
        deque.pushTail(4);
        deque.popFront();
        deque.popFront();
        deque.pushTail(5);
        deque.pushTail(6);

        assert(deque.isFull());
        assert(deque.getFront() == 3);
        assert(deque.getTail() == 6);

        deque.popTail();
        assert(deque.getTail() == 5);
        deque.popFront();
        assert(deque.getFront() == 4);
    }

    {
        Deque deque(1);

        deque.pushFront(42);
        assert(deque.isFull());
        assert(deque.getFront() == 42);
        assert(deque.getTail() == 42);

        deque.popTail();
        assert(deque.isEmpty());
    }

    cout << "All deque tests passed.\n";
    return 0;
}