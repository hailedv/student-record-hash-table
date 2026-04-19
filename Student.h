/**
 * @file Student.h
 * @brief Student data structure — holds one student record.
 */
#ifndef STUDENT_H
#define STUDENT_H

#include <string>

struct Student {
    int id;
    std::string name;
    std::string department;
    double cgpa;
    int year;

    Student() : id(0), name(""), department(""), cgpa(0.0), year(0) {}
    Student(int i, std::string n, std::string d, double c, int y)
        : id(i), name(n), department(d), cgpa(c), year(y) {}
};

#endif
