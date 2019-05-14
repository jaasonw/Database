#pragma once
#include "../array_util.h"
#include <exception>

// it's like an array, but it has bounds checking
template <typename T>
class FakeArray {
private:
    T* data = nullptr;
    size_t capacity;
public:
    FakeArray() : data() {}
    FakeArray(size_t size);
    FakeArray(const FakeArray<T>& other);
    ~FakeArray() { delete[] data; }
    FakeArray<T>& operator=(const FakeArray<T>& other);

    T& operator[](int index) const;
};

template <typename T>
FakeArray<T>::FakeArray(size_t size) {
    capacity = size;
    data = new T[capacity]();
}
template <typename T>
FakeArray<T>& FakeArray<T>::operator=(const FakeArray<T>& other) {
    if (this == &other)
        return *this;
    delete[] data;
    data = new T[other.capacity];
    array::copy_array(other.data, data, other.capacity, capacity);
    return *this;
}
template <typename T>
FakeArray<T>::FakeArray(const FakeArray<T>& other) {
    data = new T[other.capacity];
    array::copy_array(other.data, data, other.capacity, capacity);
}

template <typename T>
T& FakeArray<T>::operator[](int index) const {
    if (index >= capacity || index < 0)
        throw std::out_of_range("Out of range");
    else
        return data[index];
}