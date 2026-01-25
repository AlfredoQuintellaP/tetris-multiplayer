#include "network_manager.hpp"
#include <iostream>

NetworkManager::NetworkManager() 
    : isHost(false), isConnected(false), port(54000) {
    socket.setBlocking(false);
}

NetworkManager::~NetworkManager() {
    disconnect();
}

bool NetworkManager::startServer(unsigned short serverPort) {
    port = serverPort;
    
    if (listener.listen(port) != sf::Socket::Done) {
        std::cerr << "Error: Could not bind to port " << port << std::endl;
        return false;
    }
    
    listener.setBlocking(false);
    isHost = true;
    
    std::cout << "Server started on port " << port << std::endl;
    std::cout << "Waiting for client..." << std::endl;
    
    return true;
}

bool NetworkManager::waitForClient() {
    if (!isHost) return false;
    
    if (listener.accept(socket) == sf::Socket::Done) {
        std::cout << "Client connected: " << socket.getRemoteAddress() << std::endl;
        socket.setBlocking(false);
        isConnected = true;
        return true;
    }
    
    return false;
}

bool NetworkManager::connectToServer(const std::string& ip, unsigned short serverPort) {
    serverIP = ip;
    port = serverPort;
    
    std::cout << "Attempting to connect to " << ip << ":" << port << std::endl;
    
    socket.setBlocking(true);  // Blocking for initial connection
    sf::Socket::Status status = socket.connect(ip, port, sf::seconds(5));
    socket.setBlocking(false);
    
    if (status != sf::Socket::Done) {
        std::cerr << "Error: Could not connect to server" << std::endl;
        return false;
    }
    
    std::cout << "Connected to server!" << std::endl;
    isHost = false;
    isConnected = true;
    
    return true;
}

void NetworkManager::disconnect() {
    if (isConnected) {
        NetworkMessage msg;
        msg.type = NetworkMessageType::DISCONNECT;
        sendMessage(msg);
    }
    
    socket.disconnect();
    listener.close();
    isConnected = false;
}

bool NetworkManager::sendMessage(const NetworkMessage& message) {
    if (!isConnected) return false;
    
    sf::Packet packet;
    packet << static_cast<int>(message.type) 
           << message.playerID 
           << message.data1 
           << message.data2;
    
    if (socket.send(packet) != sf::Socket::Done) {
        return false;
    }
    
    return true;
}

bool NetworkManager::receiveMessage(NetworkMessage& message) {
    if (!isConnected) return false;
    
    sf::Packet packet;
    sf::Socket::Status status = socket.receive(packet);
    
    if (status == sf::Socket::Done) {
        int type;
        packet >> type >> message.playerID >> message.data1 >> message.data2;
        message.type = static_cast<NetworkMessageType>(type);
        return true;
    }
    
    if (status == sf::Socket::Disconnected) {
        std::cout << "Connection lost" << std::endl;
        isConnected = false;
    }
    
    return false;
}
