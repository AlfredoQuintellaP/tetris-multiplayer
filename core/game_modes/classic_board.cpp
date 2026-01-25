#include "classic_board.hpp"
#include <iostream>

ClassicBoard::ClassicBoard(int w, int h) 
    : width(w), height(h), score(0), level(1), linesCleared(0), 
      canHold(true), gameOver(false), gen(rd()), dis(0, 6) {
    initializeGrid();
}

void ClassicBoard::initializeGrid() {
    grid.clear();
    grid.resize(height, std::vector<Color>(width, Color::BLACK));
}

TetrominoType ClassicBoard::getRandomPieceType() {
    TetrominoType types[] = {
        TetrominoType::I, TetrominoType::O, TetrominoType::T,
        TetrominoType::L, TetrominoType::J, TetrominoType::S, TetrominoType::Z
    };
    return types[dis(gen)];
}

bool ClassicBoard::isValidPosition(const Tetromino& piece) const {
    auto shape = piece.getShape();
    int pieceX = piece.getX();
    int pieceY = piece.getY();
    
    for (size_t i = 0; i < shape.size(); i++) {
        for (size_t j = 0; j < shape[i].size(); j++) {
            if (shape[i][j]) {
                int boardX = pieceX + j;
                int boardY = pieceY + i;
                
                if (boardX < 0 || boardX >= width || boardY >= height) {
                    return false;
                }
                
                if (boardY >= 0 && grid[boardY][boardX] != Color::BLACK) {
                    return false;
                }
            }
        }
    }
    return true;
}

bool ClassicBoard::tryWallKick(int rotation) {
    if (!currentPiece) return false;
    
    // Wall kick offsets (simplified SRS)
    // Tenta mover a peça em diferentes direções para encaixar
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
        currentPiece->move(-offset[0], -offset[1]);
    }
    
    return false;
}

void ClassicBoard::updateGhostPiece() {
    if (!currentPiece) return;

    ghostPiece = currentPiece->clone();
    while (isValidPosition(*ghostPiece)) {
        ghostPiece->move(0, 1);
    }
    ghostPiece->move(0, -1);
}

void ClassicBoard::spawnPiece() {
    if (gameOver) return;  // FIX: Não spawna peça se game over
    
    // Se nao tem proxima peca, gera uma
    if (!nextPiece) {
        nextPiece = Tetromino::create(getRandomPieceType());
    }
    
    // Current peca recebe a proxima
    currentPiece = std::move(nextPiece);
    currentPiece->setPosition(width / 2 - 1, 0);
    
    // Verifica se a peça pode ser colocada (game over check)
    if (!isValidPosition(*currentPiece)) {
        gameOver = true;
        return;
    }
    
    // Gera nova proxima peca
    nextPiece = Tetromino::create(getRandomPieceType());
    
    updateGhostPiece();
}

bool ClassicBoard::hold() {
    if (!canHold || !currentPiece || gameOver) {
        return false;
    }

    if (!holdPiece) {
        holdPiece = Tetromino::create(currentPiece->getType());
        spawnPiece();
    } else {
        auto tempType = holdPiece->getType();
        holdPiece = Tetromino::create(currentPiece->getType());
        currentPiece = Tetromino::create(tempType);
        currentPiece->setPosition(width / 2 - 1, 0);

        if (!isValidPosition(*currentPiece)) {
            return false;
        }

        updateGhostPiece();
    }

    canHold = false;
    return true;
}

bool ClassicBoard::moveLeft() {
    if (!currentPiece || gameOver) return false;
    
    currentPiece->move(-1, 0);
    if (!isValidPosition(*currentPiece)) {
        currentPiece->move(1, 0);
        return false;
    }
    updateGhostPiece();
    return true;
}

bool ClassicBoard::moveRight() {
    if (!currentPiece || gameOver) return false;
    
    currentPiece->move(1, 0);
    if (!isValidPosition(*currentPiece)) {
        currentPiece->move(-1, 0);
        return false;
    }
    updateGhostPiece();
    return true;
}

bool ClassicBoard::moveDown() {
    if (!currentPiece || gameOver) return false;
    
    currentPiece->move(0, 1);
    if (!isValidPosition(*currentPiece)) {
        currentPiece->move(0, -1);
        placePiece();
        clearLines();
        spawnPiece();
        return false;
    }
    updateGhostPiece();
    return true;
}

bool ClassicBoard::rotate() {
    if (!currentPiece || gameOver) return false;
    
    currentPiece->rotate();
    
    // Se não couber após rotação, tenta wall kick
    if (!isValidPosition(*currentPiece)) {
        if (!tryWallKick(1)) {
            // Se wall kick falhar, desfaz rotação
            for (int i = 0; i < 3; i++) {
                currentPiece->rotate();
            }
            return false;
        }
    }
    
    updateGhostPiece();
    return true;
}

bool ClassicBoard::hardDrop() {
    if (!currentPiece || gameOver) return false;
    
    while (moveDown()) {
        // Continua caindo ate nao poder mais
    }
    return true;
}

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
    
    canHold = true;  // Reset hold availability
    return true;
}

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
            grid.erase(grid.begin() + y);
            grid.insert(grid.begin(), std::vector<Color>(width, Color::BLACK));
            linesClearedThisTurn++;
            y++; 
        }
    }
    
    if (linesClearedThisTurn > 0) {
        linesCleared += linesClearedThisTurn;
        
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
        
        level = linesCleared / 10;
    }
    
    return linesClearedThisTurn;
}

void ClassicBoard::reset() {
    initializeGrid();
    
    score = 0;
    level = 1;
    linesCleared = 0;
    canHold = true;
    gameOver = false;  // Reset game over flag
    
    currentPiece.reset();
    nextPiece.reset();
    ghostPiece.reset();
    holdPiece.reset();
    
    gen = std::mt19937(rd());
}
