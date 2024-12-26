#pragma once
#include <iostream>
#include <cstring> // For memset

#include <shared_mutex>

//not great, but for now necessary
#include <stdexcept>
#include <utility>


namespace AE::Core {
    template <typename Key, typename Value, size_t Capacity>
    class AEHashMap {
    public:
        AEHashMap() : count(0) {
            std::memset(markers, EMPTY, sizeof(markers));
        }

        bool insert(const Key& key, const Value& value) {
            std::unique_lock lock(mutex);

            if (count >= Capacity) return false;

            size_t index = Hash(key) % Capacity;
            for (size_t i = 0; i < Capacity; i++) {
                size_t probeIndex = (index + i) % Capacity;

                if (markers[probeIndex] == EMPTY || markers[probeIndex] == TOMBSTONE) {
                    buckets[probeIndex] = { key, value };
                    markers[probeIndex] = OCCUPIED;
                    count++;
                    return true;
                }

                if (markers[probeIndex] == OCCUPIED && buckets[probeIndex].key == key) {
                    return false; 
                }
            }

            return false;   //full
        }

        bool get(const Key& key, Value& out) const {
            std::shared_lock lock(mutex);

            size_t index = Hash(key) % Capacity;
            for (size_t i = 0; i < Capacity; i++) {
                size_t probeIndex = (index + i) % Capacity;

                if (markers[probeIndex] == EMPTY) {
                    return false;
                }

                if (markers[probeIndex] == OCCUPIED && buckets[probeIndex].key == key) {
                    out = buckets[probeIndex].value;
                    return true;
                }
            }

            return false;
        }

        void erase(const Key& key) {
            std::unique_lock lock(mutex);

            size_t index = Hash(key) % Capacity;
            for (size_t i = 0; i < Capacity; i++) {
                size_t probeIndex = (index + i) % Capacity;

                if (markers[probeIndex] == EMPTY) {
                    return; // Key not found
                }

                if (markers[probeIndex] == OCCUPIED && buckets[probeIndex].key == key) {
                    markers[probeIndex] = TOMBSTONE;
                    count--;
                    return;
                }
            }
        }

        Value& operator[](const Key& key) {
            std::unique_lock lock(mutex);

            size_t index = Hash(key) % Capacity;
            for (size_t i = 0; i < Capacity; i++) {
                size_t probeIndex = (index + i) % Capacity;

                if (markers[probeIndex] == OCCUPIED && buckets[probeIndex].key == key) {
                    return buckets[probeIndex].value;
                }

                if (markers[probeIndex] == EMPTY || markers[probeIndex] == TOMBSTONE) {
                    buckets[probeIndex] = { key, Value{} };
                    markers[probeIndex] = OCCUPIED;
                    count++;
                    return buckets[probeIndex].value;
                }
            }

            throw std::runtime_error("Hashmap is full");
        }

        bool contains(const Key& key) {
            std::shared_lock lock(mutex);
            size_t index = Hash(key) % Capacity;
            for (size_t i = 0; i < Capacity; i++) {
                size_t probeIndex = (index + i) % Capacity;

                if (markers[probeIndex] == EMPTY) {
                    return false;
                }

                if (markers[probeIndex] == OCCUPIED && buckets[probeIndex].key == key) {
                    return true;
                }
            }

            return false;
        }

        size_t size() const {
            std::shared_lock lock(mutex);
            return count;
        }

        class Iterator {
            public:
                Iterator(const AEHashMap& map, size_t position)
                    : map(map), position(position) {
                    advanceToNextOccupied();
                }

                bool operator!=(const Iterator& other) const {
                    return position != other.position;
                }

                void operator++() {
                    position++;
                    advanceToNextOccupied();
                }

                std::pair<Key, Value> operator*() const {
                    const Bucket& bucket = map.buckets[position];
                    return std::pair<Key, Value>(bucket.key, bucket.value);
                }

            private:
                const AEHashMap& map;
                size_t position;

                void advanceToNextOccupied() {
                    while (position < Capacity && map.markers[position] != OCCUPIED) {
                        position++;
                    }
                }
        };

        Iterator begin() const {
            return Iterator(*this, 0);
        }

        Iterator end() const {
            return Iterator(*this, Capacity);
        }


    private:
        enum Marker : AE::Core::uint8 {
            EMPTY = 0,
            OCCUPIED = 1,
            TOMBSTONE = 2
        };

        struct Bucket {
            Key key;
            Value value;
        };

        Bucket buckets[Capacity];
        Marker markers[Capacity];
        size_t count;
        std::shared_mutex mutex;

        static size_t Hash(const Key& key) {
            std::hash<Key> hasher;
            return hasher(key);
        }
    };
}
