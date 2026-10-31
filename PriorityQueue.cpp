/*
优先队列PQ（默认最小元素在队首）
线性数组实现
push 后移已有项直到找到正确位置；pop 全部前移

二叉堆实现
可以使用BST实现优先队列，只要维护一个指针一直指向最小的元素即可
这样的问题是：
- BST里是没有重复元素的，而PQ可以有。如果要添加一个计数器，BST实现会比较麻烦
- BST的拷贝比较麻烦

与其使用BST，不如实现一个每个节点都比子节点要小的二叉树
如果这个二叉树是完全的complete，就可以通过数组模拟，也就是二叉堆

注意二叉树和数组的下标对应关系，给定数组下标[i]
父节点在[(i-1)/2]，左子节点是[2i+1]
*/

#include <cassert>
#include <cstring>
#include <utility>

#include "More/Queue.h"

using namespace std;

// always keep min at head/front
// push/pop 最坏O(n)
class ArrayPriorityQueue : public Queue {
private:
    int* data;
    int size;
    int capacity;

public:
    ArrayPriorityQueue(int siz) : size(0), capacity(siz) {
        data = new int[siz];
    }
    ~ArrayPriorityQueue() { delete[] data; }

    bool isEmpty() const { return size == 0; }
    bool isFull() const { return size == capacity; }
    int getFront() const override {
        assert(!isEmpty());
        return data[0];
    }
    void push(int x) override {
        assert(!isFull());
        int index = size;  // 一开始将要放置的位置
        while (index > 0 && data[index - 1] > x) {
            data[index] = data[index - 1];
            index--;
            // [index]仍为空
        }
        data[index] = x;
        size++;
    }
    void pop() override {
        assert(!isEmpty());
        // 所有元素前移，[0]直接被覆盖
        for (int i = 1; i < size; i++) {
            data[i - 1] = data[i];
        }
        size--;
    }
};

// push/pop 最坏O(log n)
class HeapPriorityQueue : public Queue {
private:
    int* data;
    int size;
    int capacity;

    // 对于根节点下标是[start]的子树，如果根比子节点更大，就不断往下交换
    // 结果是大的节点在树中下移，故称为down
    // O(h) = O(log n)，最坏情况路径长度不超过树高
    void siftDown(int start) {
        int i = start, j = 2 * i + 1;  // j是i的左子女
        while (j < size) {
            if (j + 1 < size && data[j] > data[j + 1]) j++;
            // 如果有两个子女，让j指向其中更小的
            if (data[i] <= data[j])
                break;  // 符合小的在上
            else {
                std::swap(data[i], data[j]);
                // 继续沿着交换后的位置往下检查
                i = j;
                j = 2 * j + 1;
            }
        }
    }
    // 从下标为[start]的节点开始，如果子节点比父节点小，就不断往上交换
    // 结果是小的节点在树中上移，故称为up
    // O(h) = O(log n)
    void siftUp(int start) {
        int i = start, j = (i - 1) / 2;  // j是i的父节点
        while (i > 0) {
            if (data[i] >= data[j])
                break;  // 符合
            else {
                std::swap(data[i], data[j]);
                // 继续沿着交换后的位置往上检查
                i = j;
                j = (j - 1) / 2;
            }
        }
    }

    // 事实上上面的写法不是最优的，下面以siftDown为例
    void siftDownOp(int start) {
        int i = start, j = 2 * i + 1;  // j是i的左子女
        int temp = data[i];            // 取出将要向下的元素
        while (j < size) {
            if (j + 1 < size && data[j] > data[j + 1]) j++;
            if (temp <= data[j])
                break;
            else {
                data[i] = data[j];  // 小的子女向上，此时data[j]其实是留空的
                i = j;
                j = 2 * j + 1;
            }
        }
        data[i] = temp;
        // 最后回填，相比std::swap常数更小
    }

public:
    HeapPriorityQueue() : size(0), capacity(10), data(nullptr) {}
    // 使用siftDown建堆 O(n)，使用siftUp O(n logn)
    HeapPriorityQueue(int* arr, int arr_size)
        : size(arr_size), capacity(arr_size * 2) {
        data = new int[arr_size * 2];
        memcpy(data, arr, arr_size * sizeof(int));
        // 开始调整堆的大小关系
        for (int pos = (size - 2) / 2; pos >= 0; pos--) {
            // size-1才是最后一个节点的index，取出最后一个有子女的节点
            siftDown(pos);
        }
    }
    ~HeapPriorityQueue() { delete[] data; }

    // O(log n)
    void push(int x) override {
        assert(!isFull());
        data[size] = x;
        // push前堆的大小关系是正确的，只有[size]可能不对
        siftUp(size);  // 时间复杂度来自
        size++;
    }
    // O(log n)
    void pop() override {
        assert(!isEmpty());
        std::swap(data[0], data[size - 1]);
        size--;
        // pop后只有top是错误的，从头开始下放data[0]
        siftDown(0);
    }
    int getFront() const override {
        assert(!isEmpty());
        return data[0];
    }

    bool isFull() const override { return size == capacity; }
    bool isEmpty() const override { return size == 0; }
};

/*
siftUp建立堆
for (int i = 1; i < size; i++) {
    siftUp(i);
}

siftDown建立堆
for (int pos = (size - 2) / 2; pos >= 0; pos--) {
    siftDown(pos);
}

外层循环的量级都是O(n)，主要是sift操作影响复杂度

使用siftUp，把节点往上交换，节点的最多交换次数是这个节点深度depth；
使用siftDown，把节点往下交换，最多次数是这个节点的高度height
而二叉堆模拟的是完全二叉树，完全二叉树中大部分节点都在树的底部，深度大而高度小
所以使用siftDown建堆，总的期望交换次数更小，严格证明需要用到二叉树的数学性质，这里略去
*/