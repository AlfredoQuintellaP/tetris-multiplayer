#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

#include <SFML/Network.hpp>
#include <string>
#include <memory>
#include "../entities/tetromino.hpp"

enum class NetworkMessageType {
    CONNECT,
    DISCONNECT,
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_DOWN,
    ROTATE,
    HARD_DROP,
    HOLD,
    BOARD_STATE,
    GAME_OVER,
    PING
};

struct NetworkMessage {
    NetworkMessageType type;
    int playerID;
    int data1;  // Can be used for score, lines, etc.
    int data2;  // Can be used for level, position, etc.
};

class NetworkManager {
private:
    sf::TcpSocket socket;
    sf::TcpListener listener;
    bool isHost;
    bool isConnected;
    std::string serverIP;
    unsigned short port;
    
public:
    NetworkManager();
    ~NetworkManager();
    
    // Host functions
    bool startServer(unsigned short serverPort = 54000);
    bool waitForClient();
    
    // Client functions
    bool connectToServer(const std::string& ip, unsigned short serverPort = 54000);
    
    // Common functions
    void disconnect();
    bool sendMessage(const NetworkMessage& message);
    bool receiveMessage(NetworkMessage& message);
    
    bool getIsHost() const { return isHost; }
    bool getIsConnected() const { return isConnected; }
};

#endif
