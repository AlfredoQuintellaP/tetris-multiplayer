#ifndef GAME_RENDERER_H
#define GAME_RENDERER_H

#include <SFML/Graphics.hpp>
#include <string>
#include <array>
#include <memory>
#include "../model/game_data.hpp"
#include "board_renderer.hpp"
#include "layout_manager.hpp"

/**
 * Main rendering system for the Tetris game.
 * Handles all visual output including game boards, menus, and UI elements.
 */
class GameRenderer {
public:
    GameRenderer();
    
    /**
     * Loads all required resources (fonts, textures, etc.).
     * 
     * @return True if resources loaded successfully
     */
    bool loadResources();
    
    /**
     * Main render method that draws the entire game based on current state.
     * 
     * @param window SFML render window
     * @param gameData Current game state and data
     */
    void render(sf::RenderWindow& window, const GameData& gameData);
    
private:
    // Resource management
    sf::Font font;
    BoardRenderer boardRenderer;
    LayoutManager layoutManager;
    
    // Menu text options
    const std::array<std::string, 4> MAIN_MENU_OPTIONS = {
        "Single Player",
        "Multiplayer Local",
        "Multiplayer Online",
        "Exit"
    };
    
    const std::array<std::string, 3> ONLINE_MENU_OPTIONS = {
        "Host Game",
        "Join Game",
        "Back"
    };
    
    /**
     * Attempts to load font from common system locations.
     * 
     * @param path Optional custom font path
     * @return True if font loaded successfully
     */
    bool loadFont(const std::string& path = "");
    
    // State-specific render methods
    void renderMainMenu(sf::RenderWindow& window, const GameData& gameData);
    void renderSinglePlayer(sf::RenderWindow& window, const GameData& gameData);
    void renderMultiplayerLocal(sf::RenderWindow& window, const GameData& gameData);
    void renderOnlineLobby(sf::RenderWindow& window, const GameData& gameData);
    void renderMultiplayerOnline(sf::RenderWindow& window, const GameData& gameData);
    void renderPauseScreen(sf::RenderWindow& window, const GameData& gameData);
    void renderGameOver(sf::RenderWindow& window, const GameData& gameData);
    
    /**
     * Renders a complete player setup including board and UI panels.
     * 
     * @param window SFML render window
     * @param board Player's game board
     * @param stats Player's statistics
     * @param playerLayout Layout configuration for this player
     * @param playerName Display name for the player
     * @param isLocalPlayer True for local player, false for remote opponent
     */
    void renderPlayer(sf::RenderWindow& window, const ClassicBoard& board,
                     const GameData::PlayerStats& stats,
                     const LayoutManager::PlayerLayout& playerLayout,
                     const std::string& playerName,
                     bool isLocalPlayer = true);
    
    // UI component render methods
    void renderMenu(sf::RenderWindow& window, const Layout::Menu& layout,
                   const std::array<std::string, 4>& options, int selectedIndex);
    void renderOnlineMenu(sf::RenderWindow& window, const Layout::Menu& layout,
                         const std::array<std::string, 3>& options, int selectedIndex);
    void renderIPInputBox(sf::RenderWindow& window, const Layout::Panel& panel,
                         const std::string& ip, bool isActive);
    void renderPanel(sf::RenderWindow& window, const Layout::Panel& panel,
                    const std::string& title = "");
    
    // Game element render methods
    void renderNextPiece(sf::RenderWindow& window, const ClassicBoard& board,
                        const Layout::Panel& panel);
    void renderHoldPiece(sf::RenderWindow& window, const ClassicBoard& board,
                        const Layout::Panel& panel);
    
    /**
     * Renders text inside a panel with optional centering.
     */
    void renderTextInPanel(sf::RenderWindow& window, const Layout::Panel& panel,
                          const std::string& text, unsigned int size = 20,
                          sf::Color color = sf::Color::White, bool centered = false);
    
    // Utility render methods
    void renderText(sf::RenderWindow& window, const std::string& text,
                   float x, float y, unsigned int size,
                   sf::Color color = sf::Color::White,
                   bool centered = false);
    
    void renderCenteredText(sf::RenderWindow& window, const std::string& text,
                           float y, unsigned int size,
                           sf::Color color = sf::Color::White);
    
    // Overlay methods
    void renderOverlay(sf::RenderWindow& window, sf::Color color = sf::Color(0, 0, 0, 180));
    void renderMessageBox(sf::RenderWindow& window, const std::string& title,
                         const std::string& message, const std::string& instructions = "");
    
    // Formatting utilities
    std::string formatNumber(int number, int width = 6);
    std::string formatTime(float seconds);
};

#endif
