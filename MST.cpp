/*
无向图，最小生成树Minimum Spanning Tree
两种算法Prim和Kruskal
*/

#include <functional>
#include <iostream>
#include <queue>
#include <set>
#include <tuple>
#include <vector>

#include "More/UFS.h"
#include "WeightedGraph.h"

const int INF = 0x3f3f3f3f;

/*
Prim的实现和Dijkstra基本一致，只不过：
- 因为需要检查节点是不是在正在构建的树中，多了一个marked集合
- MST的distTo[v]是入[v]边的权值（也就是原本图中边的权重），prim中也改变
- 优先队列pq的{distance,}本来是累积距离，现在是到正在构建的树的距离

当然Prim算法的实现有很多，这里选择了和CS61B的实现
同时也和我的Dijkstra实现形成对照
*/

// {distance, vertex}
using PathCost = std::pair<int, int>;

// 最小生成树Minimum Spanning Tree
// e/weight/spans这些计算属性的方法就是你想的这样，没必要看
struct MST {
    int root;
    std::vector<int> edgeTo;  // edgeTo[v]为v的父节点，即前驱
    std::vector<int> distTo;  // distTo是入[v]边的权值

    int e() const {
        int e = 0;
        for (int i = 0; i < (int)edgeTo.size(); i++) {
            if (edgeTo[i] != -1) e++;
        }
        return e;
    }
    long long weight() const {
        long long sum = 0;
        for (int i = 0; i < (int)edgeTo.size(); i++) {
            if (edgeTo[i] != -1) sum += distTo[i];
        }
        return sum;
    }
    // E == V-1，即为生成树
    bool spans() const { return e() == (int)edgeTo.size() - 1; }
};

MST prim(const WeightedGraph& g, int s) {
    int v = g.v();
    std::vector<int> edgeTo(v, -1);  //-1代表无前驱
    std::vector<int> distTo(v, INF);
    std::vector<char> marked(v, 0);  // 是否已经在结果MST内
    distTo[s] = 0;

    std::set<PathCost> pq;
    pq.insert({0, s});
    while (!pq.empty()) {
        auto it = pq.begin();
        auto [distance, p] = *it;  // 不要用&，理由见dijkstra
        pq.erase(it);
        marked[p] = 1;

        const std::vector<Edge>& neighbors = g.getNeighbors(p);
        for (const auto& [q, weight] : neighbors) {
            // 选取(顶点在树中-顶点不在树中)这样的边添加
            if (marked[q]) continue;
            // 主要区别在这部分，松弛的条件和distTo有变化
            if (weight < distTo[q]) {
                pq.erase({distTo[q], q});
                distTo[q] = weight;
                edgeTo[q] = p;
                pq.insert({distTo[q], q});
            }
        }
    }
    return {s, edgeTo, distTo};
}

/*
Kruskal算法因为需要得到{from,to,w}的所有边
然而Edge只有{to,w}，from信息存储在adj[i]的i里
让我们使用一个tuple叫FullEdge来存储这样的边
注意排序要求weight是最先的

此外，因为Kruskal排序好所有边之后，只需要从小到大依次处理边即可
没有像dijkstra那样修改优先级的操作，故这里用priority_queue，不用set了

而且Kruskal不需要distTo这些数组，输出也是FullEdge数组
所以不得不独立写一个单独于MST的KruskalMST结构体
再加上Kruskal算法不需要一个起始顶点，该结构体也删了root元素
*/

// {w,from,to}
using FullEdge = std::tuple<int, int, int>;

struct KruskalMST {
    int v;  // 没有distTo这些辅助数组提供总定点数，只能单独存
    std::vector<FullEdge> edges;

    int e() const { return (int)edges.size(); }
    long long weight() const {
        long long sum = 0;
        for (const auto& [w, from, to] : edges) {
            sum += w;
        }
        return sum;
    }
    bool spans() const { return (int)edges.size() == v - 1; }
};

KruskalMST kruskal(const WeightedGraph& g) {
    int v = g.v();
    std::vector<FullEdge> mst;
    UFS ufs(v);

    // 把图中的所有边{w,from,to}插入优先队列
    // priority_queue默认大顶堆，这里要指定greater才是小顶堆
    std::priority_queue<FullEdge, std::vector<FullEdge>, std::greater<>> pq;
    for (int i = 0; i < v; i++)
        for (const Edge& e : g.getNeighbors(i))
            // MST是无向图的性质，无向图里有a-b,b-a两条边
            // 只取出a<b的有序对，防止重复
            if (i < e.to) pq.push({e.weight, i, e.to});

    // 如果E=V-1，说明已经构成mst，可以退出
    while (!pq.empty() && (int)mst.size() != v - 1) {
        auto [w, from, to] = pq.top();  // 不要用&
        pq.pop();
        // 取出最小权的边，只要不构成环就添加此边
        if (!ufs.isConnected(from, to)) {
            ufs.connect(from, to);
            mst.push_back({w, from, to});
        }
    }

    return {v, mst};
}

/*
One More Thing：
两种算法对于不连通图的支持情况

Kruskal在pq中添加了所有边，所以可以支持不连通图
Prim是从起点s开始生长的，所以只能完成图中含有s的分量的mst
*/

int main() {
    WeightedGraph g(7, false);  // 无向图

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

    std::cout << "Using prim algorithm:\n";
    const MST mst = prim(g, 0);
    std::cout << "Given start: " << mst.root << '\n';
    std::cout << "Num of edges in MST: " << mst.e() << '\n';
    std::cout << "Is spanning: " << mst.spans() << '\n';
    std::cout << "Total distance: " << mst.weight() << '\n';

    std::cout << "\nUsing kruskal algorithm:\n";
    const KruskalMST mst2 = kruskal(g);
    std::cout << "Kruskal doesn't need a start\n";
    std::cout << "Num of edges in MST: " << mst2.e() << '\n';
    std::cout << "Is spanning: " << mst2.spans() << '\n';
    std::cout << "Total distance: " << mst2.weight() << '\n';
}