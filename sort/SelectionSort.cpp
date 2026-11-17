/*
选择排序：直接选择排序、堆排序

直接选择排序低效的地方是，每次找最小值都是一个个遍历找，下次找没有用到之前的信息
因而如果把数组存储在堆里，可以高效找到最小值，即堆排序

堆排序可以写成原地算法，即使用给定的数组建堆
这里直接用stl的heap相关方法实现，我自己的实现在 PriorityQueue.cpp
*/

#include <algorithm>
#include <functional>
#include <iostream>
#include <queue>
#include <vector>

// time O(n^2)
// space O(1)
// unstable
void selectionSort(std::vector<int>& arr) {
    int size = arr.size();
    // 最后一次在[size-1]剩下一个最大的元素，不需要排序
    for (int i = 0; i < size - 1; i++) {
        int minIndex = i;
        // 从i+1开始：arr[i]就是刚刚的min，不需要比
        for (int j = i + 1; j < size; j++) {
            if (arr[j] < arr[minIndex]) minIndex = j;
        }
        std::swap(arr[i], arr[minIndex]);
    }
}

// time O(n log n) 建堆+逐个删除
// space O(n)
// unstable
void heapSort(std::vector<int>& arr) {
    int size = arr.size();
    std::priority_queue<int> heap(arr.begin(), arr.end());
    // 默认最大堆，从后往前填充，[0...size-1]都要填充
    for (int i = size - 1; i >= 0; i--) {
        arr[i] = heap.top();
        heap.pop();
    }
}

// time O(n log n) 建堆+逐个删除
// space O(1) 原地算法
// unstable
void heapSortInPlace(std::vector<int>& arr) {
    int size = arr.size();
    std::make_heap(arr.begin(), arr.end());
    // size-1：最后一次在[0]剩下一个最小的元素，不需要排序
    for (int i = 0; i < size - 1; i++) {
        std::pop_heap(arr.begin(), arr.end() - i);
    }
}

/*
remarks
heapSort使用的是最大堆，然后后往前插入
也可以使用最小堆，然后前往后插入

原地算法也如此，复杂一些
基于最大堆的算法把arr拆成 堆区间 | 已排序区间
边界不断往前推进，堆只有底部最后一个元素受影响，做sift调整即可

对于最小堆，如果也按照最大堆操作，取出最小的元素top，应该放在arr开头
但是这样会完全破坏现有的堆，导致必须重新建堆，带来额外复杂度

所以最小堆要做对称操作，构成已排序区间 | 堆区间，其中堆顶在最右侧
需要用反向迭代器
void heapSortInPlace(std::vector<int>& arr) {
    // 通过反向迭代器，将 arr 的右侧作为堆顶
    // greater 构造最小堆
    std::make_heap(arr.rbegin(), arr.rend(), std::greater<int>());

    // 每次将最小值放到已排序区间的右侧
    for (auto last = arr.rend(); last != arr.rbegin(); --last) {
        std::pop_heap(arr.rbegin(), last, std::greater<int>());
    }
}
*/

int main() {
    using namespace std;
    vector<int> original = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};

    vector<int> arr = original;
    selectionSort(arr);
    cout << "Selection sort:\n";
    for (int i : arr) cout << i << ' ';
    cout << '\n';

    arr = original;
    heapSort(arr);
    cout << "Heap sort:\n";
    for (int i : arr) cout << i << ' ';
    cout << '\n';

    arr = original;
    heapSortInPlace(arr);
    cout << "Heap sort in-place:\n";
    for (int i : arr) cout << i << ' ';
    cout << '\n';
}