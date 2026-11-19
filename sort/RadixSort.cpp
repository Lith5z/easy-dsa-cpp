/*
不基于比较的排序方法：基数排序

可以证明，基于比较的排序方式（无论是插入、快排、归并..）
最坏情况的时间复杂度至少是O(n log n)

基数排序不用比较，所以能做到更好的时间复杂度

基数排序有两种，LSD和MSD，即从数据的最小位或者最大位开始排序
LSD的实现比较符合直觉，因为更高位的排序结果可以覆盖低位
MSD：
        |        |次高位破坏了之前的排序
9282 -> 2809 -> 9282
2809 -> 9282 -> 2809
所以要按照上一位排序结果分组，单独组内进行下一位排序

这里只实现计数排序和LSD radix sort
默认数据都为正整数
*/

#include <algorithm>
#include <array>
#include <iostream>
#include <vector>

void countingSort(std::vector<int>& arr) {
    int max = *std::max_element(arr.begin(), arr.end());
    std::vector<int> counts(max + 1, 0);
    // 0..max共max+1个数

    // 计数
    for (int i : arr) counts[i]++;
    // 转为前缀和
    // counts[v]表示<=v的元素个数，也就是值v这一组的结束位置（1-base）
    for (int i = 1; i <= max; i++) {
        counts[i] = counts[i] + counts[i - 1];
    }
    // 直接放置
    std::vector<int> res(arr.size());
    for (int idx = (int)arr.size() - 1; idx >= 0; idx--) {
        int v = arr[idx];
        res[counts[v] - 1] = v;  // 计数1-base，数组0-base
        counts[v]--;
    }
    arr.swap(res);
}

const int N = 10;

// 获取num的第k位，其中exp=10^(k-1) k=1,2,3..
// 对于越界返回0
int digit(int num, int exp) { return (num / exp) % 10; }

// 对第k位进行计数排序
void countingSortDigit(std::vector<int>& arr, int exp) {
    // 每位上只有0-9共10个数字，counts数组固定为10位
    std::array<int, N> counts{};

    // 对arr的每个第k位计数
    for (int i : arr) {
        counts[digit(i, exp)]++;
    }
    // 转为前缀和
    for (int i = 1; i <= N - 1; i++) {
        counts[i] = counts[i] + counts[i - 1];
    }
    // 直接放置
    std::vector<int> res(arr.size());
    for (int idx = (int)arr.size() - 1; idx >= 0; idx--) {
        int d = digit(arr[idx], exp);
        res[counts[d] - 1] = arr[idx];
        counts[d]--;
    }
    arr.swap(res);
}
// LSD
void radixSort(std::vector<int>& arr) {
    // 用于判断最大位数
    int max = *std::max_element(arr.begin(), arr.end());
    // exp = 10^(k-1), k = 1,2,3
    for (int exp = 1; exp <= max; exp *= 10) {
        countingSortDigit(arr, exp);
    }
}

int main() {
    using namespace std;

    vector<int> arr = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
    countingSort(arr);
    cout << "Counting sort:\n";
    for (int i : arr) cout << i << ' ';
    cout << '\n';

    vector<int> arr2 = {462513, 652413, 254136, 152436, 253461, 66};
    radixSort(arr2);
    cout << "Radix sort:\n";
    for (int i : arr2) cout << i << ' ';
    cout << '\n';
}

/*
remarks

digit()对于越界返回0，这相当于自动为不等长元素填充前导零
66 -> 000066
这样基数排序就继续生效了

至于为什么计数排序要倒序扫描
这主要是写着方便，顺序也是可以的
*/