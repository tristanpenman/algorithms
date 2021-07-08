#pragma once

#include <cstring>
#include <iostream>

template<typename T>
void bubble_down(T* elements, size_t size, size_t target)
{
  size_t min = target;

  for (int i = 1; i <= 2; i++) {
    size_t p = target * 2 + i;
    if (p < size && elements[p] > elements[min]) {
      min = p;
    }
  }

  if (target != min) {
    T tmp = elements[target];
    elements[target] = elements[min];
    elements[min] = tmp;
    bubble_down(elements, size, min);
  }
}

template<typename T>
void make_heap(T* elements, size_t size)
{
  for (int i = int(size) - 1; i >= 0; i--) {
    bubble_down(elements, size, i);
  }
}

template<typename T>
class BubbleDownHeap
{
public:
  BubbleDownHeap(const T* inputArray, size_t length)
    : elements(0)
    , size(0)
    , capacity(1)
  {
    while (capacity < length) {
      capacity *= 2;
    }

    elements = new T[capacity];
    memcpy(elements, inputArray, sizeof(T) * length);
    size = length;
    make_heap(elements, size);
  }

  ~BubbleDownHeap()
  {
    delete[] elements;
  }

  BubbleDownHeap(const BubbleDownHeap<T>& other)
  {
    elements = new T[other.capacity];
    memcpy(elements, other.elements, sizeof(T) * other.capacity);
    size = other.size;
    capacity = other.capacity;
  }

  BubbleDownHeap(BubbleDownHeap&& other)
  {
    elements = other.elements;
    other.elements = NULL;
  }

  size_t getSize() const
  {
    return size;
  }

  size_t getCapacity() const
  {
    return capacity;
  }

  T pop()
  {
    if (size == 0) {
      throw;
    }

    T result = elements[0];
    elements[0] = elements[size-1];
    size--;
    bubble_down(elements, size, 0);

    return result;
  }

private:
  T* elements;

  size_t size;
  size_t capacity;
};
