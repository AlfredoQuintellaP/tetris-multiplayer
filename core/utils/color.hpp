#ifndef COLOR_H
#define COLOR_H

/**
 * Color enumeration for Tetris game pieces and board cells.
 * Each color corresponds to a different Tetromino type.
 */
enum class Color {
    BLACK,    // Empty cell
    CYAN,     // I-piece
    YELLOW,   // O-piece  
    PURPLE,   // T-piece
    ORANGE,   // L-piece
    BLUE,     // J-piece
    GREEN,    // S-piece
    RED,      // Z-piece
    GRAY      // Ghost piece (semi-transparent)
};

#endif
