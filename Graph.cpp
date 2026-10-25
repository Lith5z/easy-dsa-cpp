/*
图的cpp邻接表实现

其实就是一个二维的数组，其中行是节点数（约定是固定的），列是节点的邻居

*/

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
    void printEdges() const {
        for (int i = 0; i < V; i++) {
            for (int e : adj[i]) {
                std::cout << i << '-' << e << std::endl;
            }
        }
    }
};

int main() {
    Graph g(3, true);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(0, 2);
    g.printEdges();
}