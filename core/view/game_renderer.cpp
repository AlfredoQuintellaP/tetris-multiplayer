#include "game_renderer.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>

GameRenderer::GameRenderer() {
    // Default constructor
}

/**
 * Loads all game resources.
 */
bool GameRenderer::loadResources() {
    return loadFont();
}

/**
 * Attempts to load font from various locations.
 */
bool GameRenderer::loadFont(const std::string& path) {
    std::vector<std::string> fontPaths;
    
    if (!path.empty()) {
        fontPaths.push_back(path);
    }
    
    // Common system font paths
    fontPaths.insert(fontPaths.end(), {
        "/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf",
        "/usr/share/fonts/truetype/liberation/LiberationSans-Bold.ttf",
        "/usr/share/fonts/truetype/freefont/FreeSansBold.ttf",
        "/usr/share/fonts/truetype/ubuntu/Ubuntu-B.ttf",
        "C:/Windows/Fonts/arial.ttf",
        "assets/fonts/arial.ttf",
        "fonts/arial.ttf"
    });
    
    for (const auto& fontPath : fontPaths) {
        if (font.loadFromFile(fontPath)) {
            std::cout << "Font loaded: " << fontPath << std::endl;
            return true;
        }
    }
    
    std::cerr << "ERROR: No font found!" << std::endl;
    return false;
}

/**
 * Main render dispatch method.
 * Renders game based on current state, handling overlays for pause/game over.
 */
void GameRenderer::render(sf::RenderWindow& window, const GameData& gameData) {
    // Use view size for layout calculations
    sf::Vector2f viewSize = window.getView().getSize();
    layoutManager.setWindowSize(sf::Vector2u(viewSize.x, viewSize.y));
    
    window.clear(sf::Color(10, 10, 40)); // Dark blue background
    
    GameState stateToRender = gameData.getCurrentState();
    
    // Handle overlay states (pause/game over)
    if (stateToRender == GameState::PAUSED || stateToRender == GameState::GAME_OVER) {
        GameState previousState = gameData.getPreviousState();
        
        // Render the underlying game state first
        switch (previousState) {
            case GameState::SINGLE_PLAYER:
                renderSinglePlayer(window, gameData);
                break;
            case GameState::MULTIPLAYER_LOCAL:
                renderMultiplayerLocal(window, gameData);
                break;
            case GameState::MULTIPLAYER_ONLINE_GAME:
                renderMultiplayerOnline(window, gameData);
                break;
            default:
                break;
        }
    } else {
        // Render normal game state
        switch (stateToRender) {
            case GameState::MAIN_MENU:
                renderMainMenu(window, gameData);
                break;
            case GameState::SINGLE_PLAYER:
                renderSinglePlayer(window, gameData);
                break;
            case GameState::MULTIPLAYER_LOCAL:
                renderMultiplayerLocal(window, gameData);
                break;
            case GameState::MULTIPLAYER_ONLINE_LOBBY:
                renderOnlineLobby(window, gameData);
                break;
            case GameState::MULTIPLAYER_ONLINE_GAME:
                renderMultiplayerOnline(window, gameData);
                break;
            default:
                break;
        }
    }
    
    // Render overlays on top if needed
    if (stateToRender == GameState::PAUSED) {
        renderPauseScreen(window, gameData);
    } else if (stateToRender == GameState::GAME_OVER) {
        renderGameOver(window, gameData);
    }
}

// ======================
// MAIN MENU RENDERING
// ======================

void GameRenderer::renderMainMenu(sf::RenderWindow& window, const GameData& gameData) {
    // Title
    renderCenteredText(window, "TETRIS", 100, 80, sf::Color::Cyan);
    
    // Menu options
    auto menuLayout = layoutManager.calculateMainMenu();
    renderMenu(window, menuLayout, MAIN_MENU_OPTIONS, 
               static_cast<int>(gameData.getSelectedMenuOption()));
    
    // Instructions and credits
    renderCenteredText(window, "Use ARROWS to navigate, ENTER to select", 
                       window.getSize().y - 80, 16, sf::Color(150, 150, 150));
    renderCenteredText(window, "Made by Helena GUACHALLA and Alfredo QUINTELLA", 
                       window.getSize().y - 50, 10, sf::Color(150, 150, 150));
}

// ======================
// SINGLE PLAYER RENDERING
// ======================

void GameRenderer::renderSinglePlayer(sf::RenderWindow& window, const GameData& gameData) {
    auto layout = layoutManager.calculateSinglePlayerLayout();
    const auto& board = gameData.getPlayerBoard(0);
    const auto& stats = gameData.getPlayerStats(0);
    
    // Render game board
    boardRenderer.render(window, board, layout.board.position.x, 
                        layout.board.position.y, layout.board.cellSize);
    
    // Render UI panels
    renderPanel(window, layout.nextPiecePanel, "NEXT");
    renderNextPiece(window, board, layout.nextPiecePanel);
    
    renderPanel(window, layout.holdPiecePanel, "HOLD");
    renderHoldPiece(window, board, layout.holdPiecePanel);
    
    renderPanel(window, layout.scorePanel, "SCORE");
    renderTextInPanel(window, layout.scorePanel, formatNumber(stats.score), 
                     24, sf::Color::White, true);
    
    renderPanel(window, layout.levelPanel, "LEVEL");
    renderTextInPanel(window, layout.levelPanel, std::to_string(stats.level), 
                     24, sf::Color::Cyan, true);
    
    renderPanel(window, layout.linesPanel, "LINES");
    renderTextInPanel(window, layout.linesPanel, std::to_string(stats.linesCleared), 
                     24, sf::Color::Green, true);
}

// ======================
// MULTIPLAYER LOCAL RENDERING
// ======================

void GameRenderer::renderPlayer(sf::RenderWindow& window, const ClassicBoard& board,
                               const GameData::PlayerStats& stats,
                               const LayoutManager::PlayerLayout& playerLayout,
                               const std::string& playerName,
                               bool isLocalPlayer) {
    
    // Render board
    boardRenderer.render(window, board, 
                        playerLayout.board.position.x, 
                        playerLayout.board.position.y, 
                        playerLayout.board.cellSize);
    
    // Render UI panels
    renderPanel(window, playerLayout.nextPiecePanel, "NEXT");
    renderNextPiece(window, board, playerLayout.nextPiecePanel);
    
    renderPanel(window, playerLayout.holdPiecePanel, "HOLD");
    renderHoldPiece(window, board, playerLayout.holdPiecePanel);
    
    renderPanel(window, playerLayout.scorePanel, "SCORE");
    renderTextInPanel(window, playerLayout.scorePanel, formatNumber(stats.score), 
                     24, sf::Color::White, true);
    
    renderPanel(window, playerLayout.levelPanel, "LEVEL");
    renderTextInPanel(window, playerLayout.levelPanel, std::to_string(stats.level), 
                     24, sf::Color::Cyan, true);
    
    renderPanel(window, playerLayout.linesPanel, "LINES");
    renderTextInPanel(window, playerLayout.linesPanel, std::to_string(stats.linesCleared), 
                     24, sf::Color::Green, true);
}

void GameRenderer::renderMultiplayerLocal(sf::RenderWindow& window, const GameData& gameData) {
    auto layout = layoutManager.calculateMultiplayerLocalLayout();
    
    // Render both players
    renderPlayer(window, gameData.getPlayerBoard(0), 
                 gameData.getPlayerStats(0), layout.player1, "PLAYER 1");
    
    renderPlayer(window, gameData.getPlayerBoard(1), 
                 gameData.getPlayerStats(1), layout.player2, "PLAYER 2");
    
    // Show winner if game over
    if (gameData.isGameOver()) {
        int score1 = gameData.getPlayerStats(0).score;
        int score2 = gameData.getPlayerStats(1).score;
        
        std::string winner;
        if (score1 > score2) {
            winner = "PLAYER 1 WINS!";
        } else if (score2 > score1) {
            winner = "PLAYER 2 WINS!";
        } else {
            winner = "DRAW!";
        }
        
        renderMessageBox(window, "GAME OVER", winner, "Press R to restart");
    }
}

// ======================
// ONLINE LOBBY RENDERING
// ======================

void GameRenderer::renderOnlineLobby(sf::RenderWindow& window, const GameData& gameData) {
    // Title
    renderCenteredText(window, "ONLINE MULTIPLAYER", 80, 50, sf::Color::Cyan);
    
    if (gameData.isInputtingIP()) {
        // IP input mode
        renderCenteredText(window, "Enter Server IP Address:", 200, 24, sf::Color::White);
        
        auto ipBox = layoutManager.calculateIPInputBox();
        renderIPInputBox(window, ipBox, gameData.getInputIP(), true);
        
        renderCenteredText(window, "Press ENTER to connect, ESC to cancel", 
                           window.getSize().y - 100, 16, sf::Color(150, 150, 150));
    } else if (gameData.isHosting() && !gameData.isConnected()) {
        // Host waiting for connection
        renderCenteredText(window, "Waiting for opponent...", 300, 30, sf::Color::Yellow);
        renderCenteredText(window, "Have your opponent connect to your IP", 
                           350, 18, sf::Color(150, 150, 150));
    } else {
        // Normal online menu
        auto menuLayout = layoutManager.calculateOnlineMenu();
        renderOnlineMenu(window, menuLayout, ONLINE_MENU_OPTIONS,
                        static_cast<int>(gameData.getSelectedOnlineOption()));
    }
}

// ======================
// MULTIPLAYER ONLINE RENDERING
// ======================

void GameRenderer::renderMultiplayerOnline(sf::RenderWindow& window, const GameData& gameData) {
    auto layout = layoutManager.calculateMultiplayerOnlineLayout();
    
    // Local player
    renderPlayer(window, gameData.getPlayerBoard(0), 
                 gameData.getPlayerStats(0), layout.localPlayer, "YOU (P1)", true);
    
    // Remote opponent
    renderPlayer(window, gameData.getPlayerBoard(1), 
                 gameData.getPlayerStats(1), layout.remotePlayer, "OPPONENT (P2)", false);
    
    // Network status panel
    renderPanel(window, layout.networkStatusPanel);
    
    std::string netStatus;
    sf::Color netColor;
    
    if (gameData.isConnected()) {
        if (gameData.isHosting()) {
            netStatus = "HOSTING";
            netColor = sf::Color::Green;
        } else {
            netStatus = "CONNECTED";
            netColor = sf::Color::Cyan;
        }
    } else {
        netStatus = "DISCONNECTED";
        netColor = sf::Color::Red;
    }
    
    renderTextInPanel(window, layout.networkStatusPanel, netStatus, 20, netColor, true);
    
    // Game over message
    if (gameData.isGameOver()) {
        int localScore = gameData.getPlayerStats(0).score;
        int remoteScore = gameData.getPlayerStats(1).score;
        
        std::string winner;
        if (localScore > remoteScore) {
            winner = "YOU WIN!";
        } else if (remoteScore > localScore) {
            winner = "OPPONENT WINS!";
        } else {
            winner = "DRAW!";
        }
        
        renderMessageBox(window, "GAME OVER", winner, "Press ESC for main menu");
    }
}

// ======================
// PAUSE SCREEN RENDERING
// ======================

void GameRenderer::renderPauseScreen(sf::RenderWindow& window, const GameData& gameData) {
    sf::Vector2f viewCenter = window.getView().getCenter();
    
    renderOverlay(window, sf::Color(0, 0, 0, 180));
    
    // Pause dialog box
    float width = 400.0f;
    float height = 200.0f;
    float x = viewCenter.x - width / 2.0f;
    float y = viewCenter.y - height / 2.0f;
    
    Layout::Panel panel;
    panel.position = sf::Vector2f(x, y);
    panel.size = sf::Vector2f(width, height);
    
    renderPanel(window, panel, "PAUSED");
    
    // Instructions
    renderText(window, "Press P to resume", 
               viewCenter.x, y + 100, 24, sf::Color::White, true);
    renderText(window, "Press ESC for main menu", 
               viewCenter.x, y + 130, 20, sf::Color(180, 180, 180), true);
}

// ======================
// GAME OVER RENDERING
// ======================

void GameRenderer::renderGameOver(sf::RenderWindow& window, const GameData& gameData) {
    sf::Vector2f viewCenter = window.getView().getCenter();
    
    renderOverlay(window, sf::Color(0, 0, 0, 200));
    
    // Game over dialog box
    float width = 400.0f;
    float height = 250.0f;
    float x = viewCenter.x - width / 2.0f;
    float y = viewCenter.y - height / 2.0f;
    
    Layout::Panel panel;
    panel.position = sf::Vector2f(x, y);
    panel.size = sf::Vector2f(width, height);
    
    renderPanel(window, panel, "GAME OVER");
    
    // Score display
    std::string scoreText = "Score: " + formatNumber(gameData.getPlayerStats(0).score);
    renderText(window, scoreText, 
               viewCenter.x, y + 100, 28, sf::Color::White, true);
    
    // Instructions
    renderText(window, "Press R to restart", 
               viewCenter.x, y + 150, 22, sf::Color::Yellow, true);
    renderText(window, "Press ESC for main menu", 
               viewCenter.x, y + 180, 18, sf::Color(180, 180, 180), true);
}

// ======================
// UI COMPONENT RENDERING
// ======================

void GameRenderer::renderMenu(sf::RenderWindow& window, const Layout::Menu& layout,
                             const std::array<std::string, 4>& options, int selectedIndex) {
    float currentY = layout.position.y;
    
    for (size_t i = 0; i < options.size(); i++) {
        std::string text = options[i];
        sf::Color color = (static_cast<int>(i) == selectedIndex) ? sf::Color::Yellow : sf::Color::White;
        
        if (static_cast<int>(i) == selectedIndex) {
            text = "> " + text + " <";
        }
        
        renderText(window, text, layout.position.x, currentY, 30, color);
        currentY += layout.optionHeight + layout.optionSpacing;
    }
}

void GameRenderer::renderOnlineMenu(sf::RenderWindow& window, const Layout::Menu& layout,
                                   const std::array<std::string, 3>& options, int selectedIndex) {
    float currentY = layout.position.y;
    
    for (size_t i = 0; i < options.size(); i++) {
        std::string text = options[i];
        sf::Color color = (static_cast<int>(i) == selectedIndex) ? sf::Color::Yellow : sf::Color::White;
        
        if (static_cast<int>(i) == selectedIndex) {
            text = "> " + text + " <";
        }
        
        renderText(window, text, layout.position.x, currentY, 30, color);
        currentY += layout.optionHeight + layout.optionSpacing;
    }
}

void GameRenderer::renderIPInputBox(sf::RenderWindow& window, const Layout::Panel& panel,
                                   const std::string& ip, bool isActive) {
    renderPanel(window, panel);
    
    // Add blinking cursor if active
    std::string displayText = ip;
    if (isActive && static_cast<int>(std::clock() / (CLOCKS_PER_SEC / 2)) % 2 == 0) {
        displayText += "_";
    }
    
    renderTextInPanel(window, panel, displayText, 24, 
                     isActive ? sf::Color::Yellow : sf::Color::White, true);
}

void GameRenderer::renderPanel(sf::RenderWindow& window, const Layout::Panel& panel,
                              const std::string& title) {
    sf::RectangleShape rect(panel.size);
    rect.setPosition(panel.position);
    rect.setFillColor(sf::Color(40, 40, 50, 200));
    rect.setOutlineThickness(2);
    rect.setOutlineColor(sf::Color(80, 80, 100));
    window.draw(rect);
    
    if (!title.empty()) {
        renderText(window, title, panel.position.x + 10, panel.position.y + 8, 
                   14, sf::Color(200, 200, 200));
    }
}

void GameRenderer::renderTextInPanel(sf::RenderWindow& window, const Layout::Panel& panel,
                                    const std::string& text, unsigned int size,
                                    sf::Color color, bool centered) {
    if (centered) {
        float centerX = panel.position.x + panel.size.x / 2.0f;
        float centerY = panel.position.y + panel.size.y / 2.0f;
        renderText(window, text, centerX, centerY, size, color, true);
    } else {
        float x = panel.position.x + 10;
        float y = panel.position.y + (panel.size.y - size) / 2.0f;
        renderText(window, text, x, y, size, color, false);
    }
}

// ======================
// GAME ELEMENT RENDERING
// ======================

void GameRenderer::renderNextPiece(sf::RenderWindow& window, const ClassicBoard& board,
                                  const Layout::Panel& panel) {
    if (!board.getNextPiece()) return;
    
    auto piece = board.getNextPiece();
    float cellSize = 20.0f;
    float pieceWidth = piece->getWidth() * cellSize;
    float pieceHeight = piece->getHeight() * cellSize;
    
    // Center in panel
    float startX = panel.position.x + (panel.size.x - pieceWidth) / 2.0f;
    float startY = panel.position.y + 30 + (panel.size.y - 30 - pieceHeight) / 2.0f;
    
    boardRenderer.renderPiece(window, *piece, startX, startY, cellSize);
}

void GameRenderer::renderHoldPiece(sf::RenderWindow& window, const ClassicBoard& board,
                                  const Layout::Panel& panel) {
    if (!board.getHoldPiece()) return;
    
    auto piece = board.getHoldPiece();
    float cellSize = 20.0f;
    float pieceWidth = piece->getWidth() * cellSize;
    float pieceHeight = piece->getHeight() * cellSize;
    
    // Center in panel
    float startX = panel.position.x + (panel.size.x - pieceWidth) / 2.0f;
    float startY = panel.position.y + 30 + (panel.size.y - 30 - pieceHeight) / 2.0f;
    
    // Apply transparency if hold not available
    float alpha = board.getCanHold() ? 1.0f : 0.5f;
    boardRenderer.renderPiece(window, *piece, startX, startY, cellSize, alpha);
}

// ======================
// UTILITY METHODS
// ======================

void GameRenderer::renderText(sf::RenderWindow& window, const std::string& text,
                             float x, float y, unsigned int size,
                             sf::Color color, bool centered) {
    sf::Text sfText(text, font, size);
    sfText.setFillColor(color);
    
    if (centered) {
        sf::FloatRect bounds = sfText.getLocalBounds();
        sfText.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
        sfText.setPosition(x, y);
    } else {
        sfText.setPosition(x, y);
    }
    
    window.draw(sfText);
}

void GameRenderer::renderCenteredText(sf::RenderWindow& window, const std::string& text,
                                     float y, unsigned int size, sf::Color color) {
    renderText(window, text, window.getSize().x / 2.0f, y, size, color, true);
}

std::string GameRenderer::formatNumber(int number, int width) {
    std::stringstream ss;
    ss << std::setw(width) << std::setfill('0') << number;
    return ss.str();
}

std::string GameRenderer::formatTime(float seconds) {
    int minutes = static_cast<int>(seconds) / 60;
    int secs = static_cast<int>(seconds) % 60;
    std::stringstream ss;
    ss << std::setw(2) << std::setfill('0') << minutes << ":"
       << std::setw(2) << std::setfill('0') << secs;
    return ss.str();
}

// ======================
// OVERLAY METHODS
// ======================

void GameRenderer::renderOverlay(sf::RenderWindow& window, sf::Color color) {
    sf::View currentView = window.getView();
    sf::Vector2f viewSize = currentView.getSize();
    sf::Vector2f viewCenter = currentView.getCenter();
    
    float x = viewCenter.x - viewSize.x / 2.0f;
    float y = viewCenter.y - viewSize.y / 2.0f;
    
    sf::RectangleShape overlay(viewSize);
    overlay.setPosition(x, y);
    overlay.setFillColor(color);
    window.draw(overlay);
}

void GameRenderer::renderMessageBox(sf::RenderWindow& window, const std::string& title,
                                   const std::string& message, const std::string& instructions) {
    sf::Vector2f viewCenter = window.getView().getCenter();
    
    float width = 400.0f;
    float height = 200.0f;
    float x = viewCenter.x - width / 2.0f;
    float y = viewCenter.y - height / 2.0f;
    
    Layout::Panel panel;
    panel.position = sf::Vector2f(x, y);
    panel.size = sf::Vector2f(width, height);
    
    renderPanel(window, panel, title);
    
    renderText(window, message, viewCenter.x, y + 80, 24, sf::Color::White, true);
    
    if (!instructions.empty()) {
        renderText(window, instructions, viewCenter.x, y + 140, 16, sf::Color(150, 150, 150), true);
    }
}
