#include <iostream>

struct lobby {
    // explicit UdpServer(ip::udp::socket socket)
    //   : socket_(std::move(socket)) {
    //   read();
    // }
  private:
    void read() {

    }

    void write() {

    }
    uint16_t lobbyId_;
    std::string lobbyPassword_;
    std::string lobbyName_;
    std::string hostIp_;
    std::string teacher_;
    std::string joker_;
};

void getLobbyList() {

}

void makeLobby(std::string name, std::string password, std::string ip) {

}

void deleteLobby(std::string name, std::string password, std::string ip) {
  
}

void joinLobby(std::string name, std::string password) {

}

void sendDataToPlayerByLobby() {

}