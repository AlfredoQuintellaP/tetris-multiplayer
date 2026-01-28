#include "layout_manager.hpp"

// ======================
// UTILITY FUNCTIONS
// ======================

/**
 * Centers a rectangle within the window.
 */
sf::Vector2f LayoutManager::centerInWindow(float width, float height, 
                                          const sf::Vector2u& windowSize) {
    return sf::Vector2f(
        (windowSize.x - width) / 2.0f,
        (windowSize.y - height) / 2.0f
    );
}

/**
 * Centers horizontally while keeping vertical position.
 */
sf::Vector2f LayoutManager::centerHorizontally(float width, float y,
                                             const sf::Vector2u& windowSize) {
    return sf::Vector2f((windowSize.x - width) / 2.0f, y);
}

/**
 * Centers vertically while keeping horizontal position.
 */
sf::Vector2f LayoutManager::centerVertically(float height, float x,
                                           const sf::Vector2u& windowSize) {
    return sf::Vector2f(x, (windowSize.y - height) / 2.0f);
}

// ======================
// MENU LAYOUTS
// ======================

/**
 * Calculates layout for main menu (4 options).
 */
Layout::Menu LayoutManager::calculateMainMenu() const {
    Layout::Menu menu;
    menu.width = 300.0f;
    menu.optionHeight = 50.0f;
    menu.optionSpacing = 15.0f;
    
    // Center menu with 4 options
    float menuHeight = 4 * (menu.optionHeight + menu.optionSpacing);
    menu.position = centerInWindow(menu.width, menuHeight, windowSize);
    
    return menu;
}

/**
 * Calculates layout for online menu (3 options).
 */
Layout::Menu LayoutManager::calculateOnlineMenu() const {
    Layout::Menu menu;
    menu.width = 300.0f;
    menu.optionHeight = 50.0f;
    menu.optionSpacing = 15.0f;
    
    // Center menu with 3 options
    float menuHeight = 3 * (menu.optionHeight + menu.optionSpacing);
    menu.position = centerInWindow(menu.width, menuHeight, windowSize);
    menu.position.y += 50.0f; // Offset for title space
    
    return menu;
}

/**
 * Calculates layout for IP address input box.
 */
Layout::Panel LayoutManager::calculateIPInputBox() const {
    Layout::Panel panel;
    panel.size = sf::Vector2f(400.0f, 50.0f);
    panel.position = centerInWindow(panel.size.x, panel.size.y, windowSize);
    panel.position.y += 100.0f; // Offset below title
    
    return panel;
}

// ======================
// SINGLE PLAYER LAYOUT
// ======================

/**
 * Calculates layout for single player mode.
 * Designed for 600x600 view area.
 */
LayoutManager::SinglePlayerLayout LayoutManager::calculateSinglePlayerLayout() const {
    SinglePlayerLayout layout;
    
    // Board dimensions
    const float BOARD_CELL_SIZE = 26.0f;
    const float BOARD_WIDTH = 10 * BOARD_CELL_SIZE;    // 260 pixels
    const float BOARD_HEIGHT = 20 * BOARD_CELL_SIZE;   // 520 pixels
    const float PANEL_WIDTH = 140.0f;
    const float SPACING = 12.0f;
    
    // Total width needed (board + panels + spacing)
    float totalWidth = BOARD_WIDTH + PANEL_WIDTH + SPACING; // 412 pixels
    
    // Center within 600x600 area
    float startX = (600.0f - totalWidth) / 2.0f;  // 94 pixels from left
    float startY = (600.0f - BOARD_HEIGHT) / 2.0f; // 40 pixels from top
    
    // Board position
    layout.board.position = sf::Vector2f(startX, startY);
    layout.board.cellSize = BOARD_CELL_SIZE;
    
    // Panels positioned to the right of the board
    float panelsX = startX + BOARD_WIDTH + SPACING;
    
    // Calculate total height of all panels to center them vertically
    float totalPanelsHeight = 90 * 2 + 55 + 45 * 2 + 12 * 4; // 463 pixels
    float panelsStartY = startY + (BOARD_HEIGHT - totalPanelsHeight) / 2.0f;
    
    // Position each panel
    layout.nextPiecePanel.position = sf::Vector2f(panelsX, panelsStartY);
    layout.nextPiecePanel.size = sf::Vector2f(PANEL_WIDTH, 90);
    
    layout.holdPiecePanel.position = sf::Vector2f(panelsX, panelsStartY + 90 + 12);
    layout.holdPiecePanel.size = sf::Vector2f(PANEL_WIDTH, 90);
    
    layout.scorePanel.position = sf::Vector2f(panelsX, panelsStartY + (90 + 12) * 2);
    layout.scorePanel.size = sf::Vector2f(PANEL_WIDTH, 55);
    
    layout.levelPanel.position = sf::Vector2f(panelsX, panelsStartY + (90 + 12) * 2 + 55 + 12);
    layout.levelPanel.size = sf::Vector2f(PANEL_WIDTH, 45);
    
    layout.linesPanel.position = sf::Vector2f(panelsX, panelsStartY + (90 + 12) * 2 + 55 + 45 + 12 * 2);
    layout.linesPanel.size = sf::Vector2f(PANEL_WIDTH, 45);
    
    // Game info panel (currently unused)
    layout.gameInfoPanel.size = sf::Vector2f(0, 0);
    
    return layout;
}

// ======================
// MULTIPLAYER LOCAL LAYOUT
// ======================

/**
 * Calculates layout for local multiplayer (two players side by side).
 * Designed for 1100x600 window.
 */
LayoutManager::MultiplayerLocalLayout LayoutManager::calculateMultiplayerLocalLayout() const {
    MultiplayerLocalLayout layout;
    
    // Board dimensions (same for both players)
    const float BOARD_CELL_SIZE = 26.0f;
    const float BOARD_WIDTH = 10 * BOARD_CELL_SIZE;    // 260 pixels
    const float BOARD_HEIGHT = 20 * BOARD_CELL_SIZE;   // 520 pixels
    const float PANEL_WIDTH = 140.0f;
    const float MARGIN = 30.0f;
    const float SPACING = 12.0f;
    
    // Width per player (board + panels)
    float playerWidth = BOARD_WIDTH + PANEL_WIDTH + SPACING; // 412 pixels
    
    // Total width for both players with margin between them
    float totalWidth = 2 * playerWidth + MARGIN; // 854 pixels
    
    // Center within 1100x600 window
    float startX = (1100.0f - totalWidth) / 2.0f;  // 123 pixels from left
    float startY = (600.0f - BOARD_HEIGHT) / 2.0f; // 40 pixels from top
    
    // Player 1 (left side)
    float player1BoardX = startX;
    float player1PanelsX = player1BoardX + BOARD_WIDTH + SPACING;
    
    layout.player1.board.position = sf::Vector2f(player1BoardX, startY);
    layout.player1.board.cellSize = BOARD_CELL_SIZE;
    
    // Player 2 (right side)
    float player2BoardX = startX + playerWidth + MARGIN;
    float player2PanelsX = player2BoardX + BOARD_WIDTH + SPACING;
    
    layout.player2.board.position = sf::Vector2f(player2BoardX, startY);
    layout.player2.board.cellSize = BOARD_CELL_SIZE;
    
    // Calculate panel positions (same vertical arrangement for both players)
    float totalPanelsHeight = 90 * 2 + 55 + 45 * 2 + 12 * 4; // 463 pixels
    float panelsStartY = startY + (BOARD_HEIGHT - totalPanelsHeight) / 2.0f;
    
    // Configure panels for player 1
    layout.player1.nextPiecePanel.position = sf::Vector2f(player1PanelsX, panelsStartY);
    layout.player1.nextPiecePanel.size = sf::Vector2f(PANEL_WIDTH, 90);
    
    layout.player1.holdPiecePanel.position = sf::Vector2f(player1PanelsX, panelsStartY + 90 + 12);
    layout.player1.holdPiecePanel.size = sf::Vector2f(PANEL_WIDTH, 90);
    
    layout.player1.scorePanel.position = sf::Vector2f(player1PanelsX, panelsStartY + (90 + 12) * 2);
    layout.player1.scorePanel.size = sf::Vector2f(PANEL_WIDTH, 55);
    
    layout.player1.levelPanel.position = sf::Vector2f(player1PanelsX, panelsStartY + (90 + 12) * 2 + 55 + 12);
    layout.player1.levelPanel.size = sf::Vector2f(PANEL_WIDTH, 45);
    
    layout.player1.linesPanel.position = sf::Vector2f(player1PanelsX, panelsStartY + (90 + 12) * 2 + 55 + 45 + 12 * 2);
    layout.player1.linesPanel.size = sf::Vector2f(PANEL_WIDTH, 45);
    
    // Configure panels for player 2 (mirrored positions)
    layout.player2.nextPiecePanel.position = sf::Vector2f(player2PanelsX, panelsStartY);
    layout.player2.nextPiecePanel.size = sf::Vector2f(PANEL_WIDTH, 90);
    
    layout.player2.holdPiecePanel.position = sf::Vector2f(player2PanelsX, panelsStartY + 90 + 12);
    layout.player2.holdPiecePanel.size = sf::Vector2f(PANEL_WIDTH, 90);
    
    layout.player2.scorePanel.position = sf::Vector2f(player2PanelsX, panelsStartY + (90 + 12) * 2);
    layout.player2.scorePanel.size = sf::Vector2f(PANEL_WIDTH, 55);
    
    layout.player2.levelPanel.position = sf::Vector2f(player2PanelsX, panelsStartY + (90 + 12) * 2 + 55 + 12);
    layout.player2.levelPanel.size = sf::Vector2f(PANEL_WIDTH, 45);
    
    layout.player2.linesPanel.position = sf::Vector2f(player2PanelsX, panelsStartY + (90 + 12) * 2 + 55 + 45 + 12 * 2);
    layout.player2.linesPanel.size = sf::Vector2f(PANEL_WIDTH, 45);
    
    return layout;
}

// ======================
// MULTIPLAYER ONLINE LAYOUT
// ======================

/**
 * Creates a standardized player layout at specified coordinates.
 */
LayoutManager::PlayerLayout LayoutManager::createPlayerLayout(float boardX, float boardY, 
                                                             float panelsX, float panelsStartY) const {
    PlayerLayout layout;
    
    // Board configuration
    layout.board.position = sf::Vector2f(boardX, boardY);
    layout.board.cellSize = BOARD_CELL_SIZE;
    
    // Panel positions (stacked vertically)
    layout.nextPiecePanel.position = sf::Vector2f(panelsX, panelsStartY);
    layout.nextPiecePanel.size = sf::Vector2f(PANEL_WIDTH, PANEL_HEIGHT_LARGE);
    
    layout.holdPiecePanel.position = sf::Vector2f(panelsX, panelsStartY + PANEL_HEIGHT_LARGE + SPACING);
    layout.holdPiecePanel.size = sf::Vector2f(PANEL_WIDTH, PANEL_HEIGHT_LARGE);
    
    layout.scorePanel.position = sf::Vector2f(panelsX, panelsStartY + (PANEL_HEIGHT_LARGE + SPACING) * 2);
    layout.scorePanel.size = sf::Vector2f(PANEL_WIDTH, PANEL_HEIGHT_MEDIUM);
    
    layout.levelPanel.position = sf::Vector2f(panelsX, panelsStartY + (PANEL_HEIGHT_LARGE + SPACING) * 2 + PANEL_HEIGHT_MEDIUM + SPACING);
    layout.levelPanel.size = sf::Vector2f(PANEL_WIDTH, PANEL_HEIGHT_SMALL);
    
    layout.linesPanel.position = sf::Vector2f(panelsX, panelsStartY + (PANEL_HEIGHT_LARGE + SPACING) * 2 + PANEL_HEIGHT_MEDIUM + PANEL_HEIGHT_SMALL + SPACING * 2);
    layout.linesPanel.size = sf::Vector2f(PANEL_WIDTH, PANEL_HEIGHT_SMALL);
    
    // Player info panel above the board
    layout.playerInfoPanel.position = sf::Vector2f(boardX, boardY - 40);
    layout.playerInfoPanel.size = sf::Vector2f(layout.board.width(), 35);
    
    return layout;
}

/**
 * Calculates layout for online multiplayer.
 * Uses local multiplayer layout and adds network status panel.
 */
LayoutManager::MultiplayerOnlineLayout LayoutManager::calculateMultiplayerOnlineLayout() const {
    MultiplayerOnlineLayout layout;
    
    // Reuse local multiplayer layout for board positioning
    MultiplayerLocalLayout localLayout = calculateMultiplayerLocalLayout();
    
    layout.localPlayer = localLayout.player1;
    layout.remotePlayer = localLayout.player2;
    
    // Add network status panel at top center
    layout.networkStatusPanel.position = sf::Vector2f(
        (1100.0f - 200.0f) / 2.0f,  // Center in 1100px width
        20.0f                       // Top margin
    );
    layout.networkStatusPanel.size = sf::Vector2f(200.0f, 40.0f);
    
    return layout;
}
