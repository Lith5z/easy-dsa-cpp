/*
具体实现由AI生成

在BST里，已经实现了对于一维数据的高效操作
（插入删除查询，也能实现取给定范围的元素、查找与某一数值最接近的元素）
对于有多个特征的二维数据，比如(x,y)，最朴素的方法就是用四叉树，四个子树代表
四个象限(x+,y+),(x+,y-),(x-,y-),(x-,y+)，剩下的操作都和BST类似

但是随着维度增加，这种增加树的度的方法就不是很好了
下面是k=2时的k-d树，一种交错BST，对于k维数据，依次按照k个特征进行二分

课程中的篇幅不长，我没怎么写注释，可以略读
或者参考：https://oi-wiki.org/ds/kdt/
*/

#include <algorithm>
#include <cassert>
#include <iostream>
#include <queue>
#include <vector>

// 以二维平面的点作为数据
struct Point {
    int x, y;

    Point(int _x = 0, int _y = 0) : x(_x), y(_y) {}
};

struct Node {
    Point p;
    Node *left, *right;

    Node(Point data, Node* l = nullptr, Node* r = nullptr)
        : p(data), left(l), right(r) {}
};

// 距离用平方比较，省掉sqrt，也没有浮点误差
long long dist2(const Point& a, const Point& b) {
    long long dx = a.x - b.x;
    long long dy = a.y - b.y;
    return dx * dx + dy * dy;
}

// k=2
class KDTree {
private:
    Node* root;

    // depth 1-base
    Node* build(std::vector<Point>& data, int low, int high, int depth) {
        if (low > high) return nullptr;

        int axis = (depth - 1) % 2;
        int mid = low + (high - low) / 2;
        // 要比较哪一维是运行时的信息，Point自己没法表达，用lambda把axis捕获进去
        std::nth_element(data.begin() + low, data.begin() + mid,
                         data.begin() + high + 1,
                         [axis](const Point& a, const Point& b) {
                             return axis == 0 ? a.x < b.x : a.y < b.y;
                         });

        Node* node = new Node(data[mid]);
        node->left = build(data, low, mid - 1, depth + 1);
        node->right = build(data, mid + 1, high, depth + 1);
        return node;
    }

    // 返回子树中离goal最近的点，best是进入这棵子树前已知的最优点
    Point* _nearest(Node* node, const Point& goal, int depth, Point* best) {
        if (node == nullptr) return best;

        if (best == nullptr || dist2(node->p, goal) < dist2(*best, goal))
            best = &node->p;

        int axis = (depth - 1) % 2;
        long long diff;
        if (axis == 0)
            diff = goal.x - node->p.x;
        else
            diff = goal.y - node->p.y;

        Node* goodSide = diff < 0 ? node->left : node->right;
        Node* badSide = diff < 0 ? node->right : node->left;

        best = _nearest(goodSide, goal, depth + 1, best);

        // 坏侧整棵子树都在切割线另一侧，其中任意一点到goal的距离
        // 至少是diff，比当前最优还远就不用去了
        if (diff * diff < dist2(*best, goal))
            best = _nearest(badSide, goal, depth + 1, best);

        return best;
    }

public:
    // 注意该操作会修改传入的data
    KDTree(std::vector<Point>& data) {
        root = build(data, 0, static_cast<int>(data.size()) - 1, 1);
    }

    Point nearest(const Point& goal) {
        assert(root != nullptr);
        return *_nearest(root, goal, 1, nullptr);
    }

    // 广度优先
    void print() const {
        if (root == nullptr) return;
        std::queue<Node*> frontier;
        frontier.push(root);
        while (!frontier.empty()) {
            Node* top = frontier.front();
            std::cout << top->p.x << ',' << top->p.y << ' ';
            frontier.pop();
            if (top->left != nullptr) frontier.push(top->left);
            if (top->right != nullptr) frontier.push(top->right);
        }
    }
};

int main() {
    using namespace std;
    vector<Point> arr{{7, 2}, {5, 4}, {9, 6}, {2, 3}, {4, 7}, {8, 1}};
    KDTree tree(arr);

    tree.print();
    cout << endl;

    Point goal{2, 7};
    Point best = tree.nearest(goal);
    cout << "nearest to (" << goal.x << ',' << goal.y << ") is (" << best.x
         << ',' << best.y << ')' << endl;
}
