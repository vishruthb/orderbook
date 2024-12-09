# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -pedantic

# Executable name
TARGET := orderbook

# Source files
SRCS := main.cpp parser.cpp matching_engine.cpp order_book.cpp
# Corresponding object files (replace .cpp with .o)
OBJS := $(SRCS:.cpp=.o)

# Header files
DEPS := parser.h matching_engine.h order_book.h order.h

# Default target
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

# Compile object files from .cpp
%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets
.PHONY: all clean
