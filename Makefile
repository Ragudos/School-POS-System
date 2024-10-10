# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -I include -pthread

# OS detection
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    # Commands for Linux
    MKDIR = mkdir -p
    RM = rm -f
    CXXFLAGS += -DLINUX_PLATFORM -Wall -Wextra  # Define for Linux
else ifeq ($(UNAME_S),Darwin)
    # Commands for macOS
    MKDIR = mkdir -p
    RM = rm -f
    CXXFLAGS += -DMAC_PLATFORM -Wall -Wextra  # Define for macOS
else
    # Commands for Windows (assumes using MinGW or similar)
    MKDIR = mkdir
    RM = del /Q
    CXXFLAGS += -DWINDOWS_PLATFORM -Wall -Wextra  # Define for Windows
endif

# Directories
SRC_DIR = src
TEST_DIR = __tests__
BIN_DIR = bin
INC_DIR = include

# Source files
SRCS = $(SRC_DIR)/main.cpp
TEST_SRCS = $(TEST_DIR)/main_test.cpp

# Object files
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(BIN_DIR)/%.o,$(SRCS))
TEST_OBJS = $(patsubst $(TEST_DIR)/%.cpp,$(BIN_DIR)/%.o,$(TEST_SRCS))

# Targets
TARGET = $(BIN_DIR)/main
TEST_TARGET = $(BIN_DIR)/main_test

# Build rules
all: $(TARGET) $(TEST_TARGET)

# Build main executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Build test executable
$(TEST_TARGET): $(TEST_OBJS) $(BIN_DIR)/utils.o
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile source files
$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(MKDIR) $(BIN_DIR)       # Create the bin directory if it doesn't exist
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BIN_DIR)/%.o: $(TEST_DIR)/%.cpp
	$(MKDIR) $(BIN_DIR)       # Create the bin directory if it doesn't exist
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule
clean:
	$(RM) $(BIN_DIR)/*.o $(TARGET) $(TEST_TARGET)

.PHONY: all clean
