#pragma once
#include <stdexcept>

// it's like an array, but it has bounds checking
template <typename T>
class Array {
private:
    T* data = nullptr;
    size_t capacity;

public:
    Array() : data() {}
    Array(size_t size);
    Array(const Array<T>& other);
    ~Array() { delete[] data; }
    Array<T>& operator=(const Array<T>& other);

    T& operator[](int index) const;

    // copy src[] into self
    void copy_from_array(const T* src, size_t src_size) {
        capacity = src_size;
        delete[] data;
        data = new T[capacity]();
        for (size_t i = 0; i < src_size; i++) {
            data[i] = src[i];
        }
    }
};

template <typename T>
Array<T>::Array(size_t size) {
    capacity = size;
    data = new T[capacity]();
}
template <typename T>
Array<T>& Array<T>::operator=(const Array<T>& other) {
    if (this == &other)
        return *this;
    delete[] data;
    data = new T[other.capacity];
    copy_from_array(other.data, other.capacity);
    return *this;
}
template <typename T>
Array<T>::Array(const Array<T>& other) {
    data = new T[other.capacity];
    copy_from_array(other.data, other.capacity);
}

template <typename T>
T& Array<T>::operator[](int index) const {
    if (index >= capacity || index < 0)
        throw std::out_of_range("Out of range");
    else
        return data[index];
}