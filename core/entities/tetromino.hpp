#ifndef TETROMINO_H
#define TETROMINO_H

#include <vector>
#include <memory>
#include "../utils/color.hpp"

enum class TetrominoType { I, O, T, L, J, S, Z };

class Tetromino {
protected:
    std::vector<std::vector<bool>> shape;
    Color color;
    int x, y;
    TetrominoType type;
    
public:
    Tetromino() : x(0), y(0) {}
    virtual ~Tetromino() = default;

    // Factory method, big bizuuu
    static std::unique_ptr<Tetromino> create(TetrominoType type);

    // Clone method, second big bizuuu
    virtual std::unique_ptr<Tetromino> clone() const;

    virtual void rotate();

    void setPosition(int newX, int newY) { x = newX; y = newY; }
    void move(int dx, int dy) { x += dx; y += dy; }

    // Getters
    const std::vector<std::vector<bool>>& getShape() const { return shape; }
    Color getColor() const { return color; }
    int getX() const { return x; }
    int getY() const { return y; }
    TetrominoType getType() const { return type; }
    int getWidth() const { return shape.empty() ? 0 : shape[0].size(); }
    int getHeight() const { return shape.size(); }

protected:
    void setType(TetrominoType newType) { type = newType; }
    void setColor(Color newColor) { color = newColor; }
    void setShape(const std::vector<std::vector<bool>>& newShape) { shape = newShape; }
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
