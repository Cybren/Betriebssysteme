
#pragma once
#include "object/queuelink.h"

template<typename T, QueueLink<T> T::* next_field = nullptr>
class Queue {
    Queue(const Queue<T, next_field> &) = delete;
    Queue &operator=(const Queue<T, next_field> &) = delete;
private:
    T *head;
    T **tail;
public:
    Queue() : head(0), tail(&head) {}

    static QueueLink<T> *get_node(T *o) {
        return queue_link_get<T, next_field>::call(o);
    }

    void enqueue(T *item) {
        QueueLink<T> *node = get_node(item);
        node->next = 0;
        *tail = item;
        tail = &(node->next);
    }

    T *dequeue() {
        T *out;
        if ((out = head)) {
            if (!(head = get_node(out)->next))
                tail = &head;
            get_node(out)->next = nullptr;
        }
        return out;
    }

    class Iterator {
        T *first;
    public:
        Iterator() : first(nullptr) {};

        Iterator(Queue<T, next_field> *queue) : first(queue->head) {};

        bool operator!=(const Iterator &other) {
            return first != other.first;
        }

        T *operator*() { return first; }

        Iterator &operator++() {
            first = get_node(first)->next;
            return *this;
        }
    };

    Iterator begin() {
        return Iterator(this);
    }

    Iterator end() {
        return Iterator();
    }

    T *remove(T *item, bool (*cmp)(T *, T *) = [](T *a, T *b) { return a == b; }) {
        T **prev = &head;
        for (T *current : *this) {
            if (cmp(item, current)) {
                *prev = get_node(current)->next;
                if (&get_node(current)->next == tail) tail = prev;
                return current;
            }
            prev = &get_node(current)->next;
        }
        return 0;
    }

    void insert_first(T *item) {
        get_node(item)->next = head;
        head = item;
        if (tail == &head) tail = &get_node(item)->next;
    }

    void insert_after(T *old_item, T *new_item) {
        get_node(new_item)->next = get_node(old_item)->next;
        get_node(old_item)->next = new_item;
        if ((*tail) == new_item)
            tail = &(get_node(new_item)->next);
    }

    T *first() {
        T *out;
        out = head;
        return out;
    }

    T *next(T *o) {
        return get_node(o)->next;
    }
};

