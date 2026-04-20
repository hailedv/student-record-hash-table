#include "LinearSearch.h"

LinearSearch::LinearSearch(int s) {
    size = s;
    count = 0;
    array = new Student[size];
}

LinearSearch::~LinearSearch() {
    delete[] array;
}

void LinearSearch::insert(int index, Student studentData) {
    if (index >= 0 && index < size) {
        array[index] = studentData;
        count++;
    }
}

// Manual linear search — O(n)
Student* LinearSearch::search(int key) {
    for (int i = 0; i < count; i++) {
        if (array[i].id == key)
            return &array[i];
    }
    return nullptr;
}

Student* LinearSearch::searchByName(const std::string& name) {
    for (int i = 0; i < count; i++) {
        if (array[i].name == name)
            return &array[i];
    }
    return nullptr;
}

int LinearSearch::getSize() { return size; }
int LinearSearch::getCount() { return count; }
