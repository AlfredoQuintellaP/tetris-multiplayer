#include "board_renderer.hpp"

sf::Color BoardRenderer::getSFMLColor(Color color, bool isGhost) const {
    int alpha = isGhost ? 100 : 255;
    
    switch (color) {
        case Color::CYAN:    return sf::Color(0, 255, 255, alpha);
        case Color::YELLOW:  return sf::Color(255, 255, 0, alpha);
        case Color::PURPLE:  return sf::Color(128, 0, 128, alpha);
        case Color::ORANGE:  return sf::Color(255, 165, 0, alpha);
        case Color::BLUE:    return sf::Color(0, 0, 255, alpha);
        case Color::GREEN:   return sf::Color(0, 255, 0, alpha);
        case Color::RED:     return sf::Color(255, 0, 0, alpha);
        case Color::GRAY:    return sf::Color(128, 128, 128, alpha);
        case Color::BLACK:   return sf::Color::Black;
        default:             return sf::Color::White;
    }
}

void BoardRenderer::drawGrid(sf::RenderWindow& window, int blockSize, int offsetX, int offsetY) const {
    for (int y = 0; y < board.getHeight(); y++) {
        for (int x = 0; x < board.getWidth(); x++) {
            sf::RectangleShape cell(sf::Vector2f(blockSize - 1, blockSize - 1));
            cell.setPosition(offsetX + x * blockSize, offsetY + y * blockSize);
            cell.setFillColor(sf::Color(50, 50, 50));
            cell.setOutlineThickness(1);
            cell.setOutlineColor(sf::Color(80, 80, 80));
            window.draw(cell);
        }
    }
}

void BoardRenderer::drawBlock(sf::RenderWindow& window, int x, int y, Color color, 
                             int blockSize, int offsetX, int offsetY, bool isGhost) const {
    if (color == Color::BLACK) return;
    
    sf::RectangleShape block(sf::Vector2f(blockSize - 1, blockSize - 1));
    block.setPosition(offsetX + x * blockSize, offsetY + y * blockSize);
    block.setFillColor(getSFMLColor(color, isGhost));
    
    if (!isGhost) {
        block.setOutlineThickness(1);
        block.setOutlineColor(sf::Color::White);
    }
    
    window.draw(block);
}

void BoardRenderer::drawPiece(sf::RenderWindow& window, const Tetromino* piece, 
                             int blockSize, int offsetX, int offsetY, bool isGhost) const {
    if (!piece) return;
    
    auto shape = piece->getShape();
    int pieceX = piece->getX();
    int pieceY = piece->getY();
    
    for (size_t i = 0; i < shape.size(); i++) {
        for (size_t j = 0; j < shape[i].size(); j++) {
            if (shape[i][j]) {
                int boardX = pieceX + static_cast<int>(j);
                int boardY = pieceY + static_cast<int>(i);
                if (boardY >= 0) {
                    drawBlock(window, boardX, boardY, piece->getColor(), 
                             blockSize, offsetX, offsetY, isGhost);
                }
            }
        }
    }
}

void BoardRenderer::draw(sf::RenderWindow& window, int blockSize, int offsetX, int offsetY) const {
    // Desenha grid de fundo
    drawGrid(window, blockSize, offsetX, offsetY);
    
    // Desenha blocos fixos
    auto& grid = board.getGrid();
    for (int y = 0; y < board.getHeight(); y++) {
        for (int x = 0; x < board.getWidth(); x++) {
            if (grid[y][x] != Color::BLACK) {
                drawBlock(window, x, y, grid[y][x], blockSize, offsetX, offsetY, false);
            }
        }
    }
    
    // Desenha ghost piece
    drawPiece(window, board.getGhostPiece(), blockSize, offsetX, offsetY, true);
    
    // Desenha peça atual
    drawPiece(window, board.getCurrentPiece(), blockSize, offsetX, offsetY, false);
}
