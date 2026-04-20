/**
 * @file DataGenerator.h
 * @brief Generates synthetic Aksum University student records to a CSV file.
 */
#ifndef DATAGENERATOR_H
#define DATAGENERATOR_H

#include <string>
#include <fstream>

class DataGenerator {
public:
    // Auto-generate Aksum University student records to CSV
    static void generateStudentData(const std::string& filename, int count);
};

#endif
