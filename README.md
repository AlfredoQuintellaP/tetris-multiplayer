# Tetris MVC - Classic Tetris Game

## Project Overview

A classic Tetris game implementation using the Model-View-Controller (MVC) architectural pattern. This project features single-player and multiplayer modes with both local and online multiplayer support.

## Features

### Game Modes
- **Single Player**: Classic Tetris gameplay
- **Local Multiplayer**: Two players on the same computer
- **Online Multiplayer**: Network play with other players over LAN or internet

### Game Mechanics
- Classic Tetris piece movement and rotation
- Ghost piece preview
- Hold piece functionality
- Wall kick rotation system
- Line clearing with progressive scoring
- Level progression system

### Technical Features
- MVC architecture with clear separation of concerns
- Customizable key bindings for different players
- Network communication for online multiplayer
- Responsive UI with adaptive layouts
- Cross-platform compatibility

## Project Structure
```bash
.
├── main.cpp # Entry point and window management
├── Makefile # Build system
├── bin/ # Compiled executable output
├── build/ # Object files and build artifacts
├── core/
│ ├── model/ # Game data and logic (Model)
│ │ ├── board.hpp/cpp # Tetris board implementation
│ │ ├── tetromino.hpp/cpp # Tetris piece implementation
│ │ └── game_data.hpp/cpp # Game state management
│ ├── controller/ # Input and game logic (Controller)
│ │ ├── game_controller.hpp/cpp # Main controller
│ │ └── networking/ # Network communication
│ └── view/ # Rendering system (View)
│ ├── game_renderer.hpp/cpp # Main renderer
│ ├── board_renderer.hpp/cpp # Board rendering
│ └── layout_manager.hpp/cpp # UI layout management
└── utils/ # Utility headers
├── color.hpp # Color enumeration
└── game_state.hpp # Game state enumeration
```

## System Requirements

### Dependencies
- **SFML 2.5+** (Simple and Fast Multimedia Library)
  - Graphics module
  - Window module
  - System module
  - Network module (for online multiplayer)
- **C++17** compatible compiler

### Supported Platforms
- Linux (Ubuntu, Fedora, Arch, etc.)
- Windows (MinGW or Visual Studio)
- macOS

## Installation

### Step 1: Install SFML

#### Linux
```bash
# Ubuntu/Debian
sudo apt-get update
sudo apt-get install libsfml-dev

# Fedora
sudo dnf install SFML-devel

# Arch Linux
sudo pacman -S sfml
```

#### macOS
```bash
brew install sfml
```

#### Windows
1. Download SFML from sfml-dev.org
2. Extract to C:\SFML or your preferred directory
3. Ensure SFML libraries are in your system PATH

### Step 2: Clone and Build
```bash
# Clone the repository
git clone <repository-url>
cd tetris-mvc

# Build the project
make

# Or build and run directly
make run
```

## Building the Project

### Using Makefile
The project includes a comprehensive Makefile with several commands:
```bash

```
