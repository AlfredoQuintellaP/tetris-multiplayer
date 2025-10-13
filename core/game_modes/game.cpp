#include "game.hpp"
#include <iostream>
// #include <sstream>

Game::Game() 
    : window(sf::VideoMode(600, 600), "Tetris - POO")
    , board()
    , renderer(board)
    , fallSpeed(1.0f)
    , gameRunning(true) {
    
    std::vector<std::string> fontPaths = {
        "/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf",
        "/usr/share/fonts/truetype/liberation/LiberationSans-Bold.ttf",
        "/usr/share/fonts/truetype/freefont/FreeSansBold.ttf",
        "/usr/share/fonts/truetype/ubuntu/Ubuntu-B.ttf"
    };
    
    bool fontLoaded = false;
    for (const auto& path : fontPaths) {
        if (font.loadFromFile(path)) {
            fontLoaded = true;
            std::cout << "Used font: " << path << std::endl;
            break;
        }
    }
    
    // Usa fonte padrao do SFML se nenhuma for encontrada
    if (!fontLoaded) {
        std::cout << "No font found, using default" << std::endl;
    }
    
    board.spawnPiece();
}

// Mudei um pouquinho pq o game over tava crashando, melhorar dps
void Game::run() {
    while (window.isOpen()) {
        handleEvents();
        
        if (gameRunning) {
            update();
        }
        
        render();
    }
}

void Game::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        
        if (event.type == sf::Event::KeyPressed) {
            switch (event.key.code) {
                case sf::Keyboard::Left:
                    board.moveLeft();
                    break;
                case sf::Keyboard::Right:
                    board.moveRight();
                    break;
                case sf::Keyboard::Down:
                    board.moveDown();
                    break;
                case sf::Keyboard::Up:
                    board.rotate();
                    break;
                case sf::Keyboard::Space:
                    board.hardDrop();
                    break;
                case sf::Keyboard::R:
                    board.reset();
                    board.spawnPiece();
                    gameRunning = true;
                    break;
                case sf::Keyboard::Escape:
                    window.close();
                    break;
                default:
                    break;
            }
        }
    }
}

void Game::update() {
    // Queda automatica baseada no nível
    float currentFallSpeed = std::max(0.05f, 1.0f - (board.getLevel() * 0.1f));
    
    if (fallClock.getElapsedTime().asSeconds() > currentFallSpeed) {
        board.moveDown();
        fallClock.restart();
    }
    
    // Verifica game over
    if (board.isGameOver()) {
        gameRunning = false;
        std::cout << "Game Over! Score: " << board.getScore() << std::endl;
    }
}

void Game::render() {
    window.clear(sf::Color::Black);
    
    renderer.draw(window, 25, 50, 50);
    
    drawUI();
    
    window.display();
}

void Game::drawUI() {
    // --- Sidebar background ---
    sf::RectangleShape sidebar(sf::Vector2f(200, 600));
    sidebar.setPosition(400, 0);
    sidebar.setFillColor(sf::Color(30, 30, 40));
    sidebar.setOutlineThickness(2);
    sidebar.setOutlineColor(sf::Color(70, 70, 90));
    window.draw(sidebar);

    // --- Titulo ---
    sf::Text titleText;
    titleText.setFont(font);
    titleText.setString("TETRIS");
    titleText.setCharacterSize(28);
    titleText.setFillColor(sf::Color::Yellow);
    titleText.setStyle(sf::Text::Bold);
    titleText.setPosition(420, 20);
    window.draw(titleText);

    // --- Score Panel ---
    drawPanel(410, 70, 180, 80, "SCORE");
    
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setString(std::to_string(board.getScore()));
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(420, 110);
    window.draw(scoreText);

    // --- Level Panel ---
    drawPanel(410, 170, 180, 60, "LEVEL");
    
    sf::Text levelText;
    levelText.setFont(font);
    levelText.setString(std::to_string(board.getLevel()));
    levelText.setCharacterSize(20);
    levelText.setFillColor(sf::Color::Cyan);
    levelText.setPosition(420, 195);
    window.draw(levelText);

    // --- Lines Panel ---
    drawPanel(410, 250, 180, 60, "LINES");
    
    sf::Text linesText;
    linesText.setFont(font);
    linesText.setString(std::to_string(board.getLines()));
    linesText.setCharacterSize(20);
    linesText.setFillColor(sf::Color::Green);
    linesText.setPosition(420, 275);
    window.draw(linesText);

    // --- Next Piece Panel ---
    drawPanel(410, 330, 180, 120, "NEXT PIECE");
    drawNextPiece();

    // --- Game Over Overlay ---
    if (!gameRunning) {
        drawGameOver();
    }
}

void Game::drawPanel(float x, float y, float width, float height, const std::string& title) {
    // Fundo do painel
    sf::RectangleShape panel(sf::Vector2f(width, height));
    panel.setPosition(x, y);
    panel.setFillColor(sf::Color(40, 40, 50, 200));
    panel.setOutlineThickness(1);
    panel.setOutlineColor(sf::Color(80, 80, 100));
    window.draw(panel);

    // Titulo do painel
    sf::Text panelTitle;
    panelTitle.setFont(font);
    panelTitle.setString(title);
    panelTitle.setCharacterSize(14);
    panelTitle.setFillColor(sf::Color(200, 200, 200));
    panelTitle.setPosition(x + 10, y + 8);
    window.draw(panelTitle);
}

void Game::drawNextPiece() {
    if (board.getNextPiece()) {
        auto nextPiece = board.getNextPiece();
        auto nextShape = nextPiece->getShape();
        
        // Centraliza a peça no painel
        int pieceWidth = nextPiece->getWidth();
        int pieceHeight = nextPiece->getHeight();
        int startX = 450 - (pieceWidth * 12);
        int startY = 370;
        
        sf::RectangleShape block(sf::Vector2f(22, 22));
        
        for (int i = 0; i < pieceHeight; i++) {
            for (int j = 0; j < pieceWidth; j++) {
                if (nextShape[i][j]) {
                    block.setPosition(startX + j * 24, startY + i * 24);
                    block.setFillColor(renderer.getSFMLColor(nextPiece->getColor()));
                    block.setOutlineThickness(1);
                    block.setOutlineColor(sf::Color::White);
                    window.draw(block);
                }
            }
        }
    }
}

void Game::drawGameOver() {
    // Overlay semi-transparente
    sf::RectangleShape overlay(sf::Vector2f(400, 600));
    overlay.setPosition(0, 0);
    overlay.setFillColor(sf::Color(0, 0, 0, 150));
    window.draw(overlay);
    
    // Painel de Game Over
    sf::RectangleShape gameOverPanel(sf::Vector2f(300, 200));
    gameOverPanel.setPosition(50, 200);
    gameOverPanel.setFillColor(sf::Color(30, 30, 40));
    gameOverPanel.setOutlineThickness(3);
    gameOverPanel.setOutlineColor(sf::Color::Red);
    window.draw(gameOverPanel);
    
    // Texto GAME OVER
    sf::Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setString("GAME OVER");
    gameOverText.setCharacterSize(32);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setStyle(sf::Text::Bold);
    gameOverText.setPosition(120, 230);
    window.draw(gameOverText);
    
    // Score final
    sf::Text finalScoreText;
    finalScoreText.setFont(font);
    finalScoreText.setString("Final Score: " + std::to_string(board.getScore()));
    finalScoreText.setCharacterSize(20);
    finalScoreText.setFillColor(sf::Color::White);
    finalScoreText.setPosition(130, 290);
    window.draw(finalScoreText);
    
    // Instrucaoo para reiniciar
    sf::Text restartText;
    restartText.setFont(font);
    restartText.setString("Press R to restart");
    restartText.setCharacterSize(16);
    restartText.setFillColor(sf::Color::Yellow);
    restartText.setPosition(150, 340);
    window.draw(restartText);
}
