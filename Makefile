CXX      = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
TARGET   = student_system
OBJS     = main.o HashTable.o LinearSearch.o DataGenerator.o

# ── Default target ────────────────────────────────────────────
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# ── Object files ──────────────────────────────────────────────
main.o: main.cpp HashTable.h LinearSearch.h DataGenerator.h Student.h Node.h
	$(CXX) $(CXXFLAGS) -c main.cpp

HashTable.o: HashTable.cpp HashTable.h Node.h Student.h
	$(CXX) $(CXXFLAGS) -c HashTable.cpp

LinearSearch.o: LinearSearch.cpp LinearSearch.h Student.h
	$(CXX) $(CXXFLAGS) -c LinearSearch.cpp

DataGenerator.o: DataGenerator.cpp DataGenerator.h
	$(CXX) $(CXXFLAGS) -c DataGenerator.cpp

# ── Run ───────────────────────────────────────────────────────
run: $(TARGET)
	./$(TARGET)

# ── Clean ─────────────────────────────────────────────────────
clean:
	rm -f $(OBJS) $(TARGET) $(TARGET).exe students.csv export.csv

# ── Help ──────────────────────────────────────────────────────
help:
	@echo "Usage:"
	@echo "  make          - Build the project"
	@echo "  make run      - Build and run"
	@echo "  make clean    - Remove build artifacts"
