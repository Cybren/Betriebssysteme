
#pragma once

template<typename T>
class QueueLink;

template<typename T, QueueLink<T> T::*>
class Queue;

template<typename T>
class QueueLink {
    template<typename X, QueueLink<X> X::* x> friend
    class Queue;

private:
    T *next;
public:
    QueueLink<T>() : next(nullptr) {}

};

template<bool B, class T = void>
struct enable_if {
};
template<class T>
struct enable_if<true, T> {
    typedef T type;
};

template<typename X, QueueLink<X> X::* next_field, class Enable = void>
struct queue_link_get {
    static QueueLink<X> *call(X *o) {
        return &(o->queue_link);
    }
};

template<typename X, QueueLink<X> X::* next_field>
struct queue_link_get<X, next_field, typename enable_if<next_field != nullptr>::type> {
    static QueueLink<X> *call(X *o) {
        return &(o->*next_field);
    }
};

