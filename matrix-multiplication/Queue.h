#pragma once

#include <mutex>
#include <optional>
#include <queue>

template<typename T>
class Queue
{
public:
  void push(T& t)
  {
    std::lock_guard<std::mutex> lock(m_mutex);

    m_items.push(t);
  }

  std::optional<T> pop()
  {
    std::lock_guard<std::mutex> lock(m_mutex);

    if (m_items.empty()) {
      return {};
    }

    T t = m_items.front();
    m_items.pop();
    return t;
  }

private:
  std::queue<T> m_items;
  std::mutex m_mutex;
};
