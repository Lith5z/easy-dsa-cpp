/*
插入排序：直接插入排序（原地）

对于N小的数组，直接插入排序是最快的
直接插入排序的交换次数就是逆序对的次数，所以大部分有序的数组插入排序速度很快

这个直接插入排序，用的是temp回填的方法
对比可以参考堆的sift和siftOp
*/

#include <iostream>
#include <vector>

// time best O(n); worst O(n^2)
// space O(1)
// stable
void insertSort(std::vector<int>& arr) {
    int size = arr.size();
    // 分为 已排序|未排序
    // 所以 [0]|其他 只有一个元素默认有序，故i=1开始
    for (int i = 1; i < size; i++) {
        int temp = arr[i];
        int j = i;

        // 后移留空
        while (j > 0 && arr[j - 1] > temp) {  // 短路求值确保j-1不越界
            arr[j] = arr[j - 1];
            j--;
        }
        arr[j] = temp;
    }
}

int main() {
    using namespace std;
    vector<int> original = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};

    vector<int> arr = original;
    insertSort(arr);
    cout << "Insert sort:\n";
    for (int i : arr) cout << i << ' ';
    cout << '\n';
}