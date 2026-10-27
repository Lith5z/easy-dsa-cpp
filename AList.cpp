// an array list

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
    AList() : size(0), capacity(INIT) { data = new int[INIT]; }
    AList(int first_val) : size(1), capacity(INIT) {
        data = new int[INIT];
        data[0] = first_val;
    }
    ~AList() { delete[] data; }
    AList(const AList& other) = delete;
    AList& operator=(const AList&) = delete;

    void addLast(int val) {
        if (size == capacity) {
            resize(capacity * 2);
        }
        data[size] = val;
        size++;
    }
    void removeLast() { size--; }
    void resize(int new_capacity) {
        assert(new_capacity >= size);

        int* new_data = new int[new_capacity];
        memcpy(new_data, data, size * sizeof(int));
        delete[] data;
        data = new_data;
        capacity = new_capacity;
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
    AList a;
    a.addLast(1);
    std::cout << a.getCapacity() << std::endl;
    a.addLast(2);
    std::cout << a.getCapacity() << std::endl;
    a.addLast(3);
    std::cout << a.getCapacity() << std::endl;
    a.addLast(4);
    std::cout << a.getCapacity() << std::endl;
    a.addLast(5);
    std::cout << a.getCapacity() << std::endl;
    a.addLast(6);
    std::cout << a.getCapacity() << std::endl;
    a.print();
}