#include "ServerCommunication.hpp"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cerrno>
#include <iostream>
#include <stdexcept>
#include <string>

Network::ClientCommunication::ClientCommunication(int port,
                                                  const std::string &hostname)
    : _clientFd(-1), _port(port), _hostname(hostname), _connected(false) {
  std::memset(&_serverAddr, 0, sizeof(_serverAddr));
}

Network::ClientCommunication::~ClientCommunication() {
  if (_clientFd != -1) {
    close(_clientFd);
    _clientFd = -1;
  }
  _connected = false;
}

bool Network::ClientCommunication::isConnected() const {
  return _connected;
}

void Network::ClientCommunication::connectToServer() {
  try {
    createSocket();
    establishConnection();
  } catch (const std::exception &e) {
    std::cerr << "Connection failed: " << e.what() << std::endl;
  }
}

void Network::ClientCommunication::createSocket() {
  _clientFd = socket(AF_INET, SOCK_STREAM, 0);
  if (_clientFd == -1)
    throw std::runtime_error("Failed to create socket: " +
                             std::string(strerror(errno)));
}

void Network::ClientCommunication::establishConnection() {
  _serverAddr.sin_family = AF_INET;
  _serverAddr.sin_port = htons(_port);
  if (inet_pton(AF_INET, _hostname.c_str(), &_serverAddr.sin_addr) <= 0)
    throw std::runtime_error("Invalid address: " + _hostname);
  if (connect(_clientFd, (struct sockaddr *)&_serverAddr, sizeof(_serverAddr)) <
      0) {
    close(_clientFd);
    _clientFd = -1;
    throw std::runtime_error("Connection failed: " +
                             std::string(strerror(errno)));
  }
  _connected = true;
  sendMessage("GRAPHIC\n");
}

void Network::ClientCommunication::sendMessage(const std::string &message) {
  if (!_connected || _clientFd == -1)
    throw std::runtime_error("Not connected to server");

  ssize_t bytesSent = send(_clientFd, message.c_str(), message.length(), 0);
  if (bytesSent == -1)
    throw std::runtime_error("Failed to send message: " +
                             std::string(strerror(errno)));
  std::cout << "Sent to server: " << message;
}

std::string Network::ClientCommunication::receiveMessage() {
  if (!_connected || _clientFd == -1)
    throw std::runtime_error("Not connected to server");
  char buffer[1024];
  ssize_t bytesReceived = recv(_clientFd, buffer, sizeof(buffer) - 1, 0);
  if (bytesReceived == -1)
    throw std::runtime_error("Failed to receive message: " +
                             std::string(strerror(errno)));
  if (bytesReceived == 0) {
    _connected = false;
    return "";
  }
  buffer[bytesReceived] = '\0';
  return std::string(buffer);
}
