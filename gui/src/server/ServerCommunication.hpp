#pragma once

#include <netinet/in.h>
#include <poll.h>
#include <string>

namespace Network {
  class ClientCommunication {
    public:
      ClientCommunication(int port, const std::string &hostname = "127.0.0.1");
      ~ClientCommunication();

      bool isConnected() const;
      bool connectToServer();
      void sendMessage(const std::string &message);
      std::string receiveMessage();

    private:
      void createSocket();
      void establishConnection();

      int _clientFd;
      int _port;
      std::string _hostname;
      struct sockaddr_in _serverAddr;
      bool _connected;
  };
}  // namespace Network
