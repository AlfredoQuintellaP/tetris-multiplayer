#include "classic_board.hpp"
#include <iostream>

ClassicBoard::ClassicBoard(int w, int h) 
    : width(w), height(h), score(0), level(1), linesCleared(0), gen(rd()), dis(0, 6) {
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

void ClassicBoard::updateGhostPiece() {
    if (!currentPiece) return;

    ghostPiece = currentPiece->clone();
    while (isValidPosition(*ghostPiece)) {
        ghostPiece->move(0, 1);
    }
    ghostPiece->move(0, -1);
}

void ClassicBoard::spawnPiece() {
    // Se nao tem proxima peca, gera uma
    if (!nextPiece) {
        nextPiece = Tetromino::create(getRandomPieceType());
    }
    
    // Current peca recebe a proxima
    currentPiece = std::move(nextPiece);
    currentPiece->setPosition(width / 2 - 1, 0);
    
    // Gera nova proxima peca
    nextPiece = Tetromino::create(getRandomPieceType());
    
    updateGhostPiece();
}

bool ClassicBoard::moveLeft() {
    if (!currentPiece) return false;
    
    currentPiece->move(-1, 0);
    if (!isValidPosition(*currentPiece)) {
        currentPiece->move(1, 0);
        return false;
    }
    updateGhostPiece();
    return true;
}

bool ClassicBoard::moveRight() {
    if (!currentPiece) return false;
    
    currentPiece->move(1, 0);
    if (!isValidPosition(*currentPiece)) {
        currentPiece->move(-1, 0);
        return false;
    }
    updateGhostPiece();
    return true;
}

bool ClassicBoard::moveDown() {
    if (!currentPiece) return false;
    
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
    if (!currentPiece) return false;
    
    currentPiece->rotate();
    if (!isValidPosition(*currentPiece)) {
        // Tenta wall kick - desfaz rotaaco se nao couber
        for (int i = 0; i < 3; i++) {
            currentPiece->rotate(); // Rotaciona mais 3x = 360 (volta ao original)
        }
        return false;
    }
    updateGhostPiece();
    return true;
}

bool ClassicBoard::hardDrop() {
    if (!currentPiece) return false;
    
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
            // Remove a linha
            grid.erase(grid.begin() + y);
            // Adiciona nova linha no topo
            grid.insert(grid.begin(), std::vector<Color>(width, Color::BLACK));
            linesClearedThisTurn++;
            y++; 
        }
    }
    
    // Atualiza score
    if (linesClearedThisTurn > 0) {
        linesCleared += linesClearedThisTurn;
        
        // Sistema de pontuacao classico do Tetris
        switch (linesClearedThisTurn) {
            case 1:
                score += 100 * level;
                break;
            case 2:
                score += 300 * level;
                break;
            case 3:
                score += 500 * level;
                break;
            case 4:
                score += 800 * level; // Tetris!
                break;
        }
        
        level = linesCleared / 10 + 1;
    }
    
    return linesClearedThisTurn;
}

bool ClassicBoard::isGameOver() const {
    if (!currentPiece) return true;
    return !isValidPosition(*currentPiece);
}

void ClassicBoard::reset() {
    initializeGrid();
    
    // Reseta todas as variaveis de estado
    score = 0;
    level = 1;
    linesCleared = 0;
    
    // Reseta as pecas
    currentPiece.reset();
    nextPiece.reset();
    ghostPiece.reset();
    
    // Reseta o gerador de numeros aleatorios
    gen = std::mt19937(rd());

}
