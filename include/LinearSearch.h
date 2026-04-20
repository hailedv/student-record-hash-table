/**
 * @file LinearSearch.h
 * @brief Linear search on an unsorted Student array — O(n) per search.
 *        Used as a performance baseline against the hash table.
 */
#ifndef LINEARSEARCH_H
#define LINEARSEARCH_H

#include "Student.h"
#include <string>

class LinearSearch {
private:
    Student* array;
    int size;
    int count;

public:
    LinearSearch(int s);
    ~LinearSearch();

    void insert(int index, Student studentData);
    Student* search(int key);
    Student* searchByName(const std::string& name);
    int getSize();
    int getCount();
};

#endif
