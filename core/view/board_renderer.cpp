#include "board_renderer.hpp"

/**
 * Renders complete board with all visual elements.
 */
void BoardRenderer::render(sf::RenderWindow& window, const ClassicBoard& board, 
                          float x, float y, float cellSize) {
    // Draw background grid
    drawGrid(window, board, x, y, cellSize);
    
    // Draw placed pieces
    auto grid = board.getGrid();
    for (int row = 0; row < board.getHeight(); ++row) {
        for (int col = 0; col < board.getWidth(); ++col) {
            if (grid[row][col] != Color::BLACK) {
                float cellX = x + col * cellSize;
                float cellY = y + row * cellSize;
                drawCell(window, cellX, cellY, cellSize, getSFMLColor(grid[row][col]));
            }
        }
    }
    
    // Draw ghost piece (where current piece will land)
    renderGhostPiece(window, board, x, y, cellSize);
    
    // Draw current piece (on top of ghost piece)
    auto currentPiece = board.getCurrentPiece();
    if (currentPiece) {
        auto shape = currentPiece->getShape();
        int pieceX = currentPiece->getX();
        int pieceY = currentPiece->getY();
        
        for (int row = 0; row < currentPiece->getHeight(); ++row) {
            for (int col = 0; col < currentPiece->getWidth(); ++col) {
                if (shape[row][col]) {
                    float cellX = x + (pieceX + col) * cellSize;
                    float cellY = y + (pieceY + row) * cellSize;
                    drawCell(window, cellX, cellY, cellSize, getSFMLColor(currentPiece->getColor()));
                }
            }
        }
    }
}

/**
 * Renders a single Tetromino piece (used for next/hold panels).
 */
void BoardRenderer::renderPiece(sf::RenderWindow& window, const Tetromino& piece,
                               float x, float y, float cellSize, float alpha) {
    auto shape = piece.getShape();
    auto color = getSFMLColor(piece.getColor());
    
    // Apply transparency if needed
    if (alpha < 1.0f) {
        color.a = static_cast<sf::Uint8>(alpha * 255);
    }
    
    // Draw each cell of the piece
    for (int row = 0; row < piece.getHeight(); ++row) {
        for (int col = 0; col < piece.getWidth(); ++col) {
            if (shape[row][col]) {
                float cellX = x + col * cellSize;
                float cellY = y + row * cellSize;
                drawCell(window, cellX, cellY, cellSize, color);
            }
        }
    }
}

/**
 * Renders ghost piece (semi-transparent preview).
 */
void BoardRenderer::renderGhostPiece(sf::RenderWindow& window, const ClassicBoard& board,
                                    float x, float y, float cellSize) {
    auto ghostPiece = board.getGhostPiece();
    if (!ghostPiece) return;
    
    auto shape = ghostPiece->getShape();
    int pieceX = ghostPiece->getX();
    int pieceY = ghostPiece->getY();
    
    // Draw each cell of ghost piece with transparency
    for (int row = 0; row < ghostPiece->getHeight(); ++row) {
        for (int col = 0; col < ghostPiece->getWidth(); ++col) {
            if (shape[row][col]) {
                float cellX = x + (pieceX + col) * cellSize;
                float cellY = y + (pieceY + row) * cellSize;
                drawCell(window, cellX, cellY, cellSize, getSFMLColor(ghostPiece->getColor(), true));
            }
        }
    }
}

/**
 * Converts game Color enum to SFML Color.
 */
sf::Color BoardRenderer::getSFMLColor(Color color, bool isGhost) const {
    sf::Color sfColor;
    
    switch(color) {
        case Color::BLACK:    sfColor = sf::Color::Black; break;
        case Color::CYAN:     sfColor = sf::Color::Cyan; break;
        case Color::YELLOW:   sfColor = sf::Color::Yellow; break;
        case Color::PURPLE:   sfColor = sf::Color(128, 0, 128); break;
        case Color::ORANGE:   sfColor = sf::Color(255, 165, 0); break;
        case Color::BLUE:     sfColor = sf::Color::Blue; break;
        case Color::GREEN:    sfColor = sf::Color::Green; break;
        case Color::RED:      sfColor = sf::Color::Red; break;
        case Color::GRAY:     sfColor = sf::Color(128, 128, 128); break;
        default:              sfColor = sf::Color::White; break;
    }
    
    // Apply transparency for ghost pieces
    if (isGhost) {
        sfColor.a = 127; // Semi-transparent (50%)
    }
    
    return sfColor;
}

/**
 * Draws a single board cell.
 */
void BoardRenderer::drawCell(sf::RenderWindow& window, float x, float y, float size,
                            sf::Color color, bool withBorder) {
    sf::RectangleShape cell(sf::Vector2f(size, size));
    cell.setPosition(x, y);
    cell.setFillColor(color);
    
    // Add white border to cells
    if (withBorder) {
        cell.setOutlineThickness(1.0f);
        cell.setOutlineColor(sf::Color::White);
    }
    
    window.draw(cell);
}

/**
 * Draws board grid and background.
 */
void BoardRenderer::drawGrid(sf::RenderWindow& window, const ClassicBoard& board,
                            float x, float y, float cellSize) {
    // Draw board background
    sf::RectangleShape background(sf::Vector2f(
        board.getWidth() * cellSize,
        board.getHeight() * cellSize
    ));
    background.setPosition(x, y);
    background.setFillColor(sf::Color(20, 20, 30)); // Dark blue background
    background.setOutlineThickness(2.0f);
    background.setOutlineColor(sf::Color(50, 50, 70)); // Slightly lighter border
    window.draw(background);
    
    // Draw grid lines
    sf::Color gridColor(40, 40, 60); // Subtle grid color
    
    // Vertical grid lines
    for (int col = 0; col <= board.getWidth(); ++col) {
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(x + col * cellSize, y), gridColor),
            sf::Vertex(sf::Vector2f(x + col * cellSize, y + board.getHeight() * cellSize), gridColor)
        };
        window.draw(line, 2, sf::Lines);
    }
    
    // Horizontal grid lines
    for (int row = 0; row <= board.getHeight(); ++row) {
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(x, y + row * cellSize), gridColor),
            sf::Vertex(sf::Vector2f(x + board.getWidth() * cellSize, y + row * cellSize), gridColor)
        };
        window.draw(line, 2, sf::Lines);
    }
}
