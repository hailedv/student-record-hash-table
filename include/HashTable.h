#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "Node.h"
#include <iostream>

/**
 * HashTable — chaining-based hash table for Student records.
 *
 * Hash function : h(key) = key mod tableSize  (Division Method)
 * Collision res.: Singly linked list per slot (chaining)
 * Complexity    : O(1) avg insert/search/delete  |  O(n) worst case
 */
class HashTable {
private:
    Node** table;       // array of linked-list heads (size = tableSize)
    int    tableSize;   // m — prime number for better key distribution
    int    numElements; // n — current number of stored elements
    int    collisionCount;

    int hashFunction(int key); // h(key) = key mod tableSize

public:
    HashTable(int size);
    ~HashTable();

    // Core CRUD operations
    void    insert(int key, Student studentData);
    Student* search(int key);
    bool    updateRecord(int key, Student newData);
    bool    deleteRecord(int key);

    // Display
    void display();                  // first 20 used slots
    void displaySortedByCGPA();      // all records, CGPA descending
    void displaySortedByID();        // all records, ID ascending

    // Search by name (O(n) full-table scan)
    Student* searchByName(const std::string& name);

    // Export / Rehash
    void exportToFile(const std::string& filename);
    void rehash(int newSize);

    // Analytics
    void   printLoadFactorAnalysis();
    void   printCollisionHistogram();
    double getLoadFactor();
    int    getCollisionCount();
    void   resetCollisionCount();
    int    getTableSize();
    int    getNumElements();
    int    getChainLength(int index);
};

#endif
