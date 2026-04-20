#include "HashTable.h"
#include <iomanip>
#include <fstream>

HashTable::HashTable(int size) {
    tableSize = size;
    table = new Node*[tableSize];
    for (int i = 0; i < tableSize; i++)
        table[i] = nullptr;
    numElements = 0;
    collisionCount = 0;
}

HashTable::~HashTable() {
    for (int i = 0; i < tableSize; i++) {
        Node* current = table[i];
        while (current != nullptr) {
            Node* temp = current;
            current = current->next;
            delete temp;
        }
    }
    delete[] table;
}

// Division method hash function: h(key) = key mod tableSize
int HashTable::hashFunction(int key) {
    return key % tableSize;
}

void HashTable::insert(int key, Student studentData) {
    int index = hashFunction(key);

    // Check if key already exists — update instead of duplicate insert
    Node* current = table[index];
    while (current != nullptr) {
        if (current->key == key) {
            current->data = studentData; // update existing
            return;
        }
        current = current->next;
    }

    Node* newNode = new Node(key, studentData);
    if (table[index] == nullptr) {
        table[index] = newNode;
    } else {
        collisionCount++;
        current = table[index];
        while (current->next != nullptr)
            current = current->next;
        current->next = newNode;
    }
    numElements++;
}

Student* HashTable::search(int key) {
    int index = hashFunction(key);
    Node* current = table[index];
    while (current != nullptr) {
        if (current->key == key)
            return &(current->data);
        current = current->next;
    }
    return nullptr;
}

bool HashTable::updateRecord(int key, Student newData) {
    int index = hashFunction(key);
    Node* current = table[index];
    while (current != nullptr) {
        if (current->key == key) {
            current->data = newData;
            return true;
        }
        current = current->next;
    }
    return false;
}

Student* HashTable::searchByName(const std::string& name) {
    for (int i = 0; i < tableSize; i++) {
        Node* current = table[i];
        while (current != nullptr) {
            if (current->data.name == name)
                return &(current->data);
            current = current->next;
        }
    }
    return nullptr;
}

void HashTable::displaySortedByCGPA() {
    if (numElements == 0) { std::cout << "  No records.\n"; return; }
    // Collect all into temp array
    Student* arr = new Student[numElements];
    int idx = 0;
    for (int i = 0; i < tableSize; i++) {
        Node* cur = table[i];
        while (cur != nullptr) { arr[idx++] = cur->data; cur = cur->next; }
    }
    // Selection sort descending by CGPA
    for (int i = 0; i < idx - 1; i++) {
        int maxIdx = i;
        for (int j = i + 1; j < idx; j++)
            if (arr[j].cgpa > arr[maxIdx].cgpa) maxIdx = j;
        if (maxIdx != i) { Student tmp = arr[i]; arr[i] = arr[maxIdx]; arr[maxIdx] = tmp; }
    }
    std::cout << "\n  --------------------------------------------------------\n";
    std::cout << "  Students Sorted by CGPA (Descending)\n";
    std::cout << "  --------------------------------------------------------\n";
    std::cout << std::left << std::setw(12) << "  ID"
              << std::setw(25) << "Name"
              << std::setw(28) << "Department"
              << std::setw(8)  << "CGPA"
              << "Year\n";
    std::cout << "  " << std::string(74, '-') << "\n";
    for (int i = 0; i < idx; i++) {
        std::cout << "  " << std::left << std::setw(12) << arr[i].id
                  << std::setw(25) << arr[i].name
                  << std::setw(28) << arr[i].department
                  << std::fixed << std::setprecision(2) << std::setw(8) << arr[i].cgpa
                  << arr[i].year << "\n";
    }
    std::cout << "  " << std::string(74, '-') << "\n";
    std::cout << "  Total: " << idx << " records\n";
    delete[] arr;
}

void HashTable::displaySortedByID() {
    if (numElements == 0) { std::cout << "  No records.\n"; return; }
    Student* arr = new Student[numElements];
    int idx = 0;
    for (int i = 0; i < tableSize; i++) {
        Node* cur = table[i];
        while (cur != nullptr) { arr[idx++] = cur->data; cur = cur->next; }
    }
    // Selection sort ascending by ID
    for (int i = 0; i < idx - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < idx; j++)
            if (arr[j].id < arr[minIdx].id) minIdx = j;
        if (minIdx != i) { Student tmp = arr[i]; arr[i] = arr[minIdx]; arr[minIdx] = tmp; }
    }
    std::cout << "\n  --------------------------------------------------------\n";
    std::cout << "  Students Sorted by ID (Ascending)\n";
    std::cout << "  --------------------------------------------------------\n";
    std::cout << std::left << std::setw(12) << "  ID"
              << std::setw(25) << "Name"
              << std::setw(28) << "Department"
              << std::setw(8)  << "CGPA"
              << "Year\n";
    std::cout << "  " << std::string(74, '-') << "\n";
    for (int i = 0; i < idx; i++) {
        std::cout << "  " << std::left << std::setw(12) << arr[i].id
                  << std::setw(25) << arr[i].name
                  << std::setw(28) << arr[i].department
                  << std::fixed << std::setprecision(2) << std::setw(8) << arr[i].cgpa
                  << arr[i].year << "\n";
    }
    std::cout << "  " << std::string(74, '-') << "\n";
    std::cout << "  Total: " << idx << " records\n";
    delete[] arr;
}

void HashTable::exportToFile(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "  [ERROR] Cannot create file: " << filename << "\n";
        return;
    }
    file << "StudentID,Name,Department,CGPA,Year\n";
    for (int i = 0; i < tableSize; i++) {
        Node* cur = table[i];
        while (cur != nullptr) {
            file << cur->data.id << ","
                 << cur->data.name << ","
                 << cur->data.department << ","
                 << std::fixed << std::setprecision(2) << cur->data.cgpa << ","
                 << cur->data.year << "\n";
            cur = cur->next;
        }
    }
    file.close();
    std::cout << "  Exported " << numElements << " records to '" << filename << "'\n";
}

void HashTable::printCollisionHistogram() {
    // Find max chain length
    int maxLen = 0;
    for (int i = 0; i < tableSize; i++) {
        int len = getChainLength(i);
        if (len > maxLen) maxLen = len;
    }
    // Count slots per chain length
    int* freq = new int[maxLen + 1]();
    for (int i = 0; i < tableSize; i++)
        freq[getChainLength(i)]++;

    std::cout << "\n========================================================\n";
    std::cout << "  COLLISION CHAIN-LENGTH HISTOGRAM\n";
    std::cout << "========================================================\n";
    std::cout << std::left << std::setw(12) << "  Chain Len"
              << std::setw(10) << "Slots"
              << "Bar\n";
    std::cout << "  " << std::string(56, '-') << "\n";
    for (int len = 0; len <= maxLen; len++) {
        int barLen = freq[len] / 5;  // scale: 1 char per 5 slots
        if (freq[len] > 0 && barLen == 0) barLen = 1;
        std::cout << "  " << std::setw(10) << len
                  << std::setw(10) << freq[len]
                  << std::string(barLen, '#') << "\n";
    }
    std::cout << "  " << std::string(56, '-') << "\n";
    std::cout << "  (each # = ~5 slots)\n";
    std::cout << "========================================================\n";
    delete[] freq;
}

void HashTable::rehash(int newSize) {
    std::cout << "\n  Rehashing: " << tableSize << " -> " << newSize << " slots...\n";
    Node** oldTable = table;
    int oldSize = tableSize;

    tableSize = newSize;
    table = new Node*[tableSize];
    for (int i = 0; i < tableSize; i++) table[i] = nullptr;
    int oldElements = numElements;
    numElements = 0;
    collisionCount = 0;

    for (int i = 0; i < oldSize; i++) {
        Node* cur = oldTable[i];
        while (cur != nullptr) {
            insert(cur->key, cur->data);
            cur = cur->next;
        }
    }
    // Free old table
    for (int i = 0; i < oldSize; i++) {
        Node* cur = oldTable[i];
        while (cur != nullptr) {
            Node* tmp = cur; cur = cur->next; delete tmp;
        }
    }
    delete[] oldTable;
    std::cout << "  Rehash complete. Elements: " << oldElements
              << "  New load factor: " << std::fixed << std::setprecision(4)
              << getLoadFactor() << "\n";
}

bool HashTable::deleteRecord(int key) {
    int index = hashFunction(key);
    Node* current = table[index];
    Node* prev = nullptr;

    while (current != nullptr) {
        if (current->key == key) {
            if (prev == nullptr)
                table[index] = current->next;
            else
                prev->next = current->next;
            delete current;
            numElements--;
            return true;
        }
        prev = current;
        current = current->next;
    }
    return false;
}

void HashTable::display() {
    std::cout << "\n  --------------------------------------------------------\n";
    std::cout << "  Hash Table Contents (first 20 used slots)\n";
    std::cout << "  --------------------------------------------------------\n";
    int shown = 0;
    for (int i = 0; i < tableSize && shown < 20; i++) {
        if (table[i] != nullptr) {
            std::cout << "  Slot[" << std::setw(4) << i << "]: ";
            Node* current = table[i];
            while (current != nullptr) {
                std::cout << "[" << current->key << " | "
                          << current->data.name << "] -> ";
                current = current->next;
            }
            std::cout << "NULL\n";
            shown++;
        }
    }
    std::cout << "  --------------------------------------------------------\n";
    std::cout << "  Total records in table: " << numElements << "\n";
}

double HashTable::getLoadFactor() {
    return (double)numElements / tableSize;
}

int HashTable::getCollisionCount() {
    return collisionCount;
}

void HashTable::resetCollisionCount() {
    collisionCount = 0;
}

int HashTable::getTableSize() {
    return tableSize;
}

int HashTable::getNumElements() {
    return numElements;
}

int HashTable::getChainLength(int index) {
    int len = 0;
    Node* current = table[index];
    while (current != nullptr) { len++; current = current->next; }
    return len;
}

void HashTable::printLoadFactorAnalysis() {
    double alpha = getLoadFactor();
    int maxChain = 0, emptySlots = 0, usedSlots = 0;
    long long totalChainLen = 0;

    for (int i = 0; i < tableSize; i++) {
        int len = getChainLength(i);
        if (len == 0) emptySlots++;
        else {
            usedSlots++;
            totalChainLen += len;
            if (len > maxChain) maxChain = len;
        }
    }

    std::cout << std::fixed << std::setprecision(4);
    std::cout << "\n========================================================\n";
    std::cout << "  LOAD FACTOR ANALYSIS\n";
    std::cout << "========================================================\n";
    std::cout << "  n (elements)   : " << numElements << "\n";
    std::cout << "  m (table size) : " << tableSize << "\n";
    std::cout << "  Load factor a  : " << alpha << "\n";
    std::cout << "  Empty slots    : " << emptySlots << "\n";
    std::cout << "  Used slots     : " << usedSlots << "\n";
    std::cout << "  Max chain len  : " << maxChain << "\n";
    std::cout << "  Avg chain len  : "
              << (usedSlots > 0 ? (double)totalChainLen / usedSlots : 0) << "\n";
    std::cout << "  Collisions     : " << collisionCount << "\n";
    std::cout << "  Collision rate : "
              << (double)collisionCount / numElements * 100 << "%\n";
    std::cout << "\n  Theoretical complexity (chaining):\n";
    std::cout << "    Successful search   : Theta(1 + " << alpha
              << ") = Theta(" << 1 + alpha << ")\n";
    std::cout << "    Unsuccessful search : Theta(1 + " << alpha
              << ") = Theta(" << 1 + alpha << ")\n";
    std::cout << "========================================================\n";
}
