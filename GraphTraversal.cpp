/*
图的dfs和bfs遍历
分别给出迭代和递归实现

（不推荐，虽然伪代码好像很好看：
def dfs(v):
    mark v
    for w in unmarked adj(v):
        dfs(w)
）

dfs给出前序，对于递归来说容易改成后序，对于迭代比较麻烦
*/

#include <iostream>
#include <queue>
#include <stack>
#include <vector>

#include "Graph.h"

// 基于栈的 DFS 迭代遍历
// preorder：操作在继续遍历之前，这里即先访问top再考虑top的邻居
std::vector<int> dfs(const Graph& g, int s = 0) {
    std::stack<int> frontier_id;  // 边缘栈，又叫fringe
    // vector<char>作为标记数组，visited[i]==0代表未访问
    // 也可以用vector<bool>，但是我觉得这个太奇葩了
    std::vector<char> visited(g.v(), 0);
    std::vector<int> result;
    result.reserve(g.v());

    // 标记，把顶点[s]放入；标记和访问不是同一时刻
    visited[s] = 1;
    frontier_id.push(s);
    // 开始dfs遍历其他
    while (!frontier_id.empty()) {
        // 访问栈顶的顶点
        int cur = frontier_id.top();
        frontier_id.pop();
        result.push_back(cur);
        // 把邻居放入栈
        // 注意匹配返回类型，避免拷贝vector
        const std::vector<int>& successors = g.getNeighbors(cur);
        for (int i : successors) {
            if (visited[i] == 0) {
                frontier_id.push(i);
                visited[i] = 1;
            }
        }
    }
    return result;
}

// bfs遍历的迭代实现，把dfs的栈换成队列，其他都不变
std::vector<int> bfs(const Graph& g, int s = 0) {
    std::queue<int> frontier_id;  // 边缘
    std::vector<char> visited(g.v(), 0);
    std::vector<int> result;
    result.reserve(g.v());

    visited[s] = 1;
    frontier_id.push(s);
    while (!frontier_id.empty()) {
        int cur = frontier_id.front();
        frontier_id.pop();
        result.push_back(cur);

        const std::vector<int>& successors = g.getNeighbors(cur);
        for (int i : successors) {
            if (visited[i] == 0) {
                frontier_id.push(i);
                visited[i] = 1;
            }
        }
    }
    return result;
}
// dfs的递归实现，需要一个帮助方法
// 因为visited和result对于不同调用都是一样的，所以需要传递该参数的引用
void dfs_recursion_helper(const Graph& g, int cur, std::vector<char>& visited,
                          std::vector<int>& result) {
    visited[cur] = 1;

    result.push_back(cur);
    // preorder：先访问当前顶点，再访问邻居；后序交换即可
    for (int next : g.getNeighbors(cur)) {
        if (visited[next] == 0) {
            dfs_recursion_helper(g, next, visited, result);
        }
    }
}
// dfs的递归实现，preorder
std::vector<int> dfs_recursion(const Graph& g, int s = 0) {
    std::vector<char> visited(g.v(), 0);
    std::vector<int> result;
    result.reserve(g.v());

    dfs_recursion_helper(g, s, visited, result);
    return result;
}

/*
同样是preorder但是和迭代方法访问顺序不同：

1. 首先因为迭代方法使用了栈，递归版本
for (int next : g.getNeighbors(cur)) {
    dfs_recursion_helper(g, next, ...);
}
是直接按照邻接表中给出的顺序访问

而迭代版本
for (int next : successors) {
    frontier_id.push(next);
}
先放入栈再访问，所以栈的LIFO使得最后压入的邻居会最先访问
2. 其次这和邻接表中顶点的添加顺序有关
顶点的添加顺序和影响邻接表的储存顺序（是vector）
可以试着调整下面(0,1)和(0,6)的顺序，看看输出结果不同

总的来说，这个迭代的dfs邻居按adj[v]的逆序访问；
若要与递归版输出一致，压栈时改成逆序遍历邻居（使用反向迭代器）

remarks
遍历顺序依赖 adj[v] 中邻居的存储顺序，即依赖 addEdge 的调用顺序。
图作为抽象结构不含这个顺序，所以同一张图的不同邻接表编码会给出不同的DFS 树
这不影响正确性（可达集合不变），但意味着输出序列不是图的固有属性
*/

int main() {
    Graph g(8, true);
    g.addEdge(3, 4);
    g.addEdge(7, 5);
    g.addEdge(0, 6);
    g.addEdge(0, 1);
    g.addEdge(2, 3);
    g.addEdge(6, 7);
    g.addEdge(1, 2);
    g.addEdge(1, 5);

    for (int i : dfs_recursion(g)) {
        std::cout << i << '\n';
    }
    std::cout << '\n';
    for (int i : dfs(g)) {
        std::cout << i << '\n';
    }
}