#pragma once
#include <iostream>
#include <stdexcept>

// An Array wrapper class that provides bounds checking on the [] operator
// does not resize like a vector so its performance should be comparable to a
// regular array
namespace b_array {
template <typename T>
class Array {
private:
    T* data = nullptr;
    size_t capacity;
    // copy src[] into self
    void copy_from_array(const T* src, size_t src_size);

public:
    Array() : data(){};
    Array(const T& item);
    Array(size_t size);
    Array(const Array<T>& other);
    ~Array() { delete[] data; }
    Array<T>& operator=(const Array<T>& other);

    // bounds checked element access, throws std::out_of_range
    T& operator[](size_t index) const;

    // prints the entire array
    friend std::ostream& operator<<(std::ostream& outs, const Array<T>& a) {
        outs << '[' << ' ';
        for (size_t i = 0; i < a.capacity; i++) {
            outs << a[i] << " ";
        }
        outs << ' ' << ']';
        return outs;
    }
};

template <typename T>
Array<T>::Array(const T& item) {
    capacity = 1;
    data = new T[1]();
    data[0] = item;
}
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
T& Array<T>::operator[](size_t index) const {
    if (index >= capacity || index < 0)
        throw std::out_of_range("Out of range");
    else
        return data[index];
}

template <typename T>
void Array<T>::copy_from_array(const T* src, size_t src_size) {
    capacity = src_size;
    delete[] data;
    data = new T[capacity]();
    for (size_t i = 0; i < src_size; i++) {
        data[i] = src[i];
    }
}
} // namespace b_array