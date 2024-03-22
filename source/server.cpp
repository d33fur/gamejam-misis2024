#include <iostream>
#include <boost/asio.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include "lobby.cpp"

using namespace boost::asio;

struct UdpServer {
    explicit UdpServer(ip::udp::socket socket)
      : socket_(std::move(socket)) {
      read();
    }

  private:
    void read() {
      socket_.async_receive_from(buffer(data_, 1500),
        remote_endpoint_,
        [this](boost::system::error_code ec, std::size_t length) {
          if(ec) {
            return;
          }
          std::cout << "Sender's IP address: " << remote_endpoint_.address().to_string() << std::endl;
          std::cout << "data received: " << data_ << std::endl;
          data_[length] = '\0';
          if(strcmp(data_, "\n") == 0) {
            return;
          }
          boost::algorithm::to_upper(data_);
          this->write();
        }
      );
    }

    void write() {
      socket_.async_send_to(buffer(data_, strlen(data_)),
        remote_endpoint_,
        [this](boost::system::error_code ec, std::size_t length) {
          if (ec) return;
          data_[0] = '\0';
          this->read();
        }
      );
    }
    ip::udp::socket socket_;
    ip::udp::endpoint remote_endpoint_;
    char data_[1500 + 1]; // +1 for we can always null terminate safely
    std::vector<lobby> lobbyList;
};


int main() {
  try {
    io_context io_context;
    ip::udp::endpoint ep(ip::udp::v6(), 1895); // also listens on ipv4
    ip::udp::socket sock(io_context, ep);
    UdpServer server(std::move(sock));
    io_context.run();
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}