#pragma once

#include "device/cga_stream.h"
#include "utils/heap.h"
#include "utils/memutil.h"

extern CGA_Stream kout;
template <class T>
class Array
{
public:
    explicit Array(int size);
    Array(const Array& a);
    ~Array();
    T& operator=(const T& a);

    T& operator[](unsigned int index);
    void add(const T& item);

    unsigned int getObjectCount();
    void clear();
    void del(unsigned int pos);
    T get(unsigned int index);
    void set(T a, unsigned int index);
    void setCount(int count);

private:
    T* array;              // pointer for array's memory
    unsigned int cout;     // number of elemets
    unsigned int realsize; // actual size of allocated memory

    const static int dyn_array_step = 128; // initial size of array memory (elements)
};

template <class T>
Array<T>::Array(int size)
{
    this->realsize = size;
    // for dyn_array_step items
    this->setCount(0);
    this->array = (T*) malloc(this->realsize * sizeof(T));
}

template <class T>
Array<T>::~Array()
{
    if (this->array) {
        free(this->array); // Freeing memory
        this->array = nullptr;
    }
}

template <class T>
Array<T>::Array(const Array& a)
{
    this->array = (T*) malloc(sizeof(T) * a.realsize);

    memcpy(this->array, a.array, sizeof(T) * a.realsize);
    // memcpy call -- coping memory contents
    this->realsize = a.realsize;
    this->cout = a.cout;
}

template <class T>
T& Array<T>::operator=(const T& a)
{
    memcpy(this->array, a.array, sizeof(T) * a.realsize);

    this->cout = a.cout;
    this->realsize = a.realsize;

    return *this;
}

template <class T>
unsigned int Array<T>::getObjectCount()
{
    return this->cout; // simply return size
}

template <class T>
void Array<T>::del(unsigned int pos)
{
    if (this->objectCount == 1) { // If array has only one element
        clear();                  // than we clear it, since it will be dTeted
    } else {
        // otherwise, shift array elements
        for (unsigned int i = pos; i < this->objectCount - 1; i++)
            this->array[i] = this->array[i + 1];

        // decrease array size
        this->setCount(this->getObjectCount() - 1);
    }
}

template <class T>
void Array<T>::clear()
{
    this->objectCount = 0;
    this->array = (T*) realloc(this->array, sizeof(T) * this->dyn_array_step);
}

template <class T>
T Array<T>::get(unsigned int index)
{
    return this->array[index];
}

template <class T>
void Array<T>::set(T a, unsigned int index)
{
    if (index >= this->getObjectCount()) {
        this->setCount(this->getObjectCount() + 1);
    }
    this->array[index] = a;
}

template <class T>
T& Array<T>::operator[](unsigned int index)
{
    return this->array[index];
}

template <class T>
void Array<T>::add(const T& item)
{
    this->setCount(this->getObjectCount() + 1);

    if (this->getObjectCount() > this->realsize) {
        return;
    }

    this->array[this->getObjectCount() - 1] = item;
}

template <class T>
void Array<T>::setCount(int count)
{
    this->cout = count;
}