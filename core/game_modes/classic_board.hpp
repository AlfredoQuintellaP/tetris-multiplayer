#ifndef CLASSIC_BOARD_H
#define CLASSIC_BOARD_H

#include <vector>
#include <memory>
#include <random>
#include "../entities/tetromino.hpp"

class ClassicBoard {
private:
    std::vector<std::vector<Color>> grid;
    int width, height;
    std::unique_ptr<Tetromino> currentPiece;
    std::unique_ptr<Tetromino> nextPiece;
    std::unique_ptr<Tetromino> ghostPiece;
    int score;
    int level;
    int linesCleared;
    
    // Sistema de geracao de peças
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<> dis;
    
public:
    ClassicBoard(int w = 10, int h = 20);
    
    // Controles
    bool moveLeft();
    bool moveRight();
    bool moveDown();
    bool rotate();
    bool hardDrop();
    
    // Logica do jogo
    void spawnPiece();
    bool placePiece();
    int clearLines();
    bool isGameOver() const;
    void reset();
    
    // Verificacoes
    bool isValidPosition(const Tetromino& piece) const;
    void updateGhostPiece();
    
    // Getters para o render
    const std::vector<std::vector<Color>>& getGrid() const { return grid; }
    const Tetromino* getCurrentPiece() const { return currentPiece.get(); }
    const Tetromino* getGhostPiece() const { return ghostPiece.get(); }
    const Tetromino* getNextPiece() const { return nextPiece.get(); }
    int getScore() const { return score; }
    int getLevel() const { return level; }
    int getLines() const { return linesCleared; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }

private:
    void initializeGrid();
    TetrominoType getRandomPieceType();
};

#endif
