#pragma once

#include <cstdlib>

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
void SimpleVector<T>::PushBack(const T& value) {
  if (Size() == 0) {
    data_ = new T[++capacity_];
  } else if (Size() == Capacity()) {
    T* old_data = data_;
    capacity_ *= 2;
    data_ = new T[capacity_];

    for (size_t i = 0; i < Size(); ++i) {
      data_[i] = old_data[i];
    }

    delete[] old_data;
  }

  data_[size_++] = value;
  end_ = data_ + size_;
}
