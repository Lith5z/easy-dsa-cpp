/*
当集合中存储的元素是可以一一枚举的时候，可以用一系列的bool值代表集合中有无此元素

理论上最方便的方法是vector<char>（vector<bool>是个特殊情况，这里不讨论）
每个char都是一个byte=8bit，然而只需要1bit就足够表示1/0即有无了
为了充分利用每一个bit，我们存储一个32bit的数组，通过数学关系，其中每一个bit都能单独调整

bitset使用uint32_t保证32位，int和unsigned int都受编译器影响
为了实现方便，对于每一个32bits组成的包的内部，我们约定下标的顺序是
[31] [30] .. [2] [1] [0]
0     0   ..  1   0   1
这样让位运算的左移右移，和index对齐了，上面例子右移2位，得到的就是[31]...[2]的子序列

insert/contain/remove操作大量使用掩码mask，也就是把1U=00...01的1调整到想要改变的位置上，比如
uint32_t mask = 1U << bit_id;
-> 除了[bit_id]为1，其余位都是0
*/
#include <cstdint>
#include <iostream>

using namespace std;

class BitSet {
private:
    int setSize;     // 对外界看到的size，对应每一位bit
    int vectorSize;  // 实际上使用了多少个32位
    uint32_t* bitVector;

public:
    BitSet(int size) : setSize(size) {
        vectorSize = (size + 31) / 32;           // 不满一个数，向上取整
        bitVector = new uint32_t[vectorSize]();  // 初始为0
    }
    ~BitSet() { delete[] bitVector; }

    bool contain(int val) {
        int vector_id = val / 32, bit_id = val % 32;
        uint32_t mask = 1U << bit_id;
        return (bitVector[vector_id] & mask) != 0;
        // mask的其他位是0，取且必然是0，结果串是不是全0，只看原串[bit_id]位的值
        // mask[bit_id]位是1，原串是1即为1，非全0；为0即为全0串
    }
    void insert(int val) {
        int vector_id = val / 32, bit_id = val % 32;
        uint32_t mask = 1U << bit_id;
        bitVector[vector_id] |= mask;
        // mask的[bit_id]位是1，取或必然为1；其他位是0，取或不变
    }
    void remove(int val) {
        int vector_id = val / 32, bit_id = val % 32;
        uint32_t mask = ~(1U << bit_id);
        bitVector[vector_id] &= mask;
        // mask的[bit_id]位是0，取且必然为0；其他位是1，取且不变
    }

    // 如果想要实现集合的交、并甚至差
    // 直接逐一对bitVector的每个32bits做& |等运算就行
    // 注意内存分配和越界问题，这里的代码没有考虑
};