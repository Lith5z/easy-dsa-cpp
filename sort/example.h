/*
排序测试的例子
*/

#pragma once

#include <iostream>
#include <vector>

std::vector<int> arr = {9, 8, 6, 7, 5, 3, 0, 4, 1, 2};

void printArr(const std::vector<int>& arr) {
    for (int p : arr) {
        std::cout << p << ' ';
    }
    std::cout << '\n';
}