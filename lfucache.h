// MIT License
//
// Copyright (c) 2025 Carson Smith
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
#pragma once

#include <list>
#include <unordered_map>
#include <utility>

#include "cacheinterface.h"

namespace cts {

template <typename Key, typename Value>
class LFUCache {
    using KVPair = std::pair<Key, Value>;

    std::unordered_map<size_t, std::list<KVPair>>
        m_frequencies;  // Frequency → List of key-value pairs
    std::unordered_map<Key, std::pair<size_t, typename std::list<KVPair>::iterator>>
        m_cache;  // Key → (Frequency, Iterator)
    size_t m_capacity;
    size_t m_minf;

    void insert(const Key& key, const size_t frequency, const Value& value) {
        m_frequencies[frequency].emplace_back(key, value);
        m_cache[key] = {frequency, --m_frequencies[frequency].end()};
    }

   public:
    LFUCache(const size_t capacity = 0) : m_capacity(capacity), m_minf(0) {}

    Value* get(const Key& key) {
        auto it = m_cache.find(key);
        if (it == m_cache.end()) {
            return nullptr;
        }

        size_t f = it->second.first;
        auto iter = it->second.second;
        Value& value = iter->second;

        m_frequencies[f].erase(iter);

        if (m_frequencies[f].empty()) {
            m_frequencies.erase(f);
            if (m_minf == f) {
                ++m_minf;
            }
        }

        insert(key, f + 1, value);
        return &m_cache[key].second->second;
    }

    void put(const Key& key, const Value& value) {
        if (m_capacity == 0) {
            return;
        }

        auto it = m_cache.find(key);
        if (it != m_cache.end()) {
            it->second.second->second = value;
            get(key);
            return;
        }

        if (m_cache.size() >= m_capacity) {
            m_cache.erase(m_frequencies[m_minf].front().first);
            m_frequencies[m_minf].pop_front();
            if (m_frequencies[m_minf].empty()) {
                m_frequencies.erase(m_minf);
            }
        }

        m_minf = 1;
        insert(key, 1, value);
    }
};

}  // namespace cts