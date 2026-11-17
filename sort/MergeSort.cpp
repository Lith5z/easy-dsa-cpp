/*
归并排序

把数组不断递归分成两个有序区间，再合并
这里把mergeSort作为递归函数，调用时候麻烦些但是代码更短
*/

#include <iostream>
#include <vector>

void merge(std::vector<int>& arr, std::vector<int>& aux, int left, int mid,
           int right);  // 前置，让后面阅读可以按照顺序来

// time O(n log n)
// space O(n)
// stable
void mergeSort(std::vector<int>& arr, int left, int right) {
    if (left >= right) return;

    int mid = (left + right) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    // 此时[left..mid]和[mid+1..right]都已经排序，只需要合并为一个数组
    std::vector<int> aux(arr.size());
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
    mergeSort(arr, 0, (int)arr.size() - 1);

    std::cout << "Merge sort:\n";
    for (int value : arr) std::cout << value << ' ';
    std::cout << '\n';
}
