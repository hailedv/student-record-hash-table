/*
 * ================================================================
 *  AKSUM UNIVERSITY — STUDENT RECORD MANAGEMENT SYSTEM
 *  Department of Computer Science & Software Engineering
 *  Course   : Design and Analysis of Algorithms
 *  Topic    : Hashing with Collision Handling (Chaining)
 *  Hash Fn  : h(key) = key mod m  (Division Method)
 *  Dataset  : 1,000 Aksum University student records
 *  Language : C++11
 *  Authors  : Haile Gebremichael | Abel Tkabo | Lwam Tesfu
 * ================================================================
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include "HashTable.h"
#include "LinearSearch.h"
#include "DataGenerator.h"

using namespace std;
using namespace chrono;

// ── ANSI Color Codes ─────────────────────────────────────────
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define BGREEN  "\033[1;32m"
#define BYELLOW "\033[1;33m"
#define BCYAN   "\033[1;36m"
#define BBLUE   "\033[1;34m"
#define BWHITE  "\033[1;37m"

// ── UI Helpers ───────────────────────────────────────────────
void printLine(char c = '=', int w = 66) {
    cout << "  " << string(w, c) << "\n";
}

void printHeader() {
    cout << "\n";
    printLine('=');
    cout << BBLUE;
    cout << "  ||                                                              ||\n";
    cout << "  ||        AKSUM UNIVERSITY — STUDENT RECORD SYSTEM             ||\n";
    cout << "  ||        Department of CS & Software Engineering              ||\n";
    cout << "  ||        Design and Analysis of Algorithms  |  2026           ||\n";
    cout << "  ||                                                              ||\n";
    cout << RESET;
    printLine('=');
    cout << CYAN << "  Hash Function : h(key) = key mod m  (Division Method)\n";
    cout << "  Collision Res.: Chaining — Array of Linked Lists\n" << RESET;
    printLine('-');
}

void printSectionTitle(const string& title) {
    cout << "\n";
    printLine('-');
    cout << BYELLOW << "  >> " << title << RESET << "\n";
    printLine('-');
}

void printSuccess(const string& msg) { cout << BGREEN  << "  [OK] " << RESET << msg << "\n"; }
void printError  (const string& msg) { cout << RED     << "  [ERR] "<< RESET << msg << "\n"; }
void printInfo   (const string& msg) { cout << CYAN    << "  [i]  " << RESET << msg << "\n"; }
void printWarn   (const string& msg) { cout << BYELLOW << "  [!]  " << RESET << msg << "\n"; }

// ── Prime Helpers ────────────────────────────────────────────
bool isPrime(int n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (int i = 5; i * i <= n; i += 6)
        if (n % i == 0 || n % (i + 2) == 0) return false;
    return true;
}
int getNextPrime(int n) { while (!isPrime(n)) n++; return n; }

// ── CSV Loaders ──────────────────────────────────────────────
int loadFromCSV(const string& filename, HashTable& ht, LinearSearch& ls) {
    ifstream file(filename);
    if (!file.is_open()) { printError("Cannot open: " + filename); return 0; }
    string line; getline(file, line);
    int index = 0;
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string idStr, name, dept, cgpaStr, yearStr;
        getline(ss,idStr,','); getline(ss,name,',');
        getline(ss,dept,','); getline(ss,cgpaStr,','); getline(ss,yearStr,',');
        int id = stoi(idStr);
        double cgpa = stod(cgpaStr);
        int year = yearStr.empty() ? 1 : stoi(yearStr);
        Student s(id, name, dept, cgpa, year);
        ht.insert(id, s);
        if (index < ls.getSize()) ls.insert(index, s);
        index++;
    }
    file.close();
    return index;
}

int readKeysFromCSV(const string& filename, int* keys, int maxCount) {
    ifstream file(filename);
    if (!file.is_open()) return 0;
    string line; getline(file, line);
    int count = 0;
    while (getline(file, line) && count < maxCount) {
        if (line.empty()) continue;
        stringstream ss(line); string idStr;
        getline(ss, idStr, ',');
        keys[count++] = stoi(idStr);
    }
    file.close();
    return count;
}

// ── Input Validation ─────────────────────────────────────────
int readValidatedID() {
    int id;
    while (true) {
        cin >> id; cin.ignore();
        if (cin.fail() || id < 10000000 || id > 999999999) {
            cin.clear();
            printWarn("Invalid ID. Must be 8-9 digits. Try again: ");
        } else break;
    }
    return id;
}

double readValidatedCGPA() {
    double cgpa;
    while (true) {
        cin >> cgpa; cin.ignore();
        if (cin.fail() || cgpa < 2.0 || cgpa > 4.0) {
            cin.clear();
            printWarn("CGPA must be 2.00 – 4.00. Try again: ");
        } else break;
    }
    return cgpa;
}

int readValidatedYear() {
    int year;
    while (true) {
        cin >> year; cin.ignore();
        if (cin.fail() || year < 1 || year > 5) {
            cin.clear();
            printWarn("Year must be 1–5. Try again: ");
        } else break;
    }
    return year;
}

// ── Manual Student Entry ─────────────────────────────────────
int inputStudentsFromUser(const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) { printError("Cannot create file."); return 0; }
    file << "StudentID,Name,Department,CGPA,Year\n";

    const string depts[] = {
        "Computer Science",       "Electrical Engineering",
        "Mechanical Engineering", "Civil Engineering",
        "Business Administration","Mathematics",
        "Physics",                "Software Engineering"
    };

    int n;
    cout << BCYAN << "  How many students to enter? " << RESET;
    cin >> n; cin.ignore();
    if (n <= 0) { printWarn("Nothing to enter."); file.close(); return 0; }

    printInfo("Aksum University name examples:");
    printInfo("  Abebe Tadesse | Tigist Bekele | Haben Gebremichael | Kibrom Tsegay");

    int* enteredIDs = new int[n]();
    int enteredCount = 0;

    for (int i = 0; i < n; i++) {
        cout << "\n" << BYELLOW;
        printLine('-');
        cout << "  Student " << (i+1) << " of " << n << "\n";
        printLine('-');
        cout << RESET;

        int id;
        while (true) {
            cout << CYAN << "  Student ID (8-9 digits) : " << RESET;
            id = readValidatedID();
            bool dup = false;
            for (int k = 0; k < enteredCount; k++)
                if (enteredIDs[k] == id) { dup = true; break; }
            if (dup) printWarn("Duplicate ID. Enter a unique ID.");
            else break;
        }
        enteredIDs[enteredCount++] = id;

        string name;
        cout << CYAN << "  Full Name                : " << RESET;
        getline(cin, name);
        if (name.empty()) name = "Unknown";

        cout << CYAN << "  Department:\n" << RESET;
        for (int d = 0; d < 8; d++)
            cout << "    " << BYELLOW << (d+1) << RESET << ". " << depts[d] << "\n";
        cout << CYAN << "  Choose (1-8): " << RESET;
        int dc; cin >> dc; cin.ignore();
        string dept = (dc >= 1 && dc <= 8) ? depts[dc-1] : "Unknown";

        cout << CYAN << "  CGPA (2.00-4.00)         : " << RESET;
        double cgpa = readValidatedCGPA();

        cout << CYAN << "  Year (1-5)               : " << RESET;
        int year = readValidatedYear();

        file << id << "," << name << "," << dept << "," << cgpa << "," << year << "\n";
        printSuccess("Student " + name + " added.");
    }
    delete[] enteredIDs;
    file.close();
    return n;
}

// ── Display Student Row ───────────────────────────────────────
void printStudentRow(const Student* s) {
    cout << "  " << BGREEN << left
         << setw(12) << s->id
         << RESET << setw(22) << s->name
         << setw(26) << s->department
         << BYELLOW << fixed << setprecision(2) << setw(7) << s->cgpa << RESET
         << "  Year " << s->year << "\n";
}

void printTableHeader() {
    printLine('-');
    cout << "  " << BWHITE << left
         << setw(12) << "ID"
         << setw(22) << "Name"
         << setw(26) << "Department"
         << setw(7)  << "CGPA"
         << "  Year" << RESET << "\n";
    printLine('-');
}

// ── Performance Test ─────────────────────────────────────────
void performanceTest(HashTable& ht, LinearSearch& ls,
                     int* testKeys, int numTests, const string& label) {
    cout << "\n  " << BCYAN << label << RESET << "\n";
    printLine('-', 62);

    auto t1 = high_resolution_clock::now();
    int htFound = 0;
    for (int i = 0; i < numTests; i++)
        if (ht.search(testKeys[i]) != nullptr) htFound++;
    auto t2 = high_resolution_clock::now();
    long long htTime = duration_cast<microseconds>(t2 - t1).count();

    t1 = high_resolution_clock::now();
    int lsFound = 0;
    for (int i = 0; i < numTests; i++)
        if (ls.search(testKeys[i]) != nullptr) lsFound++;
    t2 = high_resolution_clock::now();
    long long lsTime = duration_cast<microseconds>(t2 - t1).count();

    double speedup = htTime > 0 ? (double)lsTime / htTime : 0;

    cout << fixed << setprecision(4);
    cout << "  " << left << setw(20) << "Structure"
         << setw(12) << "Time(us)"
         << setw(10) << "Found"
         << "Avg/search\n";
    printLine('-', 62);
    cout << "  " << BGREEN << setw(20) << "Hash Table" << RESET
         << setw(12) << htTime
         << setw(10) << htFound
         << (double)htTime / numTests << " us\n";
    cout << "  " << setw(20) << "Linear Search"
         << setw(12) << lsTime
         << setw(10) << lsFound
         << (double)lsTime / numTests << " us\n";
    printLine('-', 62);
    if (htTime > 0)
        cout << "  " << BYELLOW << "Speedup: " << fixed << setprecision(1)
             << speedup << "x faster" << RESET << "\n";
    else
        cout << "  " << BYELLOW << "Speedup: Hash table sub-microsecond" << RESET << "\n";
}

// ── Worst Case Simulation ────────────────────────────────────
void worstCaseSimulation(int tableSize, int numTests) {
    cout << "\n  " << BCYAN << "True Worst Case — All " << numTests
         << " keys forced to slot 0" << RESET << "\n";
    printLine('-', 62);

    HashTable    ht(tableSize);
    LinearSearch ls(numTests);
    int* keys = new int[numTests];

    for (int i = 0; i < numTests; i++) {
        int key = (i + 1) * tableSize;
        keys[i] = key;
        Student s(key, "Test Student", "Computer Science", 3.0, 1);
        ht.insert(key, s);
        ls.insert(i, s);
    }
    int* searchKeys = new int[numTests];
    for (int i = 0; i < numTests; i++) searchKeys[i] = keys[numTests-1-i];

    auto t1 = high_resolution_clock::now();
    int htFound = 0;
    for (int i = 0; i < numTests; i++)
        if (ht.search(searchKeys[i]) != nullptr) htFound++;
    auto t2 = high_resolution_clock::now();
    long long htTime = duration_cast<microseconds>(t2-t1).count();

    t1 = high_resolution_clock::now();
    int lsFound = 0;
    for (int i = 0; i < numTests; i++)
        if (ls.search(searchKeys[i]) != nullptr) lsFound++;
    t2 = high_resolution_clock::now();
    long long lsTime = duration_cast<microseconds>(t2-t1).count();

    cout << "  Chain length = " << numTests << " (all keys in one slot)\n";
    cout << "  " << BGREEN << "Hash Table    : " << RESET << htTime << " us  | found: " << htFound << "\n";
    cout << "  Linear Search : " << lsTime << " us  | found: " << lsFound << "\n";
    printWarn("HT degrades to O(n) when all keys collide — chain = n");

    delete[] keys; delete[] searchKeys;
}

// ── Load Factor Experiment ───────────────────────────────────
void loadFactorExperiment(const string& filename, int totalRecords) {
    printSectionTitle("LOAD FACTOR DEGRADATION EXPERIMENT  —  Theta(1 + alpha)");
    cout << "  " << BWHITE << left
         << setw(8)  << "n"
         << setw(8)  << "m"
         << setw(10) << "alpha"
         << setw(12) << "HT (us)"
         << setw(12) << "LS (us)"
         << setw(10) << "Speedup"
         << "Complexity" << RESET << "\n";
    printLine('-', 62);

    int steps[] = {100, 200, 400, 600, 800, 1000};
    for (int s = 0; s < 6; s++) {
        int n = steps[s]; if (n > totalRecords) n = totalRecords;
        int tSize = getNextPrime(n / 2 + 1);
        HashTable ht(tSize); LinearSearch ls(n);

        ifstream file(filename); string line; getline(file, line);
        int idx = 0;
        while (getline(file, line) && idx < n) {
            if (line.empty()) continue;
            stringstream ss(line);
            string idStr, name, dept, cgpaStr, yearStr;
            getline(ss,idStr,','); getline(ss,name,',');
            getline(ss,dept,','); getline(ss,cgpaStr,','); getline(ss,yearStr,',');
            int id=stoi(idStr); double cgpa=stod(cgpaStr);
            int year=yearStr.empty()?1:stoi(yearStr);
            Student st(id,name,dept,cgpa,year);
            ht.insert(id,st); ls.insert(idx++,st);
        }
        file.close();

        int* keys = new int[n];
        int kc = readKeysFromCSV(filename, keys, n);
        int numTests = (kc < 50) ? kc : 50;
        srand((unsigned int)time(0));

        auto t1 = high_resolution_clock::now();
        for (int i = 0; i < numTests; i++) ht.search(keys[rand()%n]);
        auto t2 = high_resolution_clock::now();
        long long htTime = duration_cast<microseconds>(t2-t1).count();

        t1 = high_resolution_clock::now();
        for (int i = 0; i < numTests; i++) ls.search(keys[rand()%n]);
        t2 = high_resolution_clock::now();
        long long lsTime = duration_cast<microseconds>(t2-t1).count();

        double alpha   = ht.getLoadFactor();
        double speedup = htTime > 0 ? (double)lsTime/htTime : 0;

        cout << fixed << setprecision(3) << left
             << "  " << BGREEN << setw(8) << n << RESET
             << setw(8)  << tSize
             << setw(10) << alpha
             << BYELLOW << setw(12) << htTime << RESET
             << setw(12) << lsTime
             << BCYAN << setw(10) << speedup << RESET
             << "Theta(" << 1+alpha << ")\n";
        delete[] keys;
    }
    printLine('-', 62);
    printInfo("As n grows, HT time stays ~constant. Linear search grows O(n).");
}

// ── Math Analysis ────────────────────────────────────────────
void printMathAnalysis(double alpha, int n, int m) {
    printSectionTitle("MATHEMATICAL ANALYSIS  —  Theta(1 + alpha)");
    cout << "  Hash Function  : " << BCYAN << "h(key) = key mod m" << RESET << "  (Division Method)\n";
    cout << "  Collision Res. : Chaining (array of linked lists)\n";
    cout << "  n = " << BGREEN << n << RESET << " elements,  m = " << BGREEN << m << RESET << " slots\n";
    cout << "  Load factor    : " << BYELLOW << "alpha = " << n << "/" << m
         << " = " << fixed << setprecision(4) << alpha << RESET << "\n\n";
    cout << "  THEOREM (CLRS Ch.11 — Simple Uniform Hashing + Chaining):\n";
    printLine('-', 62);
    cout << "  Unsuccessful search : Theta(1 + alpha)\n";
    cout << "    O(1) hash compute + traverse full chain (avg len = alpha)\n\n";
    cout << "  Successful search   : Theta(1 + alpha)\n";
    cout << "    O(1) hash compute + traverse ~half chain on average\n\n";
    cout << "  With alpha = " << BYELLOW << fixed << setprecision(4) << alpha << RESET << ":\n";
    cout << "    Expected comparisons = 1 + " << alpha
         << " = " << BGREEN << 1 + alpha << RESET << "\n\n";
    cout << "  If n = O(m)  =>  alpha = O(1)  =>  all ops are " << BGREEN << "O(1) avg\n" << RESET;
    cout << "  Linear search is always " << RED << "Theta(n)" << RESET << " regardless of input.\n";
}

// ── Menu ─────────────────────────────────────────────────────
void printMenu() {
    cout << "\n";
    printLine('=');
    cout << BBLUE << "  ||          AKSUM UNIVERSITY — MAIN MENU                        ||\n" << RESET;
    printLine('=');
    cout << BCYAN  << "  [1]" << RESET << "  Search student by ID\n";
    cout << BCYAN  << "  [2]" << RESET << "  Search student by Name\n";
    cout << BCYAN  << "  [3]" << RESET << "  Insert new student record\n";
    cout << BCYAN  << "  [4]" << RESET << "  Update student record\n";
    cout << BCYAN  << "  [5]" << RESET << "  Delete student record\n";
    printLine('-', 40);
    cout << BYELLOW << "  [6]" << RESET << "  Display hash table (first 20 slots)\n";
    cout << BYELLOW << "  [7]" << RESET << "  Display all students — sorted by CGPA\n";
    cout << BYELLOW << "  [8]" << RESET << "  Display all students — sorted by ID\n";
    printLine('-', 40);
    cout << MAGENTA << "  [9]" << RESET << "  Statistics, analysis & collision histogram\n";
    cout << MAGENTA << "  [10]"<< RESET << " Export records / Rehash table\n";
    printLine('-', 40);
    cout << RED    << "  [0]" << RESET << "  Exit\n";
    printLine('=');
    cout << BWHITE << "  Choose: " << RESET;
}

// ── MAIN ─────────────────────────────────────────────────────
int main() {
    const string filename = "students.csv";
    const int MAX_RECORDS = 1000;

    printHeader();

    cout << "\n" << BWHITE << "  Select Data Source:\n" << RESET;
    cout << BCYAN << "  [1]" << RESET << " Enter students manually\n";
    cout << BCYAN << "  [2]" << RESET << " Auto-generate 1,000 Aksum University student records\n";
    cout << BWHITE << "  Choose (1 or 2): " << RESET;
    int choice; cin >> choice; cin.ignore();

    int NUM_RECORDS = 0;
    if (choice == 1) {
        printInfo("Enter Aksum University student names.");
        printInfo("Examples: Abebe Tadesse | Tigist Bekele | Haben Gebremichael");
        NUM_RECORDS = inputStudentsFromUser(filename);
    } else {
        printInfo("Generating 1,000 Aksum University student records...");
        DataGenerator::generateStudentData(filename, MAX_RECORDS);
        NUM_RECORDS = MAX_RECORDS;
        printSuccess("Dataset saved to '" + filename + "'");
        printInfo("Names   : Ethiopian/Tigrinya (Abebe, Tigist, Haben, Kibrom, Teklay...)");
        printInfo("Fields  : StudentID | Name | Department | CGPA | Year");
    }

    if (NUM_RECORDS == 0) { printError("No records to process."); return 0; }

    int tableSize = getNextPrime(NUM_RECORDS + 1);
    cout << "\n";
    printInfo("Hash Table Size (prime m) : " + to_string(tableSize));
    printInfo("Number of records   (n)   : " + to_string(NUM_RECORDS));

    HashTable    hashTable(tableSize);
    LinearSearch linearSearch(NUM_RECORDS);

    printInfo("Loading records into Hash Table and Linear Search array...");
    int loaded = loadFromCSV(filename, hashTable, linearSearch);
    printSuccess("Loaded " + to_string(loaded) + " student records successfully.");

    hashTable.printLoadFactorAnalysis();
    printMathAnalysis(hashTable.getLoadFactor(), loaded, tableSize);

    int* allKeys  = new int[NUM_RECORDS];
    int  keyCount = readKeysFromCSV(filename, allKeys, NUM_RECORDS);
    int  numTests = (keyCount < 100) ? keyCount : 100;
    int* testKeys = new int[numTests];

    printSectionTitle("PERFORMANCE COMPARISON — Hash Table vs Linear Search (" + to_string(numTests) + " searches)");

    for (int i = 0; i < numTests; i++) testKeys[i] = allKeys[i];
    performanceTest(hashTable, linearSearch, testKeys, numTests,
                    "Best Case   — keys at front of chain");

    srand((unsigned int)time(0));
    for (int i = 0; i < numTests; i++) testKeys[i] = allKeys[rand() % keyCount];
    performanceTest(hashTable, linearSearch, testKeys, numTests,
                    "Average Case — random existing keys");

    for (int i = 0; i < numTests; i++) testKeys[i] = 99999999 - i;
    performanceTest(hashTable, linearSearch, testKeys, numTests,
                    "Worst Case  — keys NOT in table");

    worstCaseSimulation(tableSize, numTests);
    loadFactorExperiment(filename, NUM_RECORDS);

    // ── Interactive Menu ──────────────────────────────────────
    int menuChoice = -1;
    while (menuChoice != 0) {
        printMenu();
        cin >> menuChoice; cin.ignore();

        const string depts[] = {
            "Computer Science",       "Electrical Engineering",
            "Mechanical Engineering", "Civil Engineering",
            "Business Administration","Mathematics",
            "Physics",                "Software Engineering"
        };

        if (menuChoice == 1) {
            cout << CYAN << "  Enter Student ID: " << RESET;
            int id = readValidatedID();
            Student* found = hashTable.search(id);
            if (found) {
                printSuccess("Student found:");
                printTableHeader();
                printStudentRow(found);
                printLine('-');
            } else {
                printWarn("Student ID " + to_string(id) + " not found.");
            }

        } else if (menuChoice == 2) {
            string name;
            cout << CYAN << "  Enter student name: " << RESET; getline(cin, name);
            Student* found = hashTable.searchByName(name);
            if (found) {
                printSuccess("Student found:");
                printTableHeader();
                printStudentRow(found);
                printLine('-');
            } else {
                printWarn("No student named '" + name + "' found.");
            }

        } else if (menuChoice == 3) {
            cout << CYAN << "  New Student ID (8-9 digits): " << RESET;
            int id = readValidatedID();
            if (hashTable.search(id)) {
                printWarn("ID " + to_string(id) + " already exists. Use Update [4] instead.");
            } else {
                string name, dept; double cgpa; int year;
                cout << CYAN << "  Full Name       : " << RESET; getline(cin, name);
                if (name.empty()) name = "Unknown";
                cout << CYAN << "  Department:\n" << RESET;
                for (int d = 0; d < 8; d++)
                    cout << "    " << BYELLOW << (d+1) << RESET << ". " << depts[d] << "\n";
                cout << CYAN << "  Choose (1-8): " << RESET;
                int dc; cin >> dc; cin.ignore();
                dept = (dc >= 1 && dc <= 8) ? depts[dc-1] : "Unknown";
                cout << CYAN << "  CGPA (2.00-4.00): " << RESET; cgpa = readValidatedCGPA();
                cout << CYAN << "  Year (1-5)      : " << RESET; year = readValidatedYear();
                hashTable.insert(id, Student(id, name, dept, cgpa, year));
                printSuccess("Student '" + name + "' inserted. Load factor: "
                    + to_string(hashTable.getLoadFactor()).substr(0,6));
            }

        } else if (menuChoice == 4) {
            cout << CYAN << "  Student ID to update: " << RESET;
            int id = readValidatedID();
            Student* existing = hashTable.search(id);
            if (!existing) {
                printWarn("Student ID " + to_string(id) + " not found.");
            } else {
                cout << "  Current record:\n";
                printTableHeader(); printStudentRow(existing); printLine('-');
                string name, dept; double cgpa; int year;
                cout << CYAN << "  New Name (Enter to keep): " << RESET; getline(cin, name);
                if (name.empty()) name = existing->name;
                cout << CYAN << "  Department:\n" << RESET;
                for (int d = 0; d < 8; d++)
                    cout << "    " << BYELLOW << (d+1) << RESET << ". " << depts[d] << "\n";
                cout << CYAN << "  Choose (1-8, 0=keep): " << RESET;
                int dc; cin >> dc; cin.ignore();
                dept = (dc >= 1 && dc <= 8) ? depts[dc-1] : existing->department;
                cout << CYAN << "  New CGPA (2.00-4.00): " << RESET; cgpa = readValidatedCGPA();
                cout << CYAN << "  New Year (1-5)      : " << RESET; year = readValidatedYear();
                hashTable.updateRecord(id, Student(id, name, dept, cgpa, year));
                printSuccess("Record updated successfully.");
            }

        } else if (menuChoice == 5) {
            cout << CYAN << "  Student ID to delete: " << RESET;
            int id = readValidatedID();
            if (hashTable.deleteRecord(id))
                printSuccess("Record deleted. Load factor: "
                    + to_string(hashTable.getLoadFactor()).substr(0,6));
            else
                printWarn("Student ID " + to_string(id) + " not found.");

        } else if (menuChoice == 6) {
            hashTable.display();

        } else if (menuChoice == 7) {
            hashTable.displaySortedByCGPA();

        } else if (menuChoice == 8) {
            hashTable.displaySortedByID();

        } else if (menuChoice == 9) {
            hashTable.printLoadFactorAnalysis();
            hashTable.printCollisionHistogram();
            printMathAnalysis(hashTable.getLoadFactor(),
                              hashTable.getNumElements(),
                              hashTable.getTableSize());

        } else if (menuChoice == 10) {
            cout << "\n" << BCYAN << "  [1]" << RESET << " Export records to CSV\n";
            cout << BCYAN << "  [2]" << RESET << " Rehash table to new size\n";
            cout << BWHITE << "  Choose: " << RESET;
            int sub; cin >> sub; cin.ignore();
            if (sub == 1) {
                cout << CYAN << "  Output filename (e.g. export.csv): " << RESET;
                string fname; getline(cin, fname);
                if (fname.empty()) fname = "export.csv";
                hashTable.exportToFile(fname);
            } else if (sub == 2) {
                cout << CYAN << "  New table size: " << RESET;
                int ns; cin >> ns; cin.ignore();
                if (ns > 0) hashTable.rehash(getNextPrime(ns));
                else printError("Invalid size.");
            }
            menuChoice = -1;

        } else if (menuChoice != 0) {
            printWarn("Invalid choice. Please select from the menu.");
        }
    }

    delete[] allKeys;
    delete[] testKeys;

    cout << "\n";
    printLine('=');
    cout << BGREEN << "  ||     Thank you for using Aksum University Record System      ||\n" << RESET;
    cout << CYAN   << "  ||     Haile Gebremichael | Abel Tkabo | Lwam Tesfu            ||\n" << RESET;
    printLine('=');
    cout << "\n";
    return 0;
}
