#ifndef STACK_TEMPLATE_H
#define STACK_TEMPLATE_H

#define MAXLEN 100
#include <QChar>

template<class T>
class Stack {
private:
    int count;
    T data[MAXLEN];
public:
    Stack();
    bool empty() const;
    bool full() const;
    void push(const T x);
    void pop();
    void get_top(T& x) const;
};

template<class T>
Stack<T>::Stack() {
    count = 0;
}

template<class T>
bool Stack<T>::empty() const {
    return count == 0;
}

template<class T>
bool Stack<T>::full() const {
    return count == MAXLEN;
}

template<class T>
void Stack<T>::push(const T x) {
    if (!full()) {
        data[count++] = x;
        return;
    }
}

template<class T>
void Stack<T>::pop() {
    if (!empty()) {
        count--;
        return;
    }
}

template<class T>
void Stack<T>::get_top(T& x) const {
    if (!empty()) {
        x = data[count - 1];
        return;
    }
}

#endif // STACK_TEMPLATE_H
