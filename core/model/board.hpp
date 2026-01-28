#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <memory>
#include <random>
#include "tetromino.hpp"

/**
 * Represents a classic Tetris game board.
 * Manages game grid, current piece, next piece, hold piece, and game logic.
 */
class ClassicBoard {
public:
    /**
     * Constructs a Tetris board with specified dimensions.
     * 
     * @param w Board width in cells (default: 10)
     * @param h Board height in cells (default: 20)
     */
    ClassicBoard(int w = 10, int h = 20);
    
    // Piece movement controls
    bool moveLeft();
    bool moveRight();
    bool moveDown();
    bool rotate();
    bool hardDrop();
    bool hold();
    
    // Game logic operations
    void spawnPiece();
    int clearLines();
    void reset();
    
    // Game state queries
    bool isGameOver() const { return gameOver; }
    const std::vector<std::vector<Color>>& getGrid() const { return grid; }
    const Tetromino* getCurrentPiece() const { return currentPiece.get(); }
    const Tetromino* getGhostPiece() const { return ghostPiece.get(); }
    const Tetromino* getNextPiece() const { return nextPiece.get(); }
    const Tetromino* getHoldPiece() const { return holdPiece.get(); }
    bool getCanHold() const { return canHold; }
    
    // Game statistics
    int getScore() const { return score; }
    int getLevel() const { return level; }
    int getLines() const { return linesCleared; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    
private:
    // Board state
    std::vector<std::vector<Color>> grid;
    int width, height;
    
    // Game pieces
    std::unique_ptr<Tetromino> currentPiece;
    std::unique_ptr<Tetromino> nextPiece;
    std::unique_ptr<Tetromino> ghostPiece;
    std::unique_ptr<Tetromino> holdPiece;
    
    // Game state
    bool canHold;
    int score;
    int level;
    int linesCleared;
    bool gameOver;
    
    // Random number generation for piece selection
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<> dis;
    
    // Helper methods
    void initializeGrid();
    bool isValidPosition(const Tetromino& piece) const;
    bool tryWallKick(int rotation);
    void updateGhostPiece();
    bool placePiece();
    TetrominoType getRandomPieceType();
};

#endif
