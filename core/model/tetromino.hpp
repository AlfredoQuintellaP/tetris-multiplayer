#ifndef TETROMINO_H
#define TETROMINO_H

#include <vector>
#include <memory>
#include "../utils/color.hpp"

/**
 * Types of Tetromino pieces in classic Tetris.
 */
enum class TetrominoType { I, O, T, L, J, S, Z };

/**
 * Base class representing a Tetromino piece.
 * Handles piece shape, color, position, and rotation.
 */
class Tetromino {
public:
    Tetromino() : x(0), y(0) {}
    virtual ~Tetromino() = default;

    /**
     * Factory method to create Tetromino pieces by type.
     * 
     * @param type Type of Tetromino to create
     * @return Unique pointer to the created Tetromino
     */
    static std::unique_ptr<Tetromino> create(TetrominoType type);

    /**
     * Creates a deep copy of this Tetromino.
     * 
     * @return Unique pointer to the cloned Tetromino
     */
    virtual std::unique_ptr<Tetromino> clone() const;

    /**
     * Rotates the piece 90 degrees clockwise.
     */
    virtual void rotate();

    // Position manipulation
    void setPosition(int newX, int newY) { x = newX; y = newY; }
    void move(int dx, int dy) { x += dx; y += dy; }

    // Accessors
    const std::vector<std::vector<bool>>& getShape() const { return shape; }
    Color getColor() const { return color; }
    int getX() const { return x; }
    int getY() const { return y; }
    TetrominoType getType() const { return type; }
    int getWidth() const { return shape.empty() ? 0 : shape[0].size(); }
    int getHeight() const { return shape.size(); }

protected:
    // Protected setters for derived classes
    void setType(TetrominoType newType) { type = newType; }
    void setColor(Color newColor) { color = newColor; }
    void setShape(const std::vector<std::vector<bool>>& newShape) { shape = newShape; }

    std::vector<std::vector<bool>> shape; // Piece shape matrix
    Color color;                          // Piece color
    int x, y;                             // Board position
    TetrominoType type;                   // Piece type
};

class IPiece : public Tetromino {
public:
    IPiece();
};

class OPiece : public Tetromino {
public:
    OPiece();
};

class TPiece : public Tetromino {
public:
    TPiece();
};

class LPiece : public Tetromino {
public:
    LPiece();
};

class JPiece : public Tetromino {
public:
    JPiece();
};

class SPiece : public Tetromino {
public:
    SPiece();
};

class ZPiece : public Tetromino {
public:
    ZPiece();
};

#endif
