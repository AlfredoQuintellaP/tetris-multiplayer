#include "game.hpp"
#include <iostream>

Game::Game() 
    : window(sf::VideoMode(800, 600), "Tetris - POO")
    , board()
    , renderer(board)
    , board2()
    , renderer2(board2)
    , fallSpeed(1.0f)
    , currentState(GameState::MAIN_MENU)
    , selectedMenuOption(MenuOption::SINGLE_PLAYER)
    , selectedOnlineOption(OnlineMenuOption::HOST_GAME)
    , inputIP("")
    , inputtingIP(false) {
    
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
    
    if (!fontLoaded) {
        std::cout << "No font found, using default" << std::endl;
    }
}

void Game::run() {
    while (window.isOpen()) {
        handleEvents();
        update();
        render();
    }
}

void Game::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        
        switch (currentState) {
            case GameState::MAIN_MENU:
                handleMenuEvents(event);
                break;
            case GameState::SINGLE_PLAYER:
            case GameState::GAME_OVER:
                handleSinglePlayerEvents(event);
                break;
            case GameState::MULTIPLAYER_LOCAL:
                handleMultiplayerLocalEvents(event);
                break;
            case GameState::MULTIPLAYER_ONLINE_LOBBY:
                handleOnlineLobbyEvents(event);
                break;
            case GameState::MULTIPLAYER_ONLINE_GAME:
                handleMultiplayerOnlineEvents(event);
                break;
            default:
                break;
        }
    }
}

void Game::handleMenuEvents(sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
            case sf::Keyboard::Up:
                selectedMenuOption = static_cast<MenuOption>(
                    (static_cast<int>(selectedMenuOption) - 1 + static_cast<int>(MenuOption::COUNT)) 
                    % static_cast<int>(MenuOption::COUNT)
                );
                break;
            case sf::Keyboard::Down:
                selectedMenuOption = static_cast<MenuOption>(
                    (static_cast<int>(selectedMenuOption) + 1) % static_cast<int>(MenuOption::COUNT)
                );
                break;
            case sf::Keyboard::Return:
            case sf::Keyboard::Space:
                switch (selectedMenuOption) {
                    case MenuOption::SINGLE_PLAYER:
                        currentState = GameState::SINGLE_PLAYER;
                        window.setSize(sf::Vector2u(600, 600));
                        board.reset();
                        board.spawnPiece();
                        break;
                    case MenuOption::MULTIPLAYER_LOCAL:
                        currentState = GameState::MULTIPLAYER_LOCAL;
                        window.setSize(sf::Vector2u(900, 600));
                        board.reset();
                        board2.reset();
                        board.spawnPiece();
                        board2.spawnPiece();
                        break;
                    case MenuOption::MULTIPLAYER_ONLINE:
                        currentState = GameState::MULTIPLAYER_ONLINE_LOBBY;
                        window.setSize(sf::Vector2u(800, 600));
                        break;
                    case MenuOption::EXIT:
                        window.close();
                        break;
                    default:
                        break;
                }
                break;
            case sf::Keyboard::Escape:
                window.close();
                break;
            default:
                break;
        }
    }
}

void Game::handleOnlineLobbyEvents(sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (inputtingIP) {
            // Handle IP input
            if (event.key.code == sf::Keyboard::Return) {
                if (!inputIP.empty()) {
                    std::cout << "Connecting to: " << inputIP << std::endl;
                    if (network.connectToServer(inputIP)) {
                        currentState = GameState::MULTIPLAYER_ONLINE_GAME;
                        window.setSize(sf::Vector2u(900, 600));
                        board.reset();
                        board2.reset();
                        board.spawnPiece();
                        inputtingIP = false;
                    } else {
                        std::cout << "Connection failed!" << std::endl;
                        inputIP.clear();
                        inputtingIP = false;
                    }
                }
            } else if (event.key.code == sf::Keyboard::Escape) {
                inputIP.clear();
                inputtingIP = false;
            } else if (event.key.code == sf::Keyboard::BackSpace) {
                if (!inputIP.empty()) {
                    inputIP.pop_back();
                }
            }
        } else {
            // Handle menu selection
            switch (event.key.code) {
                case sf::Keyboard::Up:
                    selectedOnlineOption = static_cast<OnlineMenuOption>(
                        (static_cast<int>(selectedOnlineOption) - 1 + static_cast<int>(OnlineMenuOption::COUNT)) 
                        % static_cast<int>(OnlineMenuOption::COUNT)
                    );
                    break;
                case sf::Keyboard::Down:
                    selectedOnlineOption = static_cast<OnlineMenuOption>(
                        (static_cast<int>(selectedOnlineOption) + 1) % static_cast<int>(OnlineMenuOption::COUNT)
                    );
                    break;
                case sf::Keyboard::Return:
                case sf::Keyboard::Space:
                    switch (selectedOnlineOption) {
                        case OnlineMenuOption::HOST_GAME:
                            if (network.startServer()) {
                                std::cout << "Waiting for opponent..." << std::endl;
                            }
                            break;
                        case OnlineMenuOption::JOIN_GAME:
                            inputtingIP = true;
                            inputIP.clear();
                            break;
                        case OnlineMenuOption::BACK:
                            currentState = GameState::MAIN_MENU;
                            break;
                        default:
                            break;
                    }
                    break;
                case sf::Keyboard::Escape:
                    currentState = GameState::MAIN_MENU;
                    break;
                default:
                    break;
            }
        }
    } else if (event.type == sf::Event::TextEntered && inputtingIP) {
        if (event.text.unicode < 128 && event.text.unicode != 8 && event.text.unicode != 13) {
            char c = static_cast<char>(event.text.unicode);
            if ((c >= '0' && c <= '9') || c == '.') {
                inputIP += c;
            }
        }
    }
}

void Game::handleSinglePlayerEvents(sf::Event& event) {
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
            case sf::Keyboard::C:
                board.hold();
                break;
            case sf::Keyboard::R:
                board.reset();
                board.spawnPiece();
                currentState = GameState::SINGLE_PLAYER;
                break;
            case sf::Keyboard::Escape:
                currentState = GameState::MAIN_MENU;
                window.setSize(sf::Vector2u(800, 600));
                break;
            default:
                break;
        }
    }
}

void Game::handleMultiplayerLocalEvents(sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
            // Player 1 controls
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
            case sf::Keyboard::RShift:
                board.hardDrop();
                break;
            case sf::Keyboard::RControl:
                board.hold();
                break;
                
            // Player 2 controls
            case sf::Keyboard::A:
                board2.moveLeft();
                break;
            case sf::Keyboard::D:
                board2.moveRight();
                break;
            case sf::Keyboard::S:
                board2.moveDown();
                break;
            case sf::Keyboard::W:
                board2.rotate();
                break;
            case sf::Keyboard::LShift:
                board2.hardDrop();
                break;
            case sf::Keyboard::LControl:
                board2.hold();
                break;
                
            case sf::Keyboard::R:
                board.reset();
                board2.reset();
                board.spawnPiece();
                board2.spawnPiece();
                break;
            case sf::Keyboard::Escape:
                currentState = GameState::MAIN_MENU;
                window.setSize(sf::Vector2u(800, 600));
                break;
            default:
                break;
        }
    }
}

void Game::handleMultiplayerOnlineEvents(sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        NetworkMessage msg;
        msg.playerID = network.getIsHost() ? 1 : 2;
        msg.data1 = 0;
        msg.data2 = 0;
        
        switch (event.key.code) {
            case sf::Keyboard::Left:
                board.moveLeft();
                msg.type = NetworkMessageType::MOVE_LEFT;
                network.sendMessage(msg);
                break;
            case sf::Keyboard::Right:
                board.moveRight();
                msg.type = NetworkMessageType::MOVE_RIGHT;
                network.sendMessage(msg);
                break;
            case sf::Keyboard::Down:
                board.moveDown();
                msg.type = NetworkMessageType::MOVE_DOWN;
                network.sendMessage(msg);
                break;
            case sf::Keyboard::Up:
                board.rotate();
                msg.type = NetworkMessageType::ROTATE;
                network.sendMessage(msg);
                break;
            case sf::Keyboard::Space:
                board.hardDrop();
                msg.type = NetworkMessageType::HARD_DROP;
                network.sendMessage(msg);
                break;
            case sf::Keyboard::C:
                board.hold();
                msg.type = NetworkMessageType::HOLD;
                network.sendMessage(msg);
                break;
            case sf::Keyboard::Escape:
                network.disconnect();
                currentState = GameState::MAIN_MENU;
                window.setSize(sf::Vector2u(800, 600));
                break;
            default:
                break;
        }
    }
}

void Game::update() {
    switch (currentState) {
        case GameState::SINGLE_PLAYER:
            updateSinglePlayer();
            break;
        case GameState::MULTIPLAYER_LOCAL:
            updateMultiplayerLocal();
            break;
        case GameState::MULTIPLAYER_ONLINE_LOBBY:
            // Check if client connected (for host)
            if (network.getIsHost() && network.waitForClient()) {
                currentState = GameState::MULTIPLAYER_ONLINE_GAME;
                window.setSize(sf::Vector2u(900, 600));
                board.reset();
                board2.reset();
                board.spawnPiece();
            }
            break;
        case GameState::MULTIPLAYER_ONLINE_GAME:
            updateMultiplayerOnline();
            break;
        default:
            break;
    }
}

void Game::updateSinglePlayer() {
    if (board.isGameOver()) {
        currentState = GameState::GAME_OVER;
        return;
    }
    
    float currentFallSpeed = std::max(0.05f, 1.0f - (board.getLevel() * 0.1f));
    
    if (fallClock.getElapsedTime().asSeconds() > currentFallSpeed) {
        board.moveDown();
        fallClock.restart();
    }
}

void Game::updateMultiplayerLocal() {
    float currentFallSpeed1 = std::max(0.05f, 1.0f - (board.getLevel() * 0.1f));
    float currentFallSpeed2 = std::max(0.05f, 1.0f - (board2.getLevel() * 0.1f));
    
    if (fallClock.getElapsedTime().asSeconds() > std::min(currentFallSpeed1, currentFallSpeed2)) {
        if (!board.isGameOver()) board.moveDown();
        if (!board2.isGameOver()) board2.moveDown();
        fallClock.restart();
    }
}

void Game::updateMultiplayerOnline() {
    // Receive opponent's moves
    receiveGameState();
    
    // Update local board
    float currentFallSpeed = std::max(0.05f, 1.0f - (board.getLevel() * 0.1f));
    if (fallClock.getElapsedTime().asSeconds() > currentFallSpeed) {
        board.moveDown();
        sendGameState();
        fallClock.restart();
    }
}

void Game::sendGameState() {
    NetworkMessage msg;
    msg.type = NetworkMessageType::BOARD_STATE;
    msg.playerID = network.getIsHost() ? 1 : 2;
    msg.data1 = board.getScore();
    msg.data2 = board.getLines();
    network.sendMessage(msg);
}

void Game::receiveGameState() {
    NetworkMessage msg;
    while (network.receiveMessage(msg)) {
        switch (msg.type) {
            case NetworkMessageType::MOVE_LEFT:
                board2.moveLeft();
                break;
            case NetworkMessageType::MOVE_RIGHT:
                board2.moveRight();
                break;
            case NetworkMessageType::MOVE_DOWN:
                board2.moveDown();
                break;
            case NetworkMessageType::ROTATE:
                board2.rotate();
                break;
            case NetworkMessageType::HARD_DROP:
                board2.hardDrop();
                break;
            case NetworkMessageType::HOLD:
                board2.hold();
                break;
            case NetworkMessageType::DISCONNECT:
                std::cout << "Opponent disconnected" << std::endl;
                currentState = GameState::MAIN_MENU;
                network.disconnect();
                break;
            default:
                break;
        }
    }
}

void Game::render() {
    window.clear(sf::Color::Black);
    
    switch (currentState) {
        case GameState::MAIN_MENU:
            renderMainMenu();
            break;
        case GameState::SINGLE_PLAYER:
        case GameState::GAME_OVER:
            renderSinglePlayer();
            break;
        case GameState::MULTIPLAYER_LOCAL:
            renderMultiplayerLocal();
            break;
        case GameState::MULTIPLAYER_ONLINE_LOBBY:
            renderOnlineLobby();
            break;
        case GameState::MULTIPLAYER_ONLINE_GAME:
            renderMultiplayerOnline();
            break;
        default:
            break;
    }
    
    window.display();
}

void Game::renderMainMenu() {
    sf::Text title;
    title.setFont(font);
    title.setString("TETRIS");
    title.setCharacterSize(80);
    title.setFillColor(sf::Color::Cyan);
    title.setStyle(sf::Text::Bold);
    title.setPosition(250, 100);
    window.draw(title);
    
    std::vector<std::string> options = {
        "Single Player",
        "Multiplayer Local",
        "Multiplayer Online",
        "Exit"
    };
    
    for (size_t i = 0; i < options.size(); i++) {
        sf::Text option;
        option.setFont(font);
        option.setString(options[i]);
        option.setCharacterSize(30);
        
        if (static_cast<int>(selectedMenuOption) == i) {
            option.setFillColor(sf::Color::Yellow);
            option.setString("> " + options[i] + " <");
        } else {
            option.setFillColor(sf::Color::White);
        }
        
        option.setPosition(280, 250 + i * 60);
        window.draw(option);
    }
    
    sf::Text instructions;
    instructions.setFont(font);
    instructions.setString("Use Arrow Keys | Enter to select | ESC to exit");
    instructions.setCharacterSize(16);
    instructions.setFillColor(sf::Color(150, 150, 150));
    instructions.setPosition(200, 550);
    window.draw(instructions);
}

void Game::renderOnlineLobby() {
    sf::Text title;
    title.setFont(font);
    title.setString("ONLINE MULTIPLAYER");
    title.setCharacterSize(50);
    title.setFillColor(sf::Color::Cyan);
    title.setStyle(sf::Text::Bold);
    title.setPosition(150, 80);
    window.draw(title);
    
    if (inputtingIP) {
        sf::Text prompt;
        prompt.setFont(font);
        prompt.setString("Enter Server IP Address:");
        prompt.setCharacterSize(24);
        prompt.setFillColor(sf::Color::White);
        prompt.setPosition(220, 250);
        window.draw(prompt);
        
        sf::RectangleShape inputBox(sf::Vector2f(400, 50));
        inputBox.setPosition(200, 300);
        inputBox.setFillColor(sf::Color(40, 40, 50));
        inputBox.setOutlineThickness(2);
        inputBox.setOutlineColor(sf::Color::Yellow);
        window.draw(inputBox);
        
        sf::Text ipText;
        ipText.setFont(font);
        ipText.setString(inputIP + "_");
        ipText.setCharacterSize(24);
        ipText.setFillColor(sf::Color::Yellow);
        ipText.setPosition(220, 310);
        window.draw(ipText);
        
        sf::Text hint;
        hint.setFont(font);
        hint.setString("Press Enter to connect | ESC to cancel");
        hint.setCharacterSize(16);
        hint.setFillColor(sf::Color(150, 150, 150));
        hint.setPosition(230, 380);
        window.draw(hint);
    } else if (network.getIsHost() && !network.getIsConnected()) {
        sf::Text waiting;
        waiting.setFont(font);
        waiting.setString("Waiting for opponent...");
        waiting.setCharacterSize(30);
        waiting.setFillColor(sf::Color::Yellow);
        waiting.setPosition(220, 280);
        window.draw(waiting);
        
        sf::Text hint;
        hint.setFont(font);
        hint.setString("Have your opponent connect to your IP");
        hint.setCharacterSize(18);
        hint.setFillColor(sf::Color(150, 150, 150));
        hint.setPosition(210, 340);
        window.draw(hint);
    } else {
        std::vector<std::string> options = {
            "Host Game",
            "Join Game",
            "Back"
        };
        
        for (size_t i = 0; i < options.size(); i++) {
            sf::Text option;
            option.setFont(font);
            option.setString(options[i]);
            option.setCharacterSize(30);
            
            if (static_cast<int>(selectedOnlineOption) == i) {
                option.setFillColor(sf::Color::Yellow);
                option.setString("> " + options[i] + " <");
            } else {
                option.setFillColor(sf::Color::White);
            }
            
            option.setPosition(310, 230 + i * 70);
            window.draw(option);
        }
    }
}

void Game::renderMultiplayerOnline() {
    // Similar to local multiplayer but with network indicators
    drawPlayerUI(board, renderer, 20, "You");
    drawPlayerUI(board2, renderer2, 470, "Opponent");
    
    // Network status indicator
    sf::CircleShape statusIndicator(8);
    statusIndicator.setPosition(10, 10);
    statusIndicator.setFillColor(network.getIsConnected() ? sf::Color::Green : sf::Color::Red);
    window.draw(statusIndicator);
    
    sf::Text statusText;
    statusText.setFont(font);
    statusText.setString(network.getIsConnected() ? "Connected" : "Disconnected");
    statusText.setCharacterSize(14);
    statusText.setFillColor(sf::Color::White);
    statusText.setPosition(25, 5);
    window.draw(statusText);
}

void Game::renderSinglePlayer() {
    renderer.draw(window, 25, 50, 50);
    drawUI();
}

void Game::renderMultiplayerLocal() {
    drawPlayerUI(board, renderer, 20, "Player 1 (Arrows)");
    drawPlayerUI(board2, renderer2, 470, "Player 2 (WASD)");
    
    if (board.isGameOver() && board2.isGameOver()) {
        sf::RectangleShape overlay(sf::Vector2f(900, 600));
        overlay.setFillColor(sf::Color(0, 0, 0, 180));
        window.draw(overlay);
        
        sf::Text winnerText;
        winnerText.setFont(font);
        if (board.getScore() > board2.getScore()) {
            winnerText.setString("Player 1 Wins!");
            winnerText.setFillColor(sf::Color::Green);
        } else if (board2.getScore() > board.getScore()) {
            winnerText.setString("Player 2 Wins!");
            winnerText.setFillColor(sf::Color::Blue);
        } else {
            winnerText.setString("Draw!");
            winnerText.setFillColor(sf::Color::Yellow);
        }
        winnerText.setCharacterSize(50);
        winnerText.setStyle(sf::Text::Bold);
        winnerText.setPosition(300, 250);
        window.draw(winnerText);
        
        sf::Text restartText;
        restartText.setFont(font);
        restartText.setString("Press R to restart or ESC for menu");
        restartText.setCharacterSize(20);
        restartText.setFillColor(sf::Color::White);
        restartText.setPosition(250, 350);
        window.draw(restartText);
    }
}

void Game::drawPlayerUI(const ClassicBoard& playerBoard, const BoardRenderer& playerRenderer, 
                        float offsetX, const std::string& playerName) {
    playerRenderer.draw(window, 20, offsetX, 100);
    
    sf::Text nameText;
    nameText.setFont(font);
    nameText.setString(playerName);
    nameText.setCharacterSize(20);
    nameText.setFillColor(sf::Color::Yellow);
    nameText.setStyle(sf::Text::Bold);
    nameText.setPosition(offsetX, 20);
    window.draw(nameText);
    
    sf::Text scoreLabel;
    scoreLabel.setFont(font);
    scoreLabel.setString("Score:");
    scoreLabel.setCharacterSize(14);
    scoreLabel.setFillColor(sf::Color::White);
    scoreLabel.setPosition(offsetX, 50);
    window.draw(scoreLabel);
    
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setString(std::to_string(playerBoard.getScore()));
    scoreText.setCharacterSize(16);
    scoreText.setFillColor(sf::Color::Cyan);
    scoreText.setPosition(offsetX + 70, 50);
    window.draw(scoreText);
    
    sf::Text linesText;
    linesText.setFont(font);
    linesText.setString("Lines: " + std::to_string(playerBoard.getLines()));
    linesText.setCharacterSize(14);
    linesText.setFillColor(sf::Color::Green);
    linesText.setPosition(offsetX, 75);
    window.draw(linesText);
    
    if (playerBoard.isGameOver()) {
        sf::RectangleShape overlay(sf::Vector2f(220, 400));
        overlay.setPosition(offsetX, 100);
        overlay.setFillColor(sf::Color(0, 0, 0, 150));
        window.draw(overlay);
        
        sf::Text gameOverText;
        gameOverText.setFont(font);
        gameOverText.setString("GAME\nOVER");
        gameOverText.setCharacterSize(30);
        gameOverText.setFillColor(sf::Color::Red);
        gameOverText.setStyle(sf::Text::Bold);
        gameOverText.setPosition(offsetX + 50, 250);
        window.draw(gameOverText);
    }
}

// [Rest of the UI functions remain the same as before]
void Game::drawUI() {
    sf::RectangleShape sidebar(sf::Vector2f(200, 600));
    sidebar.setPosition(400, 0);
    sidebar.setFillColor(sf::Color(30, 30, 40));
    sidebar.setOutlineThickness(2);
    sidebar.setOutlineColor(sf::Color(70, 70, 90));
    window.draw(sidebar);

    sf::Text titleText;
    titleText.setFont(font);
    titleText.setString("TETRIS");
    titleText.setCharacterSize(28);
    titleText.setFillColor(sf::Color::Yellow);
    titleText.setStyle(sf::Text::Bold);
    titleText.setPosition(420, 20);
    window.draw(titleText);

    drawPanel(410, 70, 180, 80, "SCORE");
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setString(std::to_string(board.getScore()));
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(420, 110);
    window.draw(scoreText);

    drawPanel(410, 170, 180, 60, "LEVEL");
    sf::Text levelText;
    levelText.setFont(font);
    levelText.setString(std::to_string(board.getLevel()));
    levelText.setCharacterSize(20);
    levelText.setFillColor(sf::Color::Cyan);
    levelText.setPosition(420, 195);
    window.draw(levelText);

    drawPanel(410, 250, 180, 60, "LINES");
    sf::Text linesText;
    linesText.setFont(font);
    linesText.setString(std::to_string(board.getLines()));
    linesText.setCharacterSize(20);
    linesText.setFillColor(sf::Color::Green);
    linesText.setPosition(420, 275);
    window.draw(linesText);

    drawPanel(410, 330, 180, 120, "NEXT");
    drawNextPiece();

    drawPanel(410, 470, 180, 120, "HOLD (C)");
    drawHoldPiece();

    if (currentState == GameState::GAME_OVER) {
        drawGameOver();
    }
}

void Game::drawPanel(float x, float y, float width, float height, const std::string& title) {
    sf::RectangleShape panel(sf::Vector2f(width, height));
    panel.setPosition(x, y);
    panel.setFillColor(sf::Color(40, 40, 50, 200));
    panel.setOutlineThickness(1);
    panel.setOutlineColor(sf::Color(80, 80, 100));
    window.draw(panel);

    sf::Text panelTitle;
    panelTitle.setFont(font);
    panelTitle.setString(title);
    panelTitle.setCharacterSize(14);
    panelTitle.setFillColor(sf::Color(200, 200, 200));
    panelTitle.setPosition(x + 10, y + 8);
    window.draw(panelTitle);
}

void Game::drawHoldPiece() {
    if (board.getHoldPiece()) {
        auto holdPiece = board.getHoldPiece();
        auto holdShape = holdPiece->getShape();
        
        int pieceWidth = holdPiece->getWidth();
        int pieceHeight = holdPiece->getHeight();
        int startX = 450 - (pieceWidth * 12);
        int startY = 510;
        
        sf::RectangleShape block(sf::Vector2f(22, 22));
        float alpha = board.getCanHold() ? 1.0f : 0.5f;
        
        for (int i = 0; i < pieceHeight; i++) {
            for (int j = 0; j < pieceWidth; j++) {
                if (holdShape[i][j]) {
                    block.setPosition(startX + j * 24, startY + i * 24);
                    auto color = renderer.getSFMLColor(holdPiece->getColor());
                    if (!board.getCanHold()) {
                        color.r *= alpha;
                        color.g *= alpha;
                        color.b *= alpha;
                    }
                    block.setFillColor(color);
                    block.setOutlineThickness(1);
                    block.setOutlineColor(sf::Color::White);
                    window.draw(block);
                }
            }
        }
    }
}

void Game::drawNextPiece() {
    if (board.getNextPiece()) {
        auto nextPiece = board.getNextPiece();
        auto nextShape = nextPiece->getShape();
        
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
    sf::RectangleShape overlay(sf::Vector2f(400, 600));
    overlay.setPosition(0, 0);
    overlay.setFillColor(sf::Color(0, 0, 0, 150));
    window.draw(overlay);
    
    sf::RectangleShape gameOverPanel(sf::Vector2f(300, 200));
    gameOverPanel.setPosition(50, 200);
    gameOverPanel.setFillColor(sf::Color(30, 30, 40));
    gameOverPanel.setOutlineThickness(3);
    gameOverPanel.setOutlineColor(sf::Color::Red);
    window.draw(gameOverPanel);
    
    sf::Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setString("GAME OVER");
    gameOverText.setCharacterSize(32);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setStyle(sf::Text::Bold);
    gameOverText.setPosition(90, 230);
    window.draw(gameOverText);
    
    sf::Text finalScoreText;
    finalScoreText.setFont(font);
    finalScoreText.setString("Score: " + std::to_string(board.getScore()));
    finalScoreText.setCharacterSize(20);
    finalScoreText.setFillColor(sf::Color::White);
    finalScoreText.setPosition(130, 290);
    window.draw(finalScoreText);
    
    sf::Text restartText;
    restartText.setFont(font);
    restartText.setString("R: Restart | ESC: Menu");
    restartText.setCharacterSize(16);
    restartText.setFillColor(sf::Color::Yellow);
    restartText.setPosition(100, 340);
    window.draw(restartText);
}
