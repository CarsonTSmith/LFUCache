LFUCache - C++11 Implementation

A Least Frequently Used (LFU) Cache implemented in C++11 with O(1) runtime for all operations (get() and put()).

⚡ Key Features

    O(1) time complexity for get() and put() operations.

    Uses unordered_map and linked lists for efficiency.

    Implements LFU eviction policy (removes the least frequently used item).

    Tiebreaker: If multiple keys have the same lowest frequency, the Least Recently Used (LRU) key is evicted.

    Not thread-safe – must be synchronized externally if used in a multithreaded environment.

    Written in C++11, ensuring broad compatibility.
