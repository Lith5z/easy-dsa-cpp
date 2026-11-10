/*
有权图最短路径的Dijkstra算法，权必须是非负的

- edgeTo[v] is the best known predecessor of v.
- distTo[v] is the best known total distance from source to v.
- PQ contains all unvisited vertices in order of distTo.

和CS61B里一样，Dijkstra会返回edgeTo和distTo数组，代表最短路径树SPT
所以这里用一个ShortestPath结构体，包含两个数组

此外还需要一个优先队列，存储到节点累积距离{distance, vertex}的pq
CS61B里要更新其中元素的distance，该操作叫松弛relax
但是cpp的priority_queue只能修改队顶
故这里用set实现，从小到大有序，通过删除旧值+插入新值实现修改

此文件使用了结构化绑定、列表初始化、auto等cpp特性
~~我们cpp也是有tuple用了~~
*/

#include <algorithm>
#include <iostream>
#include <set>
#include <utility>
#include <vector>

#include "WeightedGraph.h"

/*
足够大的数，视作无穷大
在一些实现里，会对无穷大进行加法，用INT_MAX会溢出
本实现里INF从不入队，只参与比较，不存在这个问题，换成INT_MAX也可以
*/
const int INF = 0x3f3f3f3f;

// 返回的路径树，需要给定一个起点s
struct ShortestPath {
    std::vector<int> edgeTo;  // edgeTo[v]为v的父节点，即前驱
    std::vector<int> distTo;  // distTo[t] = s-t的最短距离

    std::vector<int> pathTo(int v) const {
        // edgeTo的-1既代表无父节点，又作为起点s的哨兵
        // 所以不能只看edgeTo==-1判断是否有路径，这对于起点s是错误的
        if (distTo[v] == INF) return {};

        std::vector<int> path;
        while (edgeTo[v] != -1) {
            path.push_back(v);
            v = edgeTo[v];
        }
        path.push_back(v);  // 添加起点s
        // 沿着目标v回溯到起点逐个插入的，需要反向才是路径
        std::reverse(path.begin(), path.end());
        return path;
    }
};

// {distance, vertex}
// 注意是distance在前，因为set自动排序看的是pair.first
using PathCost = std::pair<int, int>;

// 给定起点s
// 时间复杂度O( (E+V)logV )
ShortestPath dijkstra(const WeightedGraph& g, int s) {
    int v = g.v();
    std::vector<int> edgeTo(v, -1);   //-1代表无前驱
    std::vector<int> distTo(v, INF);  // INF代表从s不可达
    distTo[s] = 0;

    std::set<PathCost> pq;
    pq.insert({0, s});

    while (!pq.empty()) {
        auto it = pq.begin();  // 取出最小的
        auto [distance, p] = *it;
        pq.erase(it);  // 并删除，因此上面不能用&，会造成悬空引用
        const std::vector<Edge>& neighbors = g.getNeighbors(p);
        // 考虑每个从p出发的边
        for (const auto& [q, weight] : neighbors) {  // 别搞混Edge和PathCost
            // 如果经由p到q有更优的距离，进行松弛操作
            // 即更新distTo/edgeTo/pq三者
            if (distance + weight < distTo[q]) {
                pq.erase({distTo[q], q});  // 删除旧值
                distTo[q] = distance + weight;
                edgeTo[q] = p;
                pq.insert({distTo[q], q});  // 插入新值
            }
        }
    }
    return {edgeTo, distTo};
}

int main() {
    WeightedGraph g(7, true);

    g.addEdge(0, 1, 2);
    g.addEdge(0, 2, 1);
    g.addEdge(1, 2, 5);
    g.addEdge(1, 3, 11);
    g.addEdge(1, 4, 3);
    g.addEdge(2, 4, 15);
    g.addEdge(2, 5, 15);
    g.addEdge(3, 4, 2);
    g.addEdge(4, 2, 1);
    g.addEdge(4, 5, 4);
    g.addEdge(4, 6, 5);
    g.addEdge(6, 3, 1);
    g.addEdge(6, 5, 1);

    const ShortestPath path = dijkstra(g, 0);
    std::cout << "Path from [0] to [3]: ";
    for (int i : path.pathTo(3)) {
        std::cout << i << ' ';
    }
    std::cout << '\n';
    std::cout << "Distance from [0] to [3]: ";
    std::cout << path.distTo[3] << '\n';
}

/*
auto it = pq.begin();
auto [distance, p] = *it;
pq.erase(it);

本来是

auto [distance, p] = pq.begin();
pq.erase({distance, p});

但是这样pq需要搜索{distance, p}，改成迭代器就是O(1)了
*/