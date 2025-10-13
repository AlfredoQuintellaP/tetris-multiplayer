#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "classic_board.hpp"
#include "../rendering/board_renderer.hpp"

class Game {
private:
    sf::RenderWindow window;
    ClassicBoard board;
    BoardRenderer renderer;
    sf::Clock gameClock;
    sf::Clock fallClock;
    float fallSpeed;
    bool gameRunning;
    sf::Font font;  
    
public:
    Game();
    void run();
    void handleEvents();
    void update();
    void render();
    void drawUI();
    
private:
    void drawPanel(float x, float y, float width, float height, const std::string& title);
    void drawNextPiece();
    void drawGameOver();
};

#endif
