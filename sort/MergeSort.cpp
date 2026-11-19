/*
归并排序

把数组不断二分，等两个区间各自有序后再合并
aux是合并用的辅助数组，只在主函数mergeSort里分配一次
如果放进递归里，每次合并都要分配并清零一个O(n)的数组，总时间会退化到O(n^2)
*/

#include <iostream>
#include <vector>

void merge(std::vector<int>& arr, std::vector<int>& aux, int left, int mid,
           int right);  // 前置，让后面阅读可以按照顺序来
void mergeSortHelper(std::vector<int>& arr, std::vector<int>& aux, int left,
                     int right);

// time O(n log n)
// space O(n) aux数组；递归栈是O(log n)
// stable
void mergeSort(std::vector<int>& arr) {
    std::vector<int> aux(arr.size());
    mergeSortHelper(arr, aux, 0, (int)arr.size() - 1);
}

void mergeSortHelper(std::vector<int>& arr, std::vector<int>& aux, int left,
                     int right) {
    if (left >= right) return;

    int mid = (left + right) / 2;
    mergeSortHelper(arr, aux, left, mid);
    mergeSortHelper(arr, aux, mid + 1, right);
    // 此时[left..mid]和[mid+1..right]都已经排序，只需要合并为一个数组
    merge(arr, aux, left, mid, right);
}
// 合并arr的[left..mid]和[mid+1..right]两部分
void merge(std::vector<int>& arr, std::vector<int>& aux, int left, int mid,
           int right) {
    for (int i = left; i <= right; i++) aux[i] = arr[i];
    // 剩下的就是双指针合并两个有序数组
    int i = left, j = mid + 1;
    int k = left;
    while (i <= mid && j <= right) {
        if (aux[i] <= aux[j])  // <=保持稳定性
            arr[k++] = aux[i++];
        else
            arr[k++] = aux[j++];
    }
    while (i <= mid) arr[k++] = aux[i++];
    while (j <= right) arr[k++] = aux[j++];
}

int main() {
    std::vector<int> arr = {5, 1, 4, 2, 8, 5, 3};
    mergeSort(arr);

    std::cout << "Merge sort:\n";
    for (int value : arr) std::cout << value << ' ';
    std::cout << '\n';
}
