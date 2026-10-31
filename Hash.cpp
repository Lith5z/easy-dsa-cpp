/*
哈希表
利用之前的内容，已经可以实现一个性能比较好的集合：
- 使用有序的数组或链表，配合二分查找，插入删除O(n)，查找O(log n)
- 使用2-3tree或者LLRB/红黑树，插入删除查找都是O(log n)，后者实现简单很多

但是还可以做得更好，对于如果需要的是无序的集合：
- 元素可能无法比较
- 复杂度可以比O(log n)更低

这就需要哈希表了，通过hash()把元素映射到索引上，这需要：
- hash函数的设计
- 如果不同元素映射到同一个index，该如何处理冲突

后者一般是每个位置存储一个桶，可以容纳多个元素（链表）

但是随着元素的增加，插入/查询/删除的操作最坏都是O(q)，q是最长链表的长度
而q ~ n/m，m为桶的数量。n/m又称为load ratio
这意味着逐渐的，操作会变成O(n)。为了避免这个问题，当n/m超出某个值的时候，m需要resize
resize时所有元素的下标都会变，必须重新散列；但只需要移动string，不需要拷贝字符
*/

#include <cassert>
#include <string>
#include <utility>
#include <vector>

using namespace std;

// 和java的String.hashCode一样是31进制多项式哈希：h = h * 31 + c
// 为了避免负数，用unsigned而不是int
unsigned int hashcode(const string& s) {
    unsigned int hash = 0;
    for (char c : s) {
        hash = hash * 31u + static_cast<unsigned char>(c);
    }
    return hash;
}

class StringSet {
private:
    int size;        // 当前元素个数
    int bucket_num;  // 桶的数量，即buckets数组的长度
    vector<string>* buckets;

    // 桶的数量改变后，所有元素都要重新计算下标
    void resize(int new_bucket_num) {
        vector<string>* new_buckets = new vector<string>[new_bucket_num];
        for (int i = 0; i < bucket_num; i++) {
            // 这里不能写const string&，否则std::move会退化成拷贝
            for (string& s : buckets[i]) {
                new_buckets[hashcode(s) % new_bucket_num].push_back(
                    std::move(s));
            }
        }
        delete[] buckets;
        buckets = new_buckets;
        bucket_num = new_bucket_num;
    }

public:
    StringSet(int initial_bucket_num = 8)
        : size(0), bucket_num(initial_bucket_num) {
        assert(bucket_num > 0);
        buckets = new vector<string>[bucket_num];
    }
    ~StringSet() { delete[] buckets; }

    void insert(const string& s) {
        int index = hashcode(s) % bucket_num;
        // 桶中可能有多个元素，先检查是否已经存在
        for (const string& x : buckets[index]) {
            if (x == s) return;
        }
        buckets[index].push_back(s);
        size++;

        // load ratio = size / bucket_num，超过1就扩容
        if (size > bucket_num) resize(bucket_num * 2);
    }
    bool contain(const string& s) const {
        int index = hashcode(s) % bucket_num;
        for (const string& x : buckets[index]) {
            if (x == s) return true;
        }
        return false;
    }
    void remove(const string& s) {
        int index = hashcode(s) % bucket_num;
        vector<string>& bucket = buckets[index];
        for (int i = 0; i < static_cast<int>(bucket.size()); i++) {
            if (bucket[i] == s) {
                bucket.erase(bucket.begin() + i);
                size--;
                return;
            }
        }
    }
};
