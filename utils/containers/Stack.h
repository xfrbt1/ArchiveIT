#ifndef COURSE_PROJECT_STACK_H
#define COURSE_PROJECT_STACK_H


#include <stdexcept>
#include <algorithm>
#include "../utils/structures/Node.h"


template <typename T>
class Stack
{
private:
    Node<T> *head;
    size_t stack_size;

public:
    Stack();
    ~Stack();

    void push(const T &value);
    void push(T &&value);
    void pop();
    T &top();
    const T &top() const;
    bool empty() const;
    size_t size() const;
    void clear();
};

#endif //COURSE_PROJECT_STACK_H
