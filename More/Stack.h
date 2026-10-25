#pragma once

// add and remove element in the top
class Stack {
public:
    virtual ~Stack() = default;

    virtual void push(int val) = 0;
    virtual void pop() = 0;
    virtual int getTop() const = 0;
    virtual int getSize() const = 0;
    virtual bool isEmpty() const = 0;
    virtual bool isFull() const = 0;
};