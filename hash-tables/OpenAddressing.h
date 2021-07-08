#pragma once

#include <algorithm>
#include <functional>
#include <optional>
#include <stdexcept>

template<
  typename Key,
  typename Value,
  typename HashFn = std::hash<Key>,
  typename EqualFn = std::equal_to<Key>
>
class OpenAddressing
{
public:
  OpenAddressing(unsigned int initialCapacity = 0)
    : m_capacity(std::max(msc_minCapacity, initialCapacity))
    , m_entriesArray(new Entry*[msc_minCapacity])
    , m_size(0)
    , m_deletedMarker(new Entry())
  {
    memset(m_entriesArray, 0, sizeof(Entry*) * m_capacity);
  }

  ~OpenAddressing()
  {
    freeEntries(m_entriesArray, m_deletedMarker, m_capacity);
    delete m_deletedMarker;
  }

  unsigned int capacity() const
  {
    return m_capacity;
  }

  bool contains(const Key& k)
  {
    return (findIndexOfKey(k) != -1);
  }

  size_t insert(const Key& k, const Value& v)
  {
    // Grow the hash table if it is too full
    maybeGrow();

    int index = findEmptyIndexForKey(k);

    // Add the entry to the hash table
    Entry* entry = m_entriesArray[index] = new Entry();
    entry->key = k;
    entry->value = v;
    m_size++;

    return index;
  }

  std::optional<Value> lookup(const Key& k)
  {
    int index = findIndexOfKey(k);
    if (index == -1) {
      return {};
    }
    return m_entriesArray[index]->value;
  }

  bool remove(const Key& k)
  {
    // Find entry
    int index = findIndexOfKey(k);
    if (index == -1) {
      return false;
    }

    delete m_entriesArray[index];
    m_entriesArray[index] = m_deletedMarker;

    // Shrink the hash table if it is too empty
    maybeShrink();
    return true;
  }

  unsigned int size() const
  {
    return m_size;
  }

private:
  struct Entry
  {
    Key key;
    Value value;
  };

  int findEmptyIndexForKey(const Key& k)
  {
    // Find a position for the new entry
    int increment = 0;
    int index = 0;
    do {
      index = indexOf(k, increment++);
    } while (m_entriesArray[index] != NULL && m_entriesArray[index] != m_deletedMarker);
    return index;
  }

  int findIndexOfKey(const Key& k)
  {
    int increment = 0;
    int index = -1;

    do {
      index = indexOf(k, increment++);
    } while (m_entriesArray[index] != NULL && (m_entriesArray[index] == m_deletedMarker || !EqualFn()(m_entriesArray[index]->key, k)));

    if (m_entriesArray[index] == NULL) {
      return -1;
    }

    return index;
  }

  static void freeEntries(Entry** entriesArray, Entry* deletedMarker, unsigned int capacity)
  {
    for (unsigned int i = 0; i < capacity; i++) {
      if (entriesArray[i] != deletedMarker) {
        delete entriesArray[i];
        entriesArray[i] = NULL;
      }
    }

    delete[] entriesArray;
    entriesArray = NULL;
  }

  unsigned int indexOf(const Key& key, unsigned int increment)
  {
    return (HashFn()(key) + increment) % m_capacity;
  }

  void reallocate(unsigned int newCapacity)
  {
    unsigned int oldCapacity = m_capacity;
    Entry** oldArray = m_entriesArray;
    m_capacity = newCapacity;
    m_entriesArray = new Entry*[m_capacity];

    for (unsigned int i = 0; i < oldCapacity; i++) {
      if (oldArray[i] != NULL && oldArray[i] != m_deletedMarker) {
        insert(oldArray[i]->key, oldArray[i]->value);
      }
    }

    freeEntries(oldArray, m_deletedMarker, oldCapacity);
  }

  void maybeGrow()
  {
    if (size() * 100 >= m_capacity * msc_maxOccupancyPercentage) {
      reallocate(m_capacity * 2);
    }
  }

  void maybeShrink()
  {
    if (size() * 100 < m_capacity * msc_minOccupancyPercentage && m_capacity > msc_minCapacity * 2) {
      reallocate(m_capacity / 2);
    }
  }

  static constexpr unsigned int msc_minCapacity = 16;
  static constexpr unsigned int msc_maxOccupancyPercentage = 70;
  static constexpr unsigned int msc_minOccupancyPercentage = 30;

  Entry** m_entriesArray;
  unsigned int m_capacity;
  unsigned int m_size;
  Entry* m_deletedMarker;
};
