#include "lfucache.h"

#include <cassert>
#include <iostream>
#include <string>

using namespace cts;

// Basic put and get test
void testBasicPutGet() {
    LFUCache<int, std::string> cache(2);
    cache.put(1, "one");
    cache.put(2, "two");

    assert(*cache.get(1) == "one");
    assert(*cache.get(2) == "two");
}

// Eviction test (LFU behavior)
void testEvictionLFU() {
    LFUCache<int, std::string> cache(2);

    cache.put(1, "one");
    cache.put(2, "two");
    cache.get(1);  // Increase frequency of key 1
    cache.put(3, "three");  // Key 2 should be evicted (LFU)

    assert(cache.get(2) == nullptr);
    assert(*cache.get(1) == "one");
    assert(*cache.get(3) == "three");
}

// Eviction test with tie-breaking using LRU
void testEvictionLRUOnTie() {
    LFUCache<int, std::string> cache(2);

    cache.put(1, "one");
    cache.put(2, "two");
    cache.get(1);  // Increase frequency of key 1
    cache.get(2);  // Increase frequency of key 2 (both have freq=2)
    cache.put(3, "three");  // Key 1 should be evicted (LRU tie-break)

    assert(cache.get(1) == nullptr);
    assert(*cache.get(2) == "two");
    assert(*cache.get(3) == "three");
}

// Capacity Zero (No elements should be stored)
void testZeroCapacity() {
    LFUCache<int, std::string> cache(0);
    cache.put(1, "one");

    assert(cache.get(1) == nullptr);
}

// Update existing value and check frequency update
void testUpdateValue() {
    LFUCache<int, std::string> cache(2);
    cache.put(1, "one");
    cache.put(2, "two");

    cache.put(1, "ONE");  // Update value of key 1
    assert(*cache.get(1) == "ONE");
}

int main() {
    // Run tests
    testBasicPutGet();
    testEvictionLFU();
    testEvictionLRUOnTie();
    testZeroCapacity();
    testUpdateValue();

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
