#include "DataGenerator.h"
#include <cstdlib>
#include <ctime>

void DataGenerator::generateStudentData(const std::string& filename, int count) {
    std::ofstream file(filename);
    if (!file.is_open()) return;

    file << "StudentID,Name,Department,CGPA,Year\n";

    std::string departments[] = {
        "Computer Science",       "Electrical Engineering",
        "Mechanical Engine5ering", "Civil Engineering",
        "Business Administration","Mathematics",
        "Physics",                "Software Engineering"
    };

    std::string firstNames[] = {
        "Abebe",     "Tigist",    "Dawit",     "Hiwot",     "Yonas",
        "Meron",     "Biruk",     "Selam",     "Natnael",   "Bethlehem",
        "Eyob",      "Rahel",     "Henok",     "Fikirte",   "Kaleb",
        "Liya",      "Ermias",    "Tsion",     "Mikias",    "Azeb",
        "Robel",     "Kidist",    "Amanuel",   "Selamawit", "Yared"
    };

    std::string lastNames[] = {
        "Tadesse",   "Bekele",    "Haile",     "Girma",     "Tesfaye",
        "Worku",     "Alemu",     "Kebede",    "Desta",     "Mulugeta",
        "Assefa",    "Negash",    "Wolde",     "Gebre",     "Mekonnen",
        "Teshome",   "Demeke",    "Abate",     "Shiferaw",  "Lemma",
        "Getachew",  "Yilma",     "Fekadu",    "Mengistu",  "Zeleke"
    };

    srand((unsigned int)time(0));

    // Sequential IDs starting from 160186000
    for (int i = 0; i < count; i++) {
        int id = 160186000 + i;

        std::string name = firstNames[rand() % 25] + " " + lastNames[rand() % 25];
        std::string dept = departments[rand() % 8];
        double cgpa      = 2.00 + (rand() % 201) / 100.0;
        int    year      = 1 + rand() % 5;

        file << id << "," << name << "," << dept << ","
             << cgpa << "," << year << "\n";
    }

    file.close();
}