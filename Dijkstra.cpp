/*
有权图最短路径的Dijkstra算法，权必须是非负的

- edgeTo[v] is the best known predecessor of v.
- distTo[v] is the best known total distance from source to v.

和CS61B里一样，Dijkstra会返回edgeTo和distTo数组，代表最短路径树SPT
所以这里用一个ShortestPath结构体，包含两个数组

此外还需要一个优先队列，存储到节点累积距离{distance, vertex}的pq
CS61B里要更新其中元素的distance，该操作叫松弛relax
但是cpp的priority_queue只能修改队顶
故这里用set实现，从小到大有序，通过删除旧值+插入新值实现修改
（不用担心set会忽略同权值的边，可以看后面的pair，先比first再比second）

此文件使用了结构化绑定、列表初始化、auto、pair等cpp特性
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
本实现里INF从不入队，只参与比较查找
所以不存在溢出问题，换成INT_MAX也可以
*/
const int INF = 0x3f3f3f3f;

// 返回的路径树，需要给定一个起点s作为root
struct ShortestPath {
    int root;
    std::vector<int> edgeTo;  // edgeTo[v]为v的父节点，即前驱
    std::vector<int> distTo;  // distTo[t] = s-t的最短距离

    std::vector<int> pathTo(int v) const {
        if (distTo[v] == INF) return {};  // 不可达节点处理
        std::vector<int> path;
        while (v != root) {
            path.push_back(v);
            v = edgeTo[v];
        }
        path.push_back(v);  // 添加起点root
        // 沿着目标v回溯到起点逐个插入的，需要反向才是路径
        std::reverse(path.begin(), path.end());
        return path;
    }
};

// {distance, vertex}
// 注意是distance在前，因为set自动排序先看first，再看second
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
        auto it = pq.begin();  // 取出最小的，用it见后
        auto [distance, p] = *it;
        pq.erase(it);  // 并删除，因此上面不能用&，会造成悬空引用

        const std::vector<Edge>& neighbors = g.getNeighbors(p);
        // 考虑每个从p出发的边
        for (const auto& [q, weight] : neighbors) {  // 别搞混Edge和PathCost
            // 如果经由p到q有更优的距离，进行松弛操作
            // 即更新distTo/edgeTo/pq三者
            if (distance + weight < distTo[q]) {
                pq.erase({distTo[q], q});  // 删除旧值，见后
                distTo[q] = distance + weight;
                edgeTo[q] = p;
                pq.insert({distTo[q], q});  // 插入新值
            }
        }
    }
    return {s, edgeTo, distTo};
}

/*
Proof sketch from CS61B:
Assume all edges have non-negative weights.
At start, distTo[source] = 0, which is optimal.
After relaxing all edges from source, let vertex v1 be the vertex with
minimum weight, i.e. that is closest to the source.

Claim: distTo[v1] is optimal, and thus future relaxations will fail. Why?
For every vertex p not yet dequeued (including undiscovered ones, whose
distTo is INF), distTo[p] >= distTo[v1], because v1 is the minimum in pq.
Any better path to v1 would have to leave the dequeued set through such a p,
and then distTo[p] + w >= distTo[v1], since all weights are non-negative.
Can use induction to prove that this holds for all vertices after dequeuing.
*/

int main() {
    WeightedGraph g(7, false);

    g.addEdge(0, 1, 2);
    g.addEdge(0, 2, 1);
    g.addEdge(1, 2, 5);
    g.addEdge(1, 3, 11);
    g.addEdge(1, 4, 3);
    g.addEdge(2, 4, 1);
    g.addEdge(2, 5, 15);
    g.addEdge(3, 4, 2);
    g.addEdge(3, 6, 1);
    g.addEdge(4, 5, 4);
    g.addEdge(4, 6, 3);
    g.addEdge(5, 6, 1);

    const ShortestPath path = dijkstra(g, 0);
    std::cout << "Given start: " << path.root << '\n';
    std::cout << "Path from [0] to [3]: ";
    for (int i : path.pathTo(3)) {
        std::cout << i << ' ';
    }
    std::cout << '\n';
    std::cout << "Distance from [0] to [3]: ";
    std::cout << path.distTo[3] << '\n';
}

/*
1. 用it的原因

auto it = pq.begin();
auto [distance, p] = *it;
pq.erase(it);

本来是

auto [distance, p] = pq.begin();
pq.erase({distance, p});

但是这样pq需要搜索{distance, p}，带来额外开销
改成迭代器就是O(1)了

2. pq里存的是顶点（及其当前最优距离），不是边
因而 pq.erase({distTo[q], q}); 可能删除一个还没入队的顶点
但是set对不存在的键不报错，所以没有问题

带入第一次循环：
循环之前pq只有起点{0, s}，进入主循环就被取出删除了
进入for循环，pq是空的，erase自然什么也找不到
*/