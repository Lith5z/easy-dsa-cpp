/* an array list */

#include <cassert>
#include <cstring>
#include <iostream>

#define INIT 2

class AList {
private:
    int* data;
    int size;
    int capacity;

public:
    AList() : data(new int[INIT]), size(0), capacity(INIT) {}
    AList(int firstVal) : data(new int[INIT]), size(1), capacity(INIT) {
        data[0] = firstVal;
    }
    ~AList() { delete[] data; }

    void addLast(int val) {
        if (size == capacity) {
            resize(capacity * 2);
        }
        data[size] = val;
        size++;
    }
    void removeLast() { size--; }
    void resize(int newCapacity) {
        assert(newCapacity >= size);

        int* newData = new int[newCapacity];
        memcpy(newData, data, size * sizeof(int));
        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

    void print() const {
        for (int i = 0; i < size; i++) {
            std::cout << i << ": " << data[i] << '\n';
        }
    }
    int getLast() const { return data[size - 1]; }
    int get(int index) const { return data[index]; }
    int getSize() const { return size; }
    int getCapacity() const { return capacity; }
};

int main() {
    using namespace std;

    AList a;
    a.addLast(1);
    cout << a.getCapacity() << endl;
    a.addLast(2);
    cout << a.getCapacity() << endl;
    a.addLast(3);
    cout << a.getCapacity() << endl;
    a.addLast(4);
    cout << a.getCapacity() << endl;
    a.addLast(5);
    cout << a.getCapacity() << endl;
    a.addLast(6);
    cout << a.getCapacity() << endl;
    a.print();
}