/**
 * @file Node.h
 * @brief Singly linked list node used for hash table chaining.
 */
#ifndef NODE_H
#define NODE_H

#include "Student.h"

// Node for singly linked list (chaining)
struct Node {
    int key;
    Student data;
    Node* next;

    Node(int k, Student s) : key(k), data(s), next(nullptr) {}
};

#endif
