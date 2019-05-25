#pragma once

#include <cstdlib>
#include <iostream>
using namespace std;

// Реализуйте шаблон SimpleVector
template <typename T>
class SimpleVector {
public:
  SimpleVector();
  explicit SimpleVector(size_t size);
  ~SimpleVector();

  T& operator[](size_t index);

  T* begin();
  T* end();

  const T* begin() const;
  const T* end() const;

  size_t Size() const;
  size_t Capacity() const;
  void PushBack(const T& value);

private:
  size_t size_;
  size_t capacity_;
  size_t cur_index = 0;
  T* data_;
  T* end_;
};

template <typename T>
SimpleVector<T>::SimpleVector() {
  size_ = capacity_ = 0;
  data_ = end_ = nullptr;
}

template <typename T>
SimpleVector<T>::SimpleVector(size_t size) {
  capacity_ = size_ = size;
  data_ = new T[size];
  end_ = data_ + size_;
}

template <typename T>
SimpleVector<T>::~SimpleVector() {
  delete[] data_;
}

template <typename T>
T& SimpleVector<T>::operator[](size_t index) {
  return data_[index];
}

template <typename T>
T* SimpleVector<T>::begin() {
  return data_;
}

template <typename T>
T* SimpleVector<T>::end() {
  return end_;
}

template <typename T>
const T* SimpleVector<T>::begin() const {
  return data_;
}

template <typename T>
const T* SimpleVector<T>::end() const {
  return end_;
}

template <typename T>
size_t SimpleVector<T>::Size() const {
  return size_;
}

template <typename T>
size_t SimpleVector<T>::Capacity() const {
  return capacity_;
}

template <typename T>
void SimpleVector<T>::PushBack(const T& value) {}
