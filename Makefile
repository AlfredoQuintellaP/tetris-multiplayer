CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -g
SFML_FLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network

SRC_DIR = core
BUILD_DIR = build
BIN_DIR = bin

GAME_MODES_DIR = $(SRC_DIR)/game_modes
ENTITIES_DIR = $(SRC_DIR)/entities
RENDERING_DIR = $(SRC_DIR)/rendering
UTILS_DIR = $(SRC_DIR)/utils
NETWORKING_DIR = $(SRC_DIR)/networking

SOURCES = \
	main.cpp \
	$(GAME_MODES_DIR)/game.cpp \
	$(GAME_MODES_DIR)/classic_board.cpp \
	$(ENTITIES_DIR)/tetromino.cpp \
	$(RENDERING_DIR)/board_renderer.cpp \
	$(NETWORKING_DIR)/network_manager.cpp

OBJECTS = $(SOURCES:%.cpp=$(BUILD_DIR)/%.o)

TARGET = $(BIN_DIR)/tetris

all: $(TARGET)

$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(CXX) $(OBJECTS) -o $(TARGET) $(SFML_FLAGS)

$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BUILD_DIR)/$(GAME_MODES_DIR)
	@mkdir -p $(BUILD_DIR)/$(ENTITIES_DIR)
	@mkdir -p $(BUILD_DIR)/$(RENDERING_DIR)
	@mkdir -p $(BUILD_DIR)/$(UTILS_DIR)
	@mkdir -p $(BUILD_DIR)/$(NETWORKING_DIR)

$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

rebuild: clean all

run: $(TARGET)
	./$(TARGET)

debug: $(TARGET)
	valgrind --leak-check=full ./$(TARGET)

check-sfml:
	@echo "Checking SFML dependencies..."
	@pkg-config --cflags sfml-all 2>/dev/null && echo "SFML Found !" || echo "SFML not found. Install with: sudo apt-get install libsfml-dev"

info:
	@echo "=== Tetris Project Info ==="
	@echo "@ Source files:"
	@echo $(SOURCES)
	@echo "@ Object files:"
	@echo $(OBJECTS)
	@echo "@ Target: $(TARGET)"
	@echo "@ Compiler: $(CXX)"
	@echo "@ Flags: $(CXXFLAGS)"

help:
	@echo "=== Tetris Makefile Commands ==="
	@echo "make all        - Compile all project"
	@echo "make run        - Compile and run file"
	@echo "make clean      - Clean compiled files"
	@echo "make rebuild    - Clean and recompile everything"
	@echo "make debug      - Execute with valgrind to detect possible leaks"
	@echo "make check-sfml - Check if SFML is installed"
	@echo "make info       - Show project info"
	@echo "make help       - Show this help guide"

.PHONY: all clean rebuild run debug check-sfml info help
