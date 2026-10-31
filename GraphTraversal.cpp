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

    frontier_id.push(s);
    // 开始dfs
    while (!frontier_id.empty()) {
        // 取出栈顶的顶点，根据标记决定要不要访问
        int cur = frontier_id.top();
        frontier_id.pop();
        if (visited[cur])
            continue;  // 之前访问了，不访问
        else {         // 没访问过，访问并标记
            result.push_back(cur);
            visited[cur] = 1;
        }
        // 把邻居逆序放入栈，原因见后
        // 注意匹配返回类型，避免拷贝vector
        const std::vector<int>& successors = g.getNeighbors(cur);
        for (auto it = successors.rbegin(); it != successors.rend(); ++it) {
            if (!visited[*it]) frontier_id.push(*it);
        }
    }
    return result;
}
// 不算是dfs唯一写法，这个迭代写法会重复入栈，见后
// 要避免重复，必须大改成模拟栈帧的写法，比较麻烦就算了

// bfs遍历的迭代实现，首先把栈换成队列
// 标记集合的语义也不一样，这里的意思是有没有发现过
std::vector<int> bfs(const Graph& g, int s = 0) {
    std::queue<int> frontier_id;  // 边缘
    std::vector<char> discovered(g.v(), 0);
    std::vector<int> result;
    result.reserve(g.v());

    discovered[s] = 1;
    frontier_id.push(s);
    while (!frontier_id.empty()) {
        int cur = frontier_id.front();
        frontier_id.pop();
        result.push_back(cur);

        const std::vector<int>& successors = g.getNeighbors(cur);
        for (int i : successors) {
            // 只有未发现的节点才能入队，避免重复
            if (discovered[i] == 0) {
                frontier_id.push(i);
                discovered[i] = 1;
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
问题一：dfs对successors反向入栈：

1. 因为迭代方法使用了栈，递归版本
for (int next : g.getNeighbors(cur)) {
    dfs_recursion_helper(g, next, ...);
}
是直接按照邻接表中给出的顺序访问

而迭代版本如果顺序入栈
for (int next : successors) {
    frontier_id.push(next);
}
因为dfs是先放入栈再访问，所以栈的LIFO使得最后压入的邻居会最先访问，这样就和递归版本不一样了
2. 其次这和邻接表中顶点的添加顺序有关
顶点的添加顺序会影响邻接表的储存顺序（因为是vector）
可以试着调整下面main的顺序，看看输出结果不同

remarks
遍历顺序依赖 adj[v] 中邻居的存储顺序，即依赖 addEdge 的调用顺序。
图作为抽象结构不含这个顺序，所以同一张图的不同邻接表编码会给出不同的DFS树
这不影响正确性（可达集合不变），但意味着输出序列不是图的固有属性

问题二：visited和discovered的不同

理论上，把dfs的栈改成队列，就是bfs了
（当然要去掉逆序迭代器，队列本来就是FIFO的，顺序是对的）
但是这样会重复入队，这里的bfs调整了添加到标记数组的时机，去掉了重复问题

为什么dfs不也这样做？
你可以试试按着bfs的标记时机写一个dfs，虽然也不重复入栈了，但是结果是错误的，得到03214，可是3后面应该是1

std::vector<int> dfs(const Graph& g, int s = 0) {
    std::stack<int> frontier_id;
    std::vector<char> visited(g.v(), 0);
    std::vector<int> result;
    result.reserve(g.v());

    visited[s] = 1;  // here
    frontier_id.push(s);
    while (!frontier_id.empty()) {
        int cur = frontier_id.top();
        frontier_id.pop();
        result.push_back(cur);
        const std::vector<int>& successors = g.getNeighbors(cur);
        for (int i : successors) {
            if (visited[i] == 0) {    // here
                frontier_id.push(i);
                visited[i] = 1;
            }
        }
    }
    return result;
}

这是因为在0访问并出栈之后，开始处理0的邻居123，这些都未访问，就都加入了栈，而且都标记了访问
等到访问完3，开始处理3的邻居1，就被0提前固定了没法访问

bfs并不会有这个问题，123入队，123出队，3的邻居是1这种同层之间的连接直接被忽略

remarks
这其实是frontier_id的不同
bfs按照层序访问，队列中是该层（正在访问）和下一层的节点（距离+1）
dfs一直走到底，栈中是过程中所有遇到的下一个节点
*/

int main() {
    Graph g(5, true);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(0, 3);
    g.addEdge(3, 1);
    g.addEdge(1, 4);

    for (int i : dfs_recursion(g)) {
        std::cout << i << '\n';
    }
    std::cout << '\n';
    for (int i : dfs(g)) {
        std::cout << i << '\n';
    }
}