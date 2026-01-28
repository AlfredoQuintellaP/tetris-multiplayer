#ifndef BOARD_RENDERER_H
#define BOARD_RENDERER_H

#include <SFML/Graphics.hpp>
#include "../model/board.hpp"

/**
 * Renders Tetris game boards and Tetromino pieces.
 * Handles drawing of board grid, placed pieces, current piece, and ghost piece.
 */
class BoardRenderer {
public:
    /**
     * Renders a complete Tetris board including grid, placed pieces, current piece, and ghost piece.
     * 
     * @param window SFML render window
     * @param board Game board to render
     * @param x X-coordinate for top-left corner of board
     * @param y Y-coordinate for top-left corner of board
     * @param cellSize Size of each board cell in pixels (default: 30)
     */
    void render(sf::RenderWindow& window, const ClassicBoard& board, 
                float x, float y, float cellSize = 30.0f);
    
    /**
     * Renders a single Tetromino piece (used for next/hold panels).
     * 
     * @param window SFML render window
     * @param piece Tetromino to render
     * @param x X-coordinate for top-left corner of piece
     * @param y Y-coordinate for top-left corner of piece
     * @param cellSize Size of each cell in pixels (default: 20)
     * @param alpha Transparency value (1.0 = opaque, 0.0 = transparent)
     */
    void renderPiece(sf::RenderWindow& window, const Tetromino& piece,
                     float x, float y, float cellSize = 20.0f, float alpha = 1.0f);
    
    /**
     * Converts game Color enum to SFML Color with optional ghost effect.
     * 
     * @param color Game color enum
     * @param isGhost Whether to apply ghost transparency
     * @return SFML Color object
     */
    sf::Color getSFMLColor(Color color, bool isGhost = false) const;
    
private:
    /**
     * Renders the ghost piece (preview of where current piece will land).
     * 
     * @param window SFML render window
     * @param board Game board containing ghost piece
     * @param x X-coordinate for top-left corner of board
     * @param y Y-coordinate for top-left corner of board
     * @param cellSize Size of each board cell in pixels
     */
    void renderGhostPiece(sf::RenderWindow& window, const ClassicBoard& board,
                         float x, float y, float cellSize);
    
    /**
     * Draws a single cell on the board.
     * 
     * @param window SFML render window
     * @param x X-coordinate for top-left corner of cell
     * @param y Y-coordinate for top-left corner of cell
     * @param size Width and height of cell in pixels
     * @param color Fill color of cell
     * @param withBorder Whether to draw a border around the cell
     */
    void drawCell(sf::RenderWindow& window, float x, float y, float size,
                  sf::Color color, bool withBorder = true);
    
    /**
     * Draws the board grid lines and background.
     * 
     * @param window SFML render window
     * @param board Game board reference
     * @param x X-coordinate for top-left corner of board
     * @param y Y-coordinate for top-left corner of board
     * @param cellSize Size of each board cell in pixels
     */
    void drawGrid(sf::RenderWindow& window, const ClassicBoard& board,
                  float x, float y, float cellSize);
};

#endif
