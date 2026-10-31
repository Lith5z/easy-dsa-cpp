/*
图的cpp邻接表实现

其实就是一个二维的数组，其中行是顶点数（约定是固定的），列是顶点的邻居
数据存储在std::vector<std::vector<int>> adj
adj[i]是顶点[i]的邻居，以vector形式给出
*/

#pragma once

#include <iostream>
#include <vector>

// vertices are 0-base
class Graph {
private:
    int V;  // fixed
    int E;
    bool directed;
    std::vector<std::vector<int>> adj;

public:
    Graph(int v, bool directed = false)
        : V(v), E(0), directed(directed), adj(v) {}

    // 如果是有向图，视作v->w
    void addEdge(int v, int w) {
        if (directed) {
            adj[v].push_back(w);
        }
        else {
            adj[v].push_back(w);
            adj[w].push_back(v);
        }
        E++;
    }
    // 返回const引用，不能通过此方法修改邻接表
    const std::vector<int>& getNeighbors(int v) const { return adj[v]; }

    int v() const { return V; }
    int e() const { return E; }
    // O(V+E)
    // 内部循环是adj[i].size，求和就是E（无向图是2E）
    // 同时对于外部遍历有V的额外开销
    void printEdges() const {
        for (int i = 0; i < V; i++) {
            for (int e : adj[i]) {
                std::cout << i << '-' << e << '\n';
            }
        }
    }
};