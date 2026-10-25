// a disjoint set virtual class (interface) and its different implements
// 也就是并查集UFSet，root方法即find，connect方法即union
// 其他区别是，这个类关注元素的操作，并查集关注树的操作

#include <cassert>
#include <iostream>

using namespace std;

// size N -> [0] .. [N-1]
class DisjointSet {
public:
    virtual ~DisjointSet() = default;
    virtual void connect(int p, int q) = 0;
    // check the given 2 elements are connected or not (directly or indirectly
    // connected)
    virtual bool isConnected(int p, int q) = 0;
};

// 一种基于数组的实现，每个数都有自己的识别id
// 如果p q连接就把p
// q（以及所有和p连接的数，也就是id与p_id相同的）的id设置为一致的
class QuickCheckDS : public DisjointSet {
private:
    int* id;
    int size;

public:
    QuickCheckDS(int N) : size(N) {
        id = new int[N];
        for (int i = 0; i < N; i++) {
            id[i] = i;
        }
    }
    ~QuickCheckDS() { delete[] id; }

    // connect: O(N)
    void connect(int p, int q) override {
        int p_id = id[p], q_id = id[q];
        for (int i = 0; i < size; i++) {
            if (id[i] == p_id) {
                id[i] = q_id;
            }
        }
    }
    // check: O(1)
    bool isConnected(int p, int q) override { return id[p] == id[q]; }
};

// 同样基于数组，但是数组储存了这个数的父节点是什么，从而实现一个树状的结构
// connect只需要修改一个数，但是因为root()是O(n)的，导致connect和check都是O(n)
class QuickConnectDS : public DisjointSet {
private:
    int* parent;
    int size;

    // 找到根节点是哪个数
    // O(n)
    int root(int p) {
        int res = p;
        while (parent[res] != -1) {
            res = parent[res];
        }
        return res;
    }

public:
    QuickConnectDS(int N) : size(N) {
        parent = new int[N];
        for (int i = 0; i < N; i++) {
            parent[i] = -1;  // -1代表树的根部，一开始所有节点都是独立无连接的
        }
    }
    ~QuickConnectDS() { delete[] parent; }

    // connect: O(n)
    void connect(int p, int q) override {
        int p_root = root(p);
        int q_root = root(q);

        // 已经属于同一棵树时，不需要重复连接
        if (p_root == q_root) {
            return;
        }

        parent[p_root] = q_root;  // 连接到目标的根部，尽可能减少树的深度
    }
    // check: O(n)
    bool isConnected(int p, int q) override { return root(p) == root(q); }
};

// 用权重（树中节点的个数，记录在根节点处）决定如何连接，以降低root()的复杂度
// 用树高度实现的复杂度和权重方法是一致的，但是代码复杂很多
class WeightedQuickConnectDS : public DisjointSet {
protected:
    int* parent;

    // O(log n)
    virtual int root(int p) {
        int res = p;
        while (parent[res] >= 0) {
            res = parent[res];
        }
        return res;
    }

public:
    WeightedQuickConnectDS(int N) {
        parent = new int[N];
        for (int i = 0; i < N; i++) {
            parent[i] = -1;
        }
    }
    ~WeightedQuickConnectDS() { delete[] parent; }

    // connect: O(log n)
    void connect(int p, int q) override {
        int p_root = root(p);
        int q_root = root(q);

        // 已经属于同一棵树时，不需要重复连接或更新权重
        if (p_root == q_root) {
            return;
        }

        // 根节点储存负的树大小，单个节点的parent是-1，代表这个树有1个节点
        // 数值越小，表示树的权重越大
        // Always link root of smaller tree to larger tree
        if (parent[p_root] > parent[q_root]) {
            parent[q_root] += parent[p_root];
            parent[p_root] = q_root;
        }
        else {
            parent[p_root] += parent[q_root];
            parent[q_root] = p_root;
        }
    }
    // check: O(log n)
    bool isConnected(int p, int q) override { return root(p) == root(q); }
};

// 在按权重连接的基础上压缩路径，进一步降低root()的复杂度
// 每次调用root寻找到的根节点位置更新到遍历路径上的所有节点，以继续降低树深度
class WeightedQuickConnectWithPathCompressionDS
    : public WeightedQuickConnectDS {
protected:
    // 找到根节点，并把路径上的节点直接连接到根节点
    // O(log n)，均摊复杂度接近O(1)
    int root(int p) override {
        int res = p;
        while (parent[res] >= 0) {
            res = parent[res];
        }  // 先获取根节点是什么，才能赋值给查找路径的每个parent

        // 重新按路径遍历
        while (p != res) {
            int next = parent[p];
            parent[p] = res;
            p = next;
        }
        return res;
    }

public:
    using WeightedQuickConnectDS::WeightedQuickConnectDS;
};

int main() {
    const int N = 8;
    DisjointSet* sets[] = {new QuickCheckDS(N), new QuickConnectDS(N),
                           new WeightedQuickConnectDS(N),
                           new WeightedQuickConnectWithPathCompressionDS(N)};
    const char* names[] = {"QuickCheckDS", "QuickConnectDS",
                           "WeightedQuickConnectDS",
                           "WeightedQuickConnectWithPathCompressionDS"};

    for (int i = 0; i < 4; i++) {
        DisjointSet* set = sets[i];

        // 初始时每个元素只和自己连通
        assert(set->isConnected(0, 0));
        assert(!set->isConnected(0, 1));

        // 检查直接连接和间接连接
        set->connect(0, 1);
        set->connect(1, 2);
        assert(set->isConnected(0, 1));
        assert(set->isConnected(0, 2));
        assert(set->isConnected(2, 0));

        // 重复连接不应破坏集合结构
        set->connect(0, 2);
        assert(set->isConnected(0, 2));

        // 不同集合在合并前不应连通
        set->connect(3, 4);
        assert(set->isConnected(3, 4));
        assert(!set->isConnected(0, 3));

        // 合并两个已有集合，并检查传递连通性
        set->connect(2, 3);
        assert(set->isConnected(0, 4));
        assert(set->isConnected(1, 3));

        // 检查最后一个元素的自连通和独立状态
        assert(set->isConnected(7, 7));
        assert(!set->isConnected(0, 7));

        cout << names[i] << " passed" << endl;
        delete set;
    }
}