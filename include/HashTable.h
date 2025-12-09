#pragma once
#include <iostream>
#include <functional>
using namespace std;
template <typename K, typename V>
class HashTable {
    public:
        HashTable(size_t size);
        ~HashTable();

        void insert(const K& key, const V& value);
        bool remove(const K& key);
        bool find(const K& key, V& value) const;
        void rehash();

    private:
        struct HashNode {
            K key;
            V value;
            HashNode* next;
            
            HashNode(const K& k, const V& v) : key(k), value(v), next(nullptr) {}
        };

        HashNode** table;
        size_t tableSize;
        size_t capacity;
        size_t hash(const K& key) const;
};
template<typename K, typename V>
HashTable<K,V>::HashTable(size_t size): tableSize(size), capacity(0) {
    table = new HashNode*[tableSize];
    for(size_t i = 0; i < tableSize; i++) {
        table[i] = nullptr;
    }
}
template<typename K, typename V>
HashTable<K,V>::~HashTable() {
    for(size_t i = 0; i < tableSize; i++) {
        HashNode* current = table[i];
        while(current != nullptr) {
            HashNode* temp = current;
            current = current->next;
            delete temp;
        }
    }
    delete[] table;
}
template<typename K, typename V>
void HashTable<K,V>::insert(const K& key, const V& value) {
    if ((double)capacity / tableSize > 0.7) {
        rehash();  
    }
    size_t index = hash(key);
    HashNode* current = table[index];
    while(current != nullptr) {
        if(current->key == key) {
            current->value = value;  
            return;
        }
        current = current->next;
    }
    HashNode* newNode = new HashNode(key, value);
    newNode->next = table[index];
    table[index] = newNode;
    capacity++;
}
template<typename K, typename V>
bool HashTable<K,V>::remove(const K& key) {
    size_t index = hash(key);
    HashNode* current = table[index];
    HashNode* prev = nullptr;
    while(current != nullptr) {
        if(current->key == key) {
            if(prev == nullptr) {
                table[index] = current->next;
            } else {
                prev->next = current->next;
            }
            delete current;
            capacity--;
            return true;
        }
        prev = current;
        current = current->next;
    }
    return false;  
}
template<typename K, typename V>
bool HashTable<K,V>::find(const K& key, V& value) const {
    size_t index = hash(key);
    HashNode* current = table[index];
    while(current != nullptr) {
        if(current->key == key) {
            value = current->value;  
            return true;
        }
        current = current->next;
    }
    return false;
}
template<typename K, typename V>
size_t HashTable<K,V>::hash(const K& key) const {
    return std::hash<K>()(key) % tableSize;
}
template<typename K, typename V>
void HashTable<K,V>::rehash() {
    size_t oldTableSize = tableSize;
    HashNode** oldTable = table;
    tableSize *= 2;
    table = new HashNode*[tableSize]();
    capacity = 0;
    for (size_t i = 0; i < tableSize; i++) {
        table[i] = nullptr;
    }
    for (size_t i = 0; i < oldTableSize; i++) {
        HashNode* entry = oldTable[i];
        while (entry != nullptr) {
            size_t newIndex = hash(entry->key);
            HashNode* newNode = new HashNode(entry->key, entry->value);
            newNode->next = table[newIndex];
            table[newIndex] = newNode;
            capacity++;
            HashNode* prev = entry;
            entry = entry->next;
            delete prev;
        }
    }
    delete[] oldTable;
}
