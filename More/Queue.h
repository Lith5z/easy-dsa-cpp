#pragma once

// add new element in tail, delete in front
class Queue {
public:
    virtual ~Queue() = default;

    virtual void push(int x) = 0;
    virtual void pop() = 0;

    // should be O(1)
    virtual int getFront() const = 0;
    virtual bool isFull() const = 0;
    virtual bool isEmpty() const = 0;
};