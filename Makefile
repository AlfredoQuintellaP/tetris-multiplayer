# ============================================
# COMPILER CONFIGURATION
# ============================================
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
SFML_FLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network

# ============================================
# DIRECTORY STRUCTURE
# ============================================
SRC_DIR = .
BUILD_DIR = build
BIN_DIR = bin

MODEL_DIR = core/model
CONTROLLER_DIR = core/controller
VIEW_DIR = core/view
NETWORKING_DIR = $(CONTROLLER_DIR)/networking

# ============================================
# SOURCE FILES
# ============================================
MODEL_SOURCES = \
	$(MODEL_DIR)/game_data.cpp \
	$(MODEL_DIR)/board.cpp \
	$(MODEL_DIR)/tetromino.cpp

CONTROLLER_SOURCES = \
	$(CONTROLLER_DIR)/game_controller.cpp \
	$(NETWORKING_DIR)/network_manager.cpp

VIEW_SOURCES = \
	$(VIEW_DIR)/game_renderer.cpp \
	$(VIEW_DIR)/board_renderer.cpp \
	$(VIEW_DIR)/layout_manager.cpp

MAIN_SOURCE = main.cpp

SOURCES = \
	$(MAIN_SOURCE) \
	$(MODEL_SOURCES) \
	$(CONTROLLER_SOURCES) \
	$(VIEW_SOURCES)

# ============================================
# OBJECT FILES
# ============================================
OBJECTS = $(SOURCES:%.cpp=$(BUILD_DIR)/%.o)

# ============================================
# FINAL TARGET
# ============================================
TARGET = $(BIN_DIR)/tetris_mvc

# ============================================
# MAIN RULES
# ============================================
all: $(TARGET)

$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(CXX) $(OBJECTS) -o $(TARGET) $(SFML_FLAGS)
	@echo "Tetris MVC compiled successfully: $(TARGET)"

$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -Wno-unused-parameter -c $< -o $@

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BUILD_DIR)/$(MODEL_DIR)
	@mkdir -p $(BUILD_DIR)/$(CONTROLLER_DIR)
	@mkdir -p $(BUILD_DIR)/$(NETWORKING_DIR)
	@mkdir -p $(BUILD_DIR)/$(VIEW_DIR)

$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

# ============================================
# UTILITY RULES
# ============================================
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
	@echo "Build files removed"

rebuild: clean all

run: $(TARGET)
	./$(TARGET)

debug: CXXFLAGS += -g
debug: clean all
	@echo "Compiled with debug flags"

info:
	@echo "=== Tetris MVC Project Info ==="
	@echo "Source files:"
	@echo $(SOURCES)
	@echo "Object files:"
	@echo $(OBJECTS)
	@echo "Target: $(TARGET)"

help:
	@echo "=== Tetris MVC Makefile Commands ==="
	@echo "make all           - Build the project (default)"
	@echo "make clean         - Remove build files"
	@echo "make rebuild       - Clean and rebuild everything"
	@echo "make run           - Build and run the application"
	@echo "make debug         - Build with debug symbols"
	@echo "make info          - Show project information"
	@echo "make help          - Show this help message"
	@echo "make sfml          - Show SFML installation instructions"
	@echo ""
	@echo "Target directory: $(TARGET)"

sfml:
	@echo "=== SFML Installation Instructions ==="
	@echo ""
	@echo "For Ubuntu/Debian:"
	@echo "sudo apt-get update"
	@echo "sudo apt-get install libsfml-dev"
	@echo ""
	@echo "For Fedora:"
	@echo "sudo dnf install SFML-devel"
	@echo ""
	@echo "For Arch Linux:"
	@echo "sudo pacman -S sfml"
	@echo ""
	@echo "For macOS with Homebrew:"
	@echo "brew install sfml"
	@echo ""
	@echo "For Windows (MinGW):"
	@echo "1. Download SFML from https://www.sfml-dev.org/download.php"
	@echo "2. Extract to C:\\SFML or similar directory"
	@echo "3. Set SFML environment variables or update makefile"
	@echo ""
	@echo "Note: This project requires SFML modules:"
	@echo "  - sfml-graphics"
	@echo "  - sfml-window"
	@echo "  - sfml-system"
	@echo "  - sfml-network (for multiplayer)"

.PHONY: all clean rebuild run debug info help sfml
