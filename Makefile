CXX      = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2 -Iinclude
SRCDIR   = src
INCDIR   = include
TARGET   = student_system
SOURCES  = $(SRCDIR)/main.cpp $(SRCDIR)/HashTable.cpp $(SRCDIR)/LinearSearch.cpp $(SRCDIR)/DataGenerator.cpp
OBJS     = $(SOURCES:.cpp=.o)

# ── Default target ────────────────────────────────────────────
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# ── Object files ──────────────────────────────────────────────
$(SRCDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

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
