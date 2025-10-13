#include "tetromino.hpp"

std::unique_ptr<Tetromino> Tetromino::clone() const {
    auto copy = Tetromino::create(type);
    copy->shape = shape;
    copy->color = color;
    copy->x = x;
    copy->y = y;
    return copy;
}

void Tetromino::rotate() {
    if (shape.empty() || shape[0].empty())
        return;

    std::vector<std::vector<bool>> rotated(shape[0].size(), 
                                          std::vector<bool>(shape.size()));
    
    for (size_t i = 0; i < shape.size(); i++) {
        for (size_t j = 0; j < shape[i].size(); j++) {
            rotated[j][shape.size() - 1 - i] = shape[i][j];
        }
    }
    shape = rotated;
}

IPiece::IPiece() {
    type = TetrominoType::I;
    color = Color::CYAN;
    shape = {{1, 1, 1, 1}};
}

OPiece::OPiece() {
    type = TetrominoType::O;
    color = Color::YELLOW;
    shape = {{1, 1},
             {1, 1}};
}

TPiece::TPiece() {
    type = TetrominoType::T;
    color = Color::PURPLE;
    shape = {{0, 1, 0},
             {1, 1, 1}};
}

LPiece::LPiece() {
    type = TetrominoType::L;
    color = Color::ORANGE;
    shape = {{1, 0},
             {1, 0},
             {1, 1}};
}

JPiece::JPiece() {
    type = TetrominoType::J;
    color = Color::BLUE;
    shape = {{0, 1},
             {0, 1},
             {1, 1}};
}

SPiece::SPiece() {
    type = TetrominoType::S;
    color = Color::GREEN;
    shape = {{0, 1, 1},
             {1, 1, 0}};
}

ZPiece::ZPiece() {
    type = TetrominoType::Z;
    color = Color::RED;
    shape = {{1, 1, 0},
             {0, 1, 1}};
}

std::unique_ptr<Tetromino> Tetromino::create(TetrominoType type) {
    switch(type) {
        case TetrominoType::I: return std::make_unique<IPiece>();
        case TetrominoType::O: return std::make_unique<OPiece>();
        case TetrominoType::T: return std::make_unique<TPiece>();
        case TetrominoType::L: return std::make_unique<LPiece>();
        case TetrominoType::J: return std::make_unique<JPiece>();
        case TetrominoType::S: return std::make_unique<SPiece>();
        case TetrominoType::Z: return std::make_unique<ZPiece>();
        default: return std::make_unique<IPiece>();
    }
}
