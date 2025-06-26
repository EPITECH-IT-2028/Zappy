#include "ServerCommunication.hpp"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>

network::ServerCommunication::ServerCommunication(int port,
                                                  const std::string &hostname)
    : _clientFd(-1),
      _port(port),
      _hostname(hostname),
      _connected(false),
      _pendingData() {
  std::memset(&_serverAddr, 0, sizeof(_serverAddr));
}

network::ServerCommunication::~ServerCommunication() {
  if (_clientFd != -1) {
    close(_clientFd);
    _clientFd = -1;
  }
  _connected = false;
}

bool network::ServerCommunication::isConnected() const noexcept {
  return _connected;
}

void network::ServerCommunication::createSocket() {
  _clientFd = socket(AF_INET, SOCK_STREAM, 0);
  if (_clientFd == -1)
    throw std::runtime_error("Failed to create socket: " +
                             std::string(strerror(errno)));
}

void network::ServerCommunication::establishConnection() {
  _serverAddr.sin_family = AF_INET;
  _serverAddr.sin_port = htons(_port);
  if (inet_pton(AF_INET, _hostname.c_str(), &_serverAddr.sin_addr) <= 0)
    throw std::runtime_error("Invalid address: " + _hostname);
  if (connect(_clientFd, (struct sockaddr *)&_serverAddr, sizeof(_serverAddr)) <
      0) {
    close(_clientFd);
    _clientFd = -1;
    throw std::runtime_error(std::string(strerror(errno)));
  }
  _connected = true;
}

bool network::ServerCommunication::connectToServer() {
  try {
    createSocket();
    establishConnection();
    sendMessage("GRAPHIC\n");
    return true;
  } catch (const std::exception &e) {
    std::cerr << "Connection failed: " << e.what() << std::endl;
    return false;
  }
}

void network::ServerCommunication::sendMessage(const std::string &message) {
  if (!_connected || _clientFd == -1)
    throw std::runtime_error("Not connected to server");

  size_t totalSent = 0;
  size_t messageLength = message.length();

  while (totalSent < messageLength) {
    ssize_t bytesSent = send(_clientFd, message.c_str() + totalSent,
                             messageLength - totalSent, 0);
    if (bytesSent == -1) {
      if (errno == EINTR)
        continue;
      throw std::runtime_error("Failed to send message: " +
                               std::string(strerror(errno)));
    }
    totalSent += bytesSent;
  }
  std::cout << "Sent to server: " << message;
}

std::string network::ServerCommunication::receiveMessage() {
  if (!_connected || _clientFd == -1)
    throw std::runtime_error("Not connected to server");

  size_t newlinePos = _pendingData.find('\n');
  if (newlinePos != std::string::npos) {
    std::string message = _pendingData.substr(0, newlinePos + 1);
    _pendingData.erase(0, newlinePos + 1);
    return message;
  }

  char buffer[1024];
  bool dataReceived = false;
  while (true) {
    ssize_t bytesReceived = recv(_clientFd, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived == -1) {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        if (dataReceived) {
          newlinePos = _pendingData.find('\n');
          if (newlinePos != std::string::npos) {
            std::string message = _pendingData.substr(0, newlinePos + 1);
            _pendingData.erase(0, newlinePos + 1);
            return message;
          }
        }
        return "";
      }
      if (errno == EINTR)
        continue;
      throw std::runtime_error("Failed to receive message: " +
                               std::string(strerror(errno)));
    }
    if (bytesReceived == 0) {
      _connected = false;
      throw std::runtime_error("Connection closed by server");
    }
    buffer[bytesReceived] = '\0';
    _pendingData.append(buffer, bytesReceived);
    dataReceived = true;
    if (bytesReceived < static_cast<ssize_t>(sizeof(buffer) - 1))
      break;
  }
  newlinePos = _pendingData.find('\n');
  if (newlinePos != std::string::npos) {
    std::string message = _pendingData.substr(0, newlinePos + 1);
    _pendingData.erase(0, newlinePos + 1);
    return message;
  }
  return "";
}

bool network::ServerCommunication::hasIncomingData() {
  if (!_connected || _clientFd == -1)
    return false;
  if (!_pendingData.empty())
    return _pendingData.find('\n') != std::string::npos;

  struct pollfd pfd;
  pfd.fd = _clientFd;
  pfd.events = POLLIN;
  pfd.revents = 0;

  int result = poll(&pfd, 1, 0);
  if (result == -1) {
    if (errno != EINTR)
      std::cerr << "Poll error: " << strerror(errno) << std::endl;
    return false;
  }
  return result > 0 && (pfd.revents & POLLIN);
}

