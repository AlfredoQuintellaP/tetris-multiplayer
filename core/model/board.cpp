#include "board.hpp"
#include <iostream>

/**
 * Constructs board with specified dimensions and initializes game state.
 */
ClassicBoard::ClassicBoard(int w, int h) 
    : width(w), height(h), 
      currentPiece(nullptr), 
      nextPiece(nullptr), 
      ghostPiece(nullptr), 
      holdPiece(nullptr),
      canHold(true), 
      score(0), 
      level(1), 
      linesCleared(0), 
      gameOver(false), 
      gen(rd()), 
      dis(0, 6) {
    
    initializeGrid();
}

/**
 * Initializes the game grid with empty cells.
 */
void ClassicBoard::initializeGrid() {
    grid.clear();
    grid.resize(height, std::vector<Color>(width, Color::BLACK));
}

/**
 * Generates a random Tetromino type.
 */
TetrominoType ClassicBoard::getRandomPieceType() {
    TetrominoType types[] = {
        TetrominoType::I, TetrominoType::O, TetrominoType::T,
        TetrominoType::L, TetrominoType::J, TetrominoType::S, TetrominoType::Z
    };
    return types[dis(gen)];
}

/**
 * Checks if a piece can be placed at its current position.
 */
bool ClassicBoard::isValidPosition(const Tetromino& piece) const {
    auto shape = piece.getShape();
    int pieceX = piece.getX();
    int pieceY = piece.getY();
    
    for (size_t i = 0; i < shape.size(); i++) {
        for (size_t j = 0; j < shape[i].size(); j++) {
            if (shape[i][j]) {
                int boardX = pieceX + j;
                int boardY = pieceY + i;
                
                // Check bounds
                if (boardX < 0 || boardX >= width || boardY >= height) {
                    return false;
                }
                
                // Check collision with placed pieces
                if (boardY >= 0 && grid[boardY][boardX] != Color::BLACK) {
                    return false;
                }
            }
        }
    }
    return true;
}

/**
 * Attempts wall kick adjustments when rotation fails.
 * Simplified SRS (Super Rotation System) implementation.
 */
bool ClassicBoard::tryWallKick(int rotation) {
    if (!currentPiece) return false;
    
    // Wall kick offset patterns (simplified)
    int kickOffsets[][2] = {
        {-1, 0},  // Left
        {1, 0},   // Right
        {-2, 0},  // Left 2
        {2, 0},   // Right 2
        {0, -1},  // Up
        {-1, -1}, // Left + Up
        {1, -1},  // Right + Up
    };
    
    for (auto& offset : kickOffsets) {
        currentPiece->move(offset[0], offset[1]);
        if (isValidPosition(*currentPiece)) {
            return true;
        }
        currentPiece->move(-offset[0], -offset[1]); // Revert
    }
    
    return false;
}

/**
 * Updates the ghost piece (preview of where piece will land).
 */
void ClassicBoard::updateGhostPiece() {
    if (!currentPiece) return;

    ghostPiece = currentPiece->clone();
    
    // Drop ghost piece to lowest valid position
    while (isValidPosition(*ghostPiece)) {
        ghostPiece->move(0, 1);
    }
    ghostPiece->move(0, -1); // Move back up one to valid position
}

/**
 * Spawns a new piece at the top of the board.
 */
void ClassicBoard::spawnPiece() {
    if (gameOver) return;
    
    // Generate next piece if needed
    if (!nextPiece) {
        nextPiece = Tetromino::create(getRandomPieceType());
    }
    
    // Current piece becomes next piece
    currentPiece = std::move(nextPiece);
    currentPiece->setPosition(width / 2 - 1, 0);
    
    // Game over check: piece can't be placed
    if (!isValidPosition(*currentPiece)) {
        gameOver = true;
        return;
    }
    
    // Generate new next piece
    nextPiece = Tetromino::create(getRandomPieceType());
    
    updateGhostPiece();
}

/**
 * Swaps current piece with hold piece.
 */
bool ClassicBoard::hold() {
    if (!canHold || !currentPiece || gameOver) {
        return false;
    }

    if (!holdPiece) {
        // First hold: store current, spawn new
        holdPiece = Tetromino::create(currentPiece->getType());
        spawnPiece();
    } else {
        // Swap current with held piece
        auto tempType = holdPiece->getType();
        holdPiece = Tetromino::create(currentPiece->getType());
        currentPiece = Tetromino::create(tempType);
        currentPiece->setPosition(width / 2 - 1, 0);

        if (!isValidPosition(*currentPiece)) {
            return false;
        }

        updateGhostPiece();
    }

    canHold = false; // Can't hold again until piece locks
    return true;
}

/**
 * Moves current piece left if possible.
 */
bool ClassicBoard::moveLeft() {
    if (!currentPiece || gameOver) return false;
    
    currentPiece->move(-1, 0);
    if (!isValidPosition(*currentPiece)) {
        currentPiece->move(1, 0); // Revert
        return false;
    }
    updateGhostPiece();
    return true;
}

/**
 * Moves current piece right if possible.
 */
bool ClassicBoard::moveRight() {
    if (!currentPiece || gameOver) return false;
    
    currentPiece->move(1, 0);
    if (!isValidPosition(*currentPiece)) {
        currentPiece->move(-1, 0); // Revert
        return false;
    }
    updateGhostPiece();
    return true;
}

/**
 * Moves current piece down; locks piece if it can't move.
 */
bool ClassicBoard::moveDown() {
    if (!currentPiece || gameOver) return false;
    
    currentPiece->move(0, 1);
    if (!isValidPosition(*currentPiece)) {
        currentPiece->move(0, -1); // Revert
        placePiece();
        clearLines();
        spawnPiece();
        return false;
    }
    updateGhostPiece();
    return true;
}

/**
 * Rotates current piece with wall kick.
 */
bool ClassicBoard::rotate() {
    if (!currentPiece || gameOver) return false;
    
    currentPiece->rotate();
    
    // Attempt wall kick if rotation invalid
    if (!isValidPosition(*currentPiece)) {
        if (!tryWallKick(1)) {
            // Revert rotation if wall kick fails
            for (int i = 0; i < 3; i++) {
                currentPiece->rotate();
            }
            return false;
        }
    }
    
    updateGhostPiece();
    return true;
}

/**
 * Drops piece instantly to lowest position.
 */
bool ClassicBoard::hardDrop() {
    if (!currentPiece || gameOver) return false;
    
    while (moveDown()) {
        // Continue dropping until piece locks
    }
    return true;
}

/**
 * Locks current piece onto the board.
 */
bool ClassicBoard::placePiece() {
    if (!currentPiece) return false;
    
    auto shape = currentPiece->getShape();
    int pieceX = currentPiece->getX();
    int pieceY = currentPiece->getY();
    
    for (size_t i = 0; i < shape.size(); i++) {
        for (size_t j = 0; j < shape[i].size(); j++) {
            if (shape[i][j]) {
                int boardX = pieceX + j;
                int boardY = pieceY + i;
                
                if (boardY >= 0) {
                    grid[boardY][boardX] = currentPiece->getColor();
                }
            }
        }
    }
    
    canHold = true; // Allow holding next piece
    return true;
}

/**
 * Clears completed lines and updates score.
 */
int ClassicBoard::clearLines() {
    int linesClearedThisTurn = 0;
    
    for (int y = height - 1; y >= 0; y--) {
        bool lineComplete = true;
        for (int x = 0; x < width; x++) {
            if (grid[y][x] == Color::BLACK) {
                lineComplete = false;
                break;
            }
        }
        
        if (lineComplete) {
            // Remove line and shift everything down
            grid.erase(grid.begin() + y);
            grid.insert(grid.begin(), std::vector<Color>(width, Color::BLACK));
            linesClearedThisTurn++;
            y++; // Recheck same position after shift
        }
    }
    
    // Update score based on lines cleared
    if (linesClearedThisTurn > 0) {
        linesCleared += linesClearedThisTurn;
        
        // Standard Tetris scoring
        switch (linesClearedThisTurn) {
            case 1:
                score += 40 * (level + 1);
                break;
            case 2:
                score += 100 * (level + 1);
                break;
            case 3:
                score += 300 * (level + 1);
                break;
            case 4:
                score += 1200 * (level + 1);
                break;
        }
        
        // Update level every 10 lines
        level = linesCleared / 10;
    }
    
    return linesClearedThisTurn;
}

/**
 * Resets board to initial state.
 */
void ClassicBoard::reset() {
    initializeGrid();
    
    score = 0;
    level = 1;
    linesCleared = 0;
    canHold = true;
    gameOver = false;
    
    currentPiece.reset();
    nextPiece.reset();
    ghostPiece.reset();
    holdPiece.reset();
    
    // Reset random number generator
    gen = std::mt19937(rd());
}
