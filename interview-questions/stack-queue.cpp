#include <stack>
#include <stdexcept>
#include <iostream>

// Generic queue implemented using two stacks instead of the traditional
// linked list, or circular array implementations.
template <typename T>
class StackQueue {
public:

  // Enqueues an element in constant time
  void enqueue(T element);

  // Dequeues an element in linear time in the worst case, which occurs
  // when the second stack (m_stackOut) is empty. If the second stack
  // contains at least one element, then that element can be dequeued
  // in constant time.
  T dequeue() throw(std::logic_error);

  // Returns the number of elements in the queue in constant time
  size_t size() const;

  // Returns true if queue is empty
  bool empty() const;

private:

  // Pop and push the elements from the first stack to the second stack
  void shiftStacks();

  // First stack - elements are pushed on to this stack if there are
  // one or more elements on the second stack
  std::stack<T> m_stackIn;

  // Second stack - an element will be pushed onto this stack if it is
  // empty, otherwise it will be pushed onto the first stack. When an
  // element is dequeued, it must be dequeued from this stack. If empty,
  // all of the elements from the first stack must be popped and pushed
  // onto this stack.
  std::stack<T> m_stackOut;
};

template<typename T>
void StackQueue<T>::enqueue(T element) {
  if (m_stackOut.empty()) {
    m_stackOut.push(element);
  } else {
    m_stackIn.push(element);
  }
}

template<typename T>
void StackQueue<T>::shiftStacks() {
  while (!m_stackIn.empty()) {
    m_stackOut.push(m_stackIn.top());
    m_stackIn.pop();
  }
}

template<typename T>
T StackQueue<T>::dequeue() throw(std::logic_error) {
  if (m_stackOut.empty()) {
    shiftStacks();
  }
  if (m_stackOut.empty()) {
    throw std::logic_error("No elements in queue");
  }
  T element = m_stackOut.top();
  m_stackOut.pop();
  return element;
}

template<typename T>
size_t StackQueue<T>::size() const {
  return m_stackIn.size() + m_stackOut.size();
}

template<typename T>
bool StackQueue<T>::empty() const {
  return size() == 0;
}

//
// ----------------------- MAIN ---------------------------
//

using std::cerr;
using std::cout;
using std::logic_error;

int main(int argc, char** argv) {
  cout << "TEST CASE 1" << std::endl;

  try {
    // Populate a queue with integer elements
    cout << "Populating queue..." << std::endl;
    StackQueue<int> queue;
    for (int i = 0; i < 10; i++) {
      queue.enqueue(i);
    }

    // Dequeue each element, hopefully in the order they were enqueued
    cout << "Dequeueing all elements in queue..." << std::endl;
    while (queue.size() > 0) {
      cout << "Dequeued: " << queue.dequeue() << std::endl;
    }

    // Repopulate
    cout << "Repopulating queue..." << std::endl;
    for (int i = 0; i < 10; i++) {
      queue.enqueue(i);
    }

    // Dequeue first five elements
    cout << "Dequeueing first five elements..." << std::endl;
    for (int i = 0; i < 5; i++) {
      cout << "Dequeued: " << queue.dequeue() << std::endl;
    }

    cout << "Enqueuing another element..." << std::endl;
    queue.enqueue(20);

    cout << "Dequeueing remaining elements..." << std::endl;
    while (!queue.empty()) {
      cout << queue.dequeue() << std::endl;
    }

    cout << "All elements dequeued." << std::endl;
    cout << "Now attempting to dequeue another item..." << std::endl;
    queue.dequeue();

  } catch (logic_error& e) {
    std::cerr << "Exception: " << e.what() << std::endl;
  }

  cout << "TEST CASE 2" << std::endl;

  StackQueue<float> queue;
  for (int i = 0; i < 20; i++) {
    queue.enqueue(float(i));
  }

  cout << "Dequeueing first five elements..." << std::endl;
  for (int i = 0; i < 5; i++) {
    cout << "Dequeued: " << queue.dequeue() << std::endl;
  }

  cout << "Alternately dequeuing/enqueuing five elements..." << std::endl;
  for (int i = 0; i < 5; i++) {
    cout << "Dequeued: " << queue.dequeue() << std::endl;
    queue.enqueue(float(i));
    cout << "Enqueued: " << float(i) << std::endl;
  }

  cout << "Dequeueing remaining elements..." << std::endl;
  while (!queue.empty()) {
    cout << "Dequeued: " << queue.dequeue() << std::endl;
  }

  return 0;
}
