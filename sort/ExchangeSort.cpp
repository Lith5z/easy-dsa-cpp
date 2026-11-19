/*
交换排序：冒泡排序和快速排序
这里把quickSort作为递归函数，调用时候麻烦些但是代码更短

快速排序等效于利用BST进行排序

时间上，使用left为基准，n^2的最坏情况出现在接近有序的数组
空间上，快排的递归栈开销比归并排序更小，常数也更小
所以一般快排就是最快的排序方式，平均情况稳定O(n log n)

快速排序的分组方法可以用来求无序数组的中位数：
选定一个基准，进行分组之后，看这个基准的位置，如果在左半边就只对右半边进行分组
不断这样操作，直到基准被放在中位数的位置
复杂度是O(n)
*/

#include <iostream>
#include <utility>
#include <vector>

// time best O(n); worst O(n^2)
// space O(1)
// stable
void bubbleSort(std::vector<int>& arr) {
    int size = arr.size();
    bool hasSwap;
    for (int i = 0; i < size - 1; i++) {
        hasSwap = false;
        for (int j = 0; j < size - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
                hasSwap = true;
            }
        }
        if (!hasSwap) break;
    }
}

// 划分好基准数两侧使得 左<=基准数<=右
int partition(std::vector<int>& arr, int left, int right) {
    // 以 arr[left] 为基准数
    int i = left, j = right;
    while (i < j) {
        while (i < j && arr[j] >= arr[left])
            j--;  // 从右向左找首个小于基准数的元素
        while (i < j && arr[i] <= arr[left])
            i++;  // 从左向右找首个大于基准数的元素
        // 交换这两个元素
        std::swap(arr[i], arr[j]);
    }
    std::swap(arr[i], arr[left]);  // 将基准数交换至两子数组的分界线
    return i;
}
void quickSort(std::vector<int>& arr, int left, int right) {
    if (left >= right) return;

    int pivot = partition(arr, left, right);
    quickSort(arr, left, pivot - 1);
    quickSort(arr, pivot + 1, right);
}

int main() {
    using namespace std;
    vector<int> original = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};

    vector<int> arr = original;
    bubbleSort(arr);
    cout << "Bubble sort:\n";
    for (int i : arr) cout << i << ' ';
    cout << '\n';

    arr = original;
    quickSort(arr, 0, (int)arr.size() - 1);
    cout << "Quick sort:\n";
    for (int i : arr) cout << i << ' ';
    cout << '\n';
}