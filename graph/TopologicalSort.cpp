/*
（有向无环、无权）图的拓扑排序
如果把每个边都视作顶点的先后依赖关系
拓扑排序就是把图整理成一条链，使得每个依赖关系都得到满足
形式化的说，对于任意边 u->v 都满足 u 排在 v 之前

CS61B里的实现是带重启的DFS，取后序的逆序
*/

#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>

#include "Graph.h"

// 后序的dfs递归写法
void dfsHelper(const Graph& g, int cur, std::vector<char>& visited,
               std::vector<int>& result) {
    visited[cur] = 1;

    // postorder：先访问邻居，再访问当前顶点
    for (int next : g.getNeighbors(cur)) {
        if (visited[next] == 0) {
            dfsHelper(g, next, visited, result);
        }
    }
    result.push_back(cur);
}

std::vector<int> topologicalOrder(const Graph& g) {
    int v = g.v();
    std::vector<int> res;
    res.reserve(v);
    std::vector<char> visited(v, 0);

    for (int i = 0; i < v; i++) {
        if (visited[i] == 0) {
            dfsHelper(g, i, visited, res);
        }
    }
    std::reverse(res.begin(), res.end());
    return res;
}
// 这个方法实际上起到了dfs递归的主方法的作用
// 保存了visited和result以及调用helper

int main() {
    using namespace std;
    Graph g(6, true);
    g.addEdge(0, 1);
    g.addEdge(0, 3);
    g.addEdge(1, 5);
    g.addEdge(2, 1);
    g.addEdge(2, 5);
    g.addEdge(4, 0);
    g.addEdge(4, 1);
    g.addEdge(4, 5);

    vector<int> result = topologicalOrder(g);
    for (int p : result) {
        cout << p << ' ';
    }
    cout << '\n';
}

/*
remarks
和directed acyclic graph/有向无环图的最短路径的关系
（也就是CS61B讲拓扑排序的动机）

有了拓扑序之后，只要按照顺序处理即可
处理到p时，所有指向p的边都已经松弛过，distTo[p]就是最终值

对比dijkstra，这个算法不需要pq，而且可以处理负权边
复杂度是O(V+E)

DAG的最长路径LPT也同理，对图的边权全部取负，再求这个负图的SPT
SPT再取负就是LPT
*/
