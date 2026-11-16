/*
并查集DisjointSet的WeightedQuickConnectWithPathCompressionDS
单独抽离一个文件，给MST.cpp的Kruskal复用
因为STL库没这样的工具，只能自己写
*/
#pragma once

#include <vector>

// 存储int
class UFS {
private:
    std::vector<int> parent;

public:
    explicit UFS(int n) : parent(n, -1) {}

    int root(int p) {
        int res = p;
        while (parent[res] >= 0) {
            res = parent[res];
        }  // res=root

        while (p != res) {
            int next = parent[p];
            parent[p] = res;
            p = next;
        }  // path compression
        return res;
    }

    void connect(int p, int q) {
        int pRoot = root(p);
        int qRoot = root(q);

        // 已经连接
        if (pRoot == qRoot) {
            return;
        }

        // Always link root of smaller tree to larger tree
        if (parent[pRoot] > parent[qRoot]) {
            parent[qRoot] += parent[pRoot];
            parent[pRoot] = qRoot;
        }
        else {
            parent[pRoot] += parent[qRoot];
            parent[qRoot] = pRoot;
        }
    }
    bool isConnected(int p, int q) { return root(p) == root(q); }
};