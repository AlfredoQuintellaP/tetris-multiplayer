#include "tetromino.hpp"

/**
 * Creates a deep copy of this Tetromino piece.
 */
std::unique_ptr<Tetromino> Tetromino::clone() const {
    auto copy = Tetromino::create(type);
    copy->shape = shape;
    copy->color = color;
    copy->x = x;
    copy->y = y;
    return copy;
}

/**
 * Rotates the piece 90 degrees clockwise.
 */
void Tetromino::rotate() {
    if (shape.empty() || shape[0].empty())
        return;

    int rows = shape.size();
    int cols = shape[0].size();
    
    // Create rotated matrix (transposed and reversed columns)
    std::vector<std::vector<bool>> rotated(cols, std::vector<bool>(rows));
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            rotated[j][rows - 1 - i] = shape[i][j];
        }
    }
    
    shape = rotated;
}

/**
 * Creates I-piece: 4 blocks in a vertical line.
 */
IPiece::IPiece() {
    type = TetrominoType::I;
    color = Color::CYAN;
    shape = {{1, 1, 1, 1}}; // 1x4 matrix
}

/**
 * Creates O-piece: 2x2 square block.
 */
OPiece::OPiece() {
    type = TetrominoType::O;
    color = Color::YELLOW;
    shape = {{1, 1},
             {1, 1}}; // 2x2 matrix
}

/**
 * Creates T-piece: T-shaped block.
 */
TPiece::TPiece() {
    type = TetrominoType::T;
    color = Color::PURPLE;
    shape = {{0, 1, 0},
             {1, 1, 1}}; // 2x3 matrix
}

/**
 * Creates L-piece: L-shaped block.
 */
LPiece::LPiece() {
    type = TetrominoType::L;
    color = Color::ORANGE;
    shape = {{1, 0},
             {1, 0},
             {1, 1}}; // 3x2 matrix
}

/**
 * Creates J-piece: Mirrored L-shaped block.
 */
JPiece::JPiece() {
    type = TetrominoType::J;
    color = Color::BLUE;
    shape = {{0, 1},
             {0, 1},
             {1, 1}}; // 3x2 matrix
}

/**
 * Creates S-piece: S-shaped block.
 */
SPiece::SPiece() {
    type = TetrominoType::S;
    color = Color::GREEN;
    shape = {{0, 1, 1},
             {1, 1, 0}}; // 2x3 matrix
}

/**
 * Creates Z-piece: Z-shaped block.
 */
ZPiece::ZPiece() {
    type = TetrominoType::Z;
    color = Color::RED;
    shape = {{1, 1, 0},
             {0, 1, 1}}; // 2x3 matrix
}

/**
 * Factory method implementation.
 */
std::unique_ptr<Tetromino> Tetromino::create(TetrominoType type) {
    switch(type) {
        case TetrominoType::I: return std::make_unique<IPiece>();
        case TetrominoType::O: return std::make_unique<OPiece>();
        case TetrominoType::T: return std::make_unique<TPiece>();
        case TetrominoType::L: return std::make_unique<LPiece>();
        case TetrominoType::J: return std::make_unique<JPiece>();
        case TetrominoType::S: return std::make_unique<SPiece>();
        case TetrominoType::Z: return std::make_unique<ZPiece>();
        default: return std::make_unique<IPiece>(); // Fallback
    }
}
