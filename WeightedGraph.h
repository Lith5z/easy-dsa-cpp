/*
有权图，同样是邻接表
区别是本来adj[v]是v的所有邻居，现在还需要存储边的权重
于是需要一个结构体同时存储邻居和权重，接下来把存储的int改成这个结构体即可
*/

#pragma once

#include <iostream>
#include <vector>

struct Edge {
    int to;  // 邻居
    int weight;
};

// vertices are 0-base
class WeightedGraph {
private:
    int V;  // fixed
    int E;
    bool directed;
    std::vector<std::vector<Edge>> adj;

public:
    WeightedGraph(int v, bool directed = false)
        : V(v), E(0), directed(directed), adj(v) {}

    // 如果是有向图，视作v->w
    void addEdge(int v, int w, int weight) {
        if (directed) {
            // cpp11的{}列表初始化
            adj[v].push_back({w, weight});
        }
        else {
            adj[v].push_back({w, weight});
            adj[w].push_back({v, weight});
        }
        E++;
    }
    const std::vector<Edge>& getNeighbors(int v) const { return adj[v]; }

    int v() const { return V; }
    int e() const { return E; }

    void printEdges() const {
        for (int i = 0; i < V; i++) {
            for (const Edge& e : adj[i]) {
                std::cout << i << '-' << e.to << ',' << e.weight << '\n';
            }
        }
    }
};