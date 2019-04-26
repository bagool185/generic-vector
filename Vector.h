#ifndef VECTOR_H
#define VECTOR_H

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>

#include "exception_utility.h"

template <class T>
class Vector {

    unsigned int capacity_;
    unsigned int size_;
    unsigned long long max_size_;

    std::allocator<T> alloc_;

    T* buffer_;

    void transfer_data_(const Vector&);
    void update_capacity_();

public:
    /* constructors */
    Vector<T>();
    Vector<T>(const unsigned int);
    Vector<T>(const unsigned int, const T);
    Vector<T>(const Vector<T>&);

    /* destructor */
    ~Vector<T>();

    /* other methods */

    /* add an element to the end of the vector */
    void push_back(const T);
    /* erase element at given position */
    void erase(const unsigned int);
    /* erase all occurrences of given element in vector */
    void erase_all(const T);
    /* clear the contents */
    void clear();
    /* return the last element in the vector and erase it */
    const T pop_back();
    /* insert an element at the given position */
    void insert(const unsigned int, const T);
    /* reserve memory for n elements  */
    void reserve(const unsigned int);
    /* changes the number of buffer stored */
    void resize(const unsigned int);
    /* reduce memory usage by freeing unused memory */
    void shrink_to_fit();
    /* find a given element */
    const unsigned int find(const T);
    /* return vector's capacity */
    unsigned int capacity() const;
    /* return vector's size */
    unsigned int size() const;

    T& at(const unsigned int) const;
    T& front();
    T& back();

    T* data() const;

    bool empty() const;

    const T* begin() const { return buffer_; }
    const T* end() const { return buffer_ + size_; }

    T& operator[](std::size_t index) { 

        if (index > size_) {
            throw OutOfBoundsException();
        }

        return buffer_[index]; 
    }
    const T& operator[](std::size_t index) const {

        if (index > size_) {
            throw OutOfBoundsException();
        }

        return buffer_[index]; 
    }
    
    Vector& operator=(const Vector& rhv) {
        
        size_ = rhv.size();
        capacity_ = rhv.capacity();
        resize(size_);
        transfer_data_(rhv);

        return *this;
    }
};

/* UTILITY PRIVATE METHODS */

template <typename T>
void Vector<T>::transfer_data_(const Vector<T>& copy_vector) {

    alloc_.deallocate(buffer_, sizeof(*buffer_) / sizeof(T));
    buffer_ = alloc_.allocate(copy_vector.size());

    const unsigned int lesser_size = std::min(copy_vector.size(), size_);

    for (unsigned int i = 0; i < lesser_size; ++i) {
        alloc_.construct(buffer_ + i, copy_vector.at(i));
    }
}

template <typename T>
void Vector<T>::update_capacity_() {
    // capacity = size / 2
    capacity_ = size_ << 1;
}

/* CONSTRUCTORS */

template<typename T>
Vector<T>::Vector() {
    size_ = 0;
    capacity_ = 2;
    buffer_ = alloc_.allocate(capacity_);
}

template <typename T>
Vector<T>::Vector(const unsigned int vector_size) {
    size_ = vector_size;
    update_capacity_();
    buffer_ = alloc_.allocate(capacity_);
}

template <typename T>
Vector<T>::Vector(const unsigned int vector_size, const T initialiser) {
    size_ = vector_size;
    update_capacity_();
    buffer_ = alloc_.allocate(capacity_);
    
    std::uninitialized_fill_n(buffer_, capacity_, initialiser);
}

template <typename T>
Vector<T>::Vector(const Vector<T>& copy_vector) {
    size_ = copy_vector.size();
    capacity_ = copy_vector.capacity();
    buffer_ = alloc_.allocate(capacity_);
    
    transfer_data_(copy_vector);
}

template <typename T>
Vector<T>::~Vector() {

    alloc_.deallocate(buffer_, sizeof(*buffer_) / sizeof(T));
    buffer_ = nullptr;

    capacity_ = size_ = max_size_ = 0;
}

/* DATA INSERTION */

template <typename T>
void Vector<T>::push_back( T new_val ) {

    if (size_ >= capacity_) {
        update_capacity_();
        resize(capacity_);
    }

    alloc_.construct(buffer_ + size_, new_val);
    ++size_;
}

template <typename T>
void Vector<T>::insert(unsigned int position, T value) {
    if (position <= size_) {
        if (size_ >= capacity_) {
            update_capacity_();
            resize(size_);
        } 
        // every element to position's right gets shifted to the right
        // by one spot
        for (unsigned int i = size_; i > position; --i) {
            alloc_.construct(buffer_ + i, buffer_[i - 1]);
        }

        alloc_.construct(buffer_ + position, value);

        ++size_;
    }
    else {
        throw OutOfBoundsException();
    }
}

/* DATA DELETION */

template <typename T>
void Vector<T>::clear() {
    size_ = 0;
    capacity_ = 2;
}

template <typename T>
void Vector<T>::erase(const unsigned int index) {

    if (index < (size_) && index >= 0) {
        // every element starting with index gets shifted to the left
        // by one spot
        for (unsigned int i = index; i < size_ - 1; ++i) {
            alloc_.construct(buffer_ + i, buffer_[i + 1]);
        }

        --size_;

        if (size_ < (capacity_ >> 2)) {
            update_capacity_();
            resize(capacity_);
        }
    }
    else {
        throw OutOfBoundsException();
    }
}

template <typename T>
void Vector<T>::erase_all(const T value) {

    for (unsigned int i = 0; i < size_; ++i) {
        if (buffer_[i] == value) {
            erase(i--);
        }
    }
}
/* DATA EXTRACTION */

template <typename T>
T& Vector<T>::at(const unsigned int position) const {
    if (position < size_ && position >= 0) {
        return buffer_[position];
    }

    throw OutOfBoundsException();
}

template <typename T>
T& Vector<T>::front() {
    if (size_ >= 1) {
        return buffer_[0];
    }

    throw EmptyVectorException();
}

template <typename T>
T& Vector<T>::back() {
    if (size_ >= 1) {
        return buffer_[size_ - 1];
    }

    throw EmptyVectorException();
}

template <typename T>
const T Vector<T>::pop_back() {
    if (!empty()) {
        const unsigned int last_index = size_ - 1;
        const T last_value = buffer_[last_index];
        
        erase(last_index);

        return last_value;
    }

    throw EmptyVectorException();
}

template <typename T>
const unsigned int Vector<T>::find(const T elem) {

    for (unsigned int i = 0; i < size_; ++i) {
        if (buffer_[i] == elem) {
            return i;
        }
    }

    return capacity_;
}

/* UTILITY METHODS */

template <typename T>
unsigned int Vector<T>::capacity() const {
    return capacity_;
}

template <typename T>
unsigned int Vector<T>::size() const {
    return size_;
}

template <typename T>
T* Vector<T>::data() const {
    return buffer_;
}

template <typename T>
void Vector<T>::reserve(unsigned int capacity) {
    
    if (buffer_ == nullptr) {
        size_ = capacity_ = 0;
        return;
    }

    T * new_buffer = alloc_.allocate(capacity);

    unsigned int lesser_size = (capacity < size_ ) ? capacity : size_;

    for (unsigned int i = 0; i < lesser_size; ++i) {
        alloc_.construct(new_buffer + i, buffer_[i]);
    }

    alloc_.deallocate(buffer_, capacity_);
    size_ = lesser_size;
    update_capacity_();
    buffer_ = new_buffer;

    new_buffer = nullptr;
}

template <typename T>
void Vector<T>::resize(const unsigned int new_size) {
    size_ = new_size;
    update_capacity_();
    reserve(capacity_);
}

template <typename T>
bool Vector<T>::empty() const {
    return (size_ == 0);
}

template <typename T>
void Vector<T>::shrink_to_fit() {
    resize(size_);
}

#endif //VECTOR_H