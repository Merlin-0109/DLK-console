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

// Constructor - Khởi tạo hash table với kích thước cho trước
template<typename K, typename V>
HashTable<K,V>::HashTable(size_t size): tableSize(size), capacity(0) {
    table = new HashNode*[tableSize];
    for(size_t i = 0; i < tableSize; i++) {
        table[i] = nullptr;
    }
}

// Destructor - Giải phóng bộ nhớ
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

// Insert - Thêm hoặc cập nhật cặp key-value
// Time Complexity: O(1) average case, O(n) worst case (khi có nhiều collision)
template<typename K, typename V>
void HashTable<K,V>::insert(const K& key, const V& value) {
    size_t index = hash(key);
    HashNode* current = table[index];
    
    // Tìm xem key đã tồn tại chưa, nếu có thì cập nhật value
    while(current != nullptr) {
        if(current->key == key) {
            current->value = value;  // Cập nhật giá trị
            return;
        }
        current = current->next;
    }
    
    // Key chưa tồn tại, thêm node mới vào đầu danh sách
    HashNode* newNode = new HashNode(key, value);
    newNode->next = table[index];
    table[index] = newNode;
    capacity++;
}

// Remove - Xóa một cặp key-value
// Time Complexity: O(1) average case, O(n) worst case
template<typename K, typename V>
bool HashTable<K,V>::remove(const K& key) {
    size_t index = hash(key);
    HashNode* current = table[index];
    HashNode* prev = nullptr;
    
    while(current != nullptr) {
        if(current->key == key) {
            // Tìm thấy node cần xóa
            if(prev == nullptr) {
                // Node cần xóa ở đầu danh sách
                table[index] = current->next;
            } else {
                // Node cần xóa ở giữa hoặc cuối danh sách
                prev->next = current->next;
            }
            delete current;
            capacity--;
            return true;
        }
        prev = current;
        current = current->next;
    }
    
    return false;  // Không tìm thấy key
}

// Find - Tìm kiếm value theo key
// Time Complexity: O(1) average case, O(n) worst case
template<typename K, typename V>
bool HashTable<K,V>::find(const K& key, V& value) const {
    size_t index = hash(key);
    HashNode* current = table[index];
    
    while(current != nullptr) {
        if(current->key == key) {
            value = current->value;  // Gán giá trị tìm được vào tham số value
            return true;
        }
        current = current->next;
    }
    
    return false;  // Không tìm thấy key
}

// Hash Function - Tính toán index từ key
// Time Complexity: O(1)
template<typename K, typename V>
size_t HashTable<K,V>::hash(const K& key) const {
    return std::hash<K>()(key) % tableSize;
}
