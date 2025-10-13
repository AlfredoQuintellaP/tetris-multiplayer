#include "core/game_modes/game.hpp"
#include <iostream>

int main() {
    try {
        Game game;
        game.run();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Erro: " << e.what() << std::endl;
        return 1;
    }
}
