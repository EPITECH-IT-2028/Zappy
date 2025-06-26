#pragma once

#include <netinet/in.h>
#include <string>

namespace network {
  class ServerCommunication {
    public:
      ServerCommunication(int port, const std::string &hostname = "127.0.0.1");
      ~ServerCommunication();

      bool isConnected() const noexcept;
      bool connectToServer();
      void sendMessage(const std::string &message);
      std::string receiveMessage();
      bool hasIncomingData();

    private:
      void createSocket();
      void establishConnection();

      int _clientFd;
      int _port;
      std::string _hostname;
      struct sockaddr_in _serverAddr;
      bool _connected;
      std::string _pendingData;
  };
}  // namespace network
