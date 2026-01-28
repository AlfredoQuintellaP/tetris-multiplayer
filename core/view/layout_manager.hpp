#ifndef LAYOUT_MANAGER_H
#define LAYOUT_MANAGER_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

/**
 * Layout structures defining positions and sizes of UI elements.
 */
struct Layout {
    struct Board {
        sf::Vector2f position;
        float cellSize;
        float width() const { return 10 * cellSize; }   // 10 columns standard
        float height() const { return 20 * cellSize; }  // 20 rows standard
    };
    
    struct Panel {
        sf::Vector2f position;
        sf::Vector2f size;
        std::string title;
    };
    
    struct Menu {
        sf::Vector2f position;
        float optionHeight;
        float optionSpacing;
        float width;
    };
};

/**
 * Manages positioning and layout of all game UI elements.
 * Calculates positions for boards, panels, and menus based on window size and game mode.
 */
class LayoutManager {
public:
    LayoutManager() = default;
    
    /**
     * Updates the reference window size for layout calculations.
     * 
     * @param size Current window dimensions
     */
    void setWindowSize(const sf::Vector2u& size) { windowSize = size; }
    
    // Single player layout structures
    struct SinglePlayerLayout {
        Layout::Board board;
        Layout::Panel nextPiecePanel;
        Layout::Panel holdPiecePanel;
        Layout::Panel scorePanel;
        Layout::Panel levelPanel;
        Layout::Panel linesPanel;
        Layout::Panel gameInfoPanel;
    };
    
    // Multiplayer layout structures
    struct PlayerLayout {
        Layout::Board board;
        Layout::Panel nextPiecePanel;
        Layout::Panel holdPiecePanel;
        Layout::Panel scorePanel;
        Layout::Panel levelPanel;
        Layout::Panel linesPanel;
        Layout::Panel playerInfoPanel;
    };
    
    struct MultiplayerLocalLayout {
        PlayerLayout player1;
        PlayerLayout player2;
    };
    
    struct MultiplayerOnlineLayout {
        PlayerLayout localPlayer;
        PlayerLayout remotePlayer;
        Layout::Panel networkStatusPanel;
    };
    
    // Layout calculation methods
    Layout::Menu calculateMainMenu() const;
    Layout::Menu calculateOnlineMenu() const;
    Layout::Panel calculateIPInputBox() const;
    
    SinglePlayerLayout calculateSinglePlayerLayout() const;
    MultiplayerLocalLayout calculateMultiplayerLocalLayout() const;
    MultiplayerOnlineLayout calculateMultiplayerOnlineLayout() const;
    
    // Utility positioning functions
    static sf::Vector2f centerInWindow(float width, float height, 
                                      const sf::Vector2u& windowSize);
    static sf::Vector2f centerHorizontally(float width, float y,
                                         const sf::Vector2u& windowSize);
    static sf::Vector2f centerVertically(float height, float x,
                                       const sf::Vector2u& windowSize);
    
private:
    sf::Vector2u windowSize;
    
    // Layout constants
    const float BOARD_CELL_SIZE = 24.0f;
    const float PANEL_WIDTH = 140.0f;
    const float PANEL_HEIGHT_SMALL = 50.0f;
    const float PANEL_HEIGHT_MEDIUM = 60.0f;
    const float PANEL_HEIGHT_LARGE = 100.0f;
    const float MARGIN = 20.0f;
    const float SPACING = 15.0f;
    
    /**
     * Creates a consistent player layout for given coordinates.
     * 
     * @param boardX X-coordinate for board
     * @param boardY Y-coordinate for board
     * @param panelsX X-coordinate for panels
     * @param panelsStartY Starting Y-coordinate for panels
     * @return Complete player layout
     */
    PlayerLayout createPlayerLayout(float boardX, float boardY, 
                                   float panelsX, float panelsStartY) const;
};

#endif
