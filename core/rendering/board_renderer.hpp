#ifndef BOARD_RENDERER_H
#define BOARD_RENDERER_H

#include <SFML/Graphics.hpp>
#include "../game_modes/classic_board.hpp"

class BoardRenderer {
private:
    const ClassicBoard& board;
    
public:
    BoardRenderer(const ClassicBoard& gameBoard) : board(gameBoard) {}
    
    // Conversão de cores
    sf::Color getSFMLColor(Color color, bool isGhost = false) const;
    
    void draw(sf::RenderWindow& window, int blockSize, int offsetX, int offsetY) const;
    void drawBlock(sf::RenderWindow& window, int x, int y, Color color, 
                   int blockSize, int offsetX, int offsetY, bool isGhost = false) const;
    void drawGrid(sf::RenderWindow& window, int blockSize, int offsetX, int offsetY) const;
    void drawPiece(sf::RenderWindow& window, const Tetromino* piece, 
                   int blockSize, int offsetX, int offsetY, bool isGhost = false) const;
};

#endif
