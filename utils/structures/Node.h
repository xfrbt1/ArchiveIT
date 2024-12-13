#ifndef COURSE_PROJECT_NODE_H
#define COURSE_PROJECT_NODE_H

template <typename T>
struct Node
{
    T value;
    Node *next;

    Node(const T &val, Node *nxt = nullptr) : value(val), next(nxt) {}
    Node(T &&val, Node *nxt = nullptr) : value(std::move(val)), next(nxt) {}
};

#endif //COURSE_PROJECT_NODE_H
