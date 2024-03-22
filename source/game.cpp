#include <iostream>
#include <boost/asio.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
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

      std::cout << "received data: " << data_ << std::endl;

      try {
        json j = json::parse(data_, data_ + length);

        std::string from = remote_endpoint_.address().to_string();
        std::string to = j["to"];
        std::string data = j["data"];

        std::cout << "To: " << to << std::endl;
        std::cout << "From: " << from << std::endl;
        std::cout << "Data: " << data << std::endl;

        json response = {
          {"to", from},
          {"from", to},
          {"data", boost::algorithm::to_upper_copy(data)}
        };

        // Send the response to the specified IP address
        ip::udp::endpoint destination_endpoint(ip::address::from_string(to), remote_endpoint_.port());
        socket_.async_send_to(buffer(response.dump()), destination_endpoint,
          [this](boost::system::error_code ec, std::size_t length) {
            if (ec) return;
            data_[0] = '\0';
            this->read();
          }
        );

      } catch (const std::exception& e) {
        std::cerr << "Error parsing JSON: " << e.what() << std::endl;
        data_[0] = '\0';
        this->read();
      }
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
    // std::vector<lobby> lobbyList;
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