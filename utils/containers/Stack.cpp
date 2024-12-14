#include "Stack.h"

#include "iostream"

template <typename T>
Stack<T>::Stack() : head(nullptr), stack_size(0) {}


template <typename T>
Stack<T>::~Stack()
{
    clear();
}


template <typename T>
void Stack<T>::push(const T &value)
{
    head = new Node<T>(value, head);
    ++stack_size;
}


template <typename T>
void Stack<T>::push(T &&value)
{
    head = new Node<T>(std::move(value), head);
    ++stack_size;
}


template <typename T>
void Stack<T>::pop()
{
    if (!head)
    {
        std::cout << "empty stack!\n";
        throw std::out_of_range("Empty stack!");
    }
    Node<T> *temp = head;
    head = head->next;
    delete temp;
    --stack_size;
}


template <typename T>
T &Stack<T>::top()
{
    if (!head)
    {
        std::cout << "empty stack!\n";
        throw std::out_of_range("Empty stack!");
    }
    return head->value;
}


template <typename T>
const T &Stack<T>::top() const
{
    if (!head)
    {
        std::cout << "empty stack!\n";
        throw std::out_of_range("Empty stack!");
    }
    return head->value;
}


template <typename T>
bool Stack<T>::empty() const
{
    return head == nullptr;
}


template <typename T>
size_t Stack<T>::size() const
{
    return stack_size;
}


template <typename T>
void Stack<T>::clear()
{
    while (head)
    {
        Node<T> *temp = head;
        head = head->next;
        delete temp;
    }
    stack_size = 0;
}
