// #include <iostream>

// struct lobby {
//     // explicit UdpServer(ip::udp::socket socket)
//     //   : socket_(std::move(socket)) {
//     //   read();
//     // }
//   private:
//     void read() {

//     }

//     void write() {

//     }
//     uint16_t lobbyId_;
//     std::string lobbyPassword_;
//     std::string lobbyName_;
//     std::string hostIp_;
//     std::string teacher_;
//     std::string joker_;
// };

// void getLobbyList() {

// }

// void makeLobby(std::string name, std::string password, std::string ip) {

// }

// void deleteLobby(std::string name, std::string password, std::string ip) {
  
// }

// void joinLobby(std::string name, std::string password) {

// }

// void sendDataToPlayerByLobby() {

// }


#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio.hpp>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

struct lobby {
  // private:
  //   void read() {

  //   }

  //   void write() {

  //   }
  // uint16_t id_;
  std::string password_;
  std::string name_;
  std::string ip_;
  std::string teacher_;
  std::string joker_;
};

uint16_t idCounter = 1;

std::unordered_map<uint16_t, lobby> lobbyMap;

namespace my_program_state {
  std::size_t
  request_count() {
    static std::size_t count = 0;
    return ++count;
  }

  std::time_t
  now() {
    return std::time(0);
  }
}

class http_connection : public std::enable_shared_from_this<http_connection> {
  public:
    http_connection(tcp::socket socket)
        : socket_(std::move(socket)) {
    }

    void start() {
      read_request();
      check_deadline();
    }

  private:
    tcp::socket socket_;
    beast::flat_buffer buffer_{8192};
    http::request<http::dynamic_body> request_;
    http::response<http::dynamic_body> response_;
    net::steady_timer deadline_{
      socket_.get_executor(), std::chrono::seconds(60)};

    void read_request() {
      auto self = shared_from_this();

      http::async_read(
        socket_,
        buffer_,
        request_,
        [self](beast::error_code ec,
          std::size_t bytes_transferred) {
            boost::ignore_unused(bytes_transferred);
            if(!ec) {
              self->process_request();
            }
          });
    }

    void process_request() {
      response_.version(request_.version());
      response_.keep_alive(false);

      switch(request_.method()) {
      case http::verb::get:
        response_.result(http::status::ok);
        response_.set(http::field::server, "Beast");
        create_response();
        break;

      default:
        response_.result(http::status::bad_request);
        response_.set(http::field::content_type, "text/plain");
        beast::ostream(response_.body())
            << "Invalid request-method '"
            << std::string(request_.method_string())
            << "'";
        break;
      }

      write_response();
    }

    void
    create_response()
    {
        if(request_.target() == "/lobbies")
        {
            response_.set(http::field::content_type, "application/json");
            beast::ostream(response_.body())
                << "<html>\n"
                <<  "<head><title>Request count</title></head>\n"
                <<  "<body>\n"
                <<  "<h1>Request count</h1>\n"
                <<  "<p>There have been "
                <<  my_program_state::request_count()
                <<  " requests so far.</p>\n"
                <<  "</body>\n"
                <<  "</html>\n";
        }
        else if(request_.target() == "/makeLobby")
        {
            response_.set(http::field::content_type, "application/json");
            beast::ostream(response_.body())
                <<  "<html>\n"
                <<  "<head><title>Current time</title></head>\n"
                <<  "<body>\n"
                <<  "<h1>Current time</h1>\n"
                <<  "<p>The current time is "
                <<  my_program_state::now()
                <<  " seconds since the epoch.</p>\n"
                <<  "</body>\n"
                <<  "</html>\n";
        }
        else if(request_.target() == "/deleteLobby")
        {
            response_.set(http::field::content_type, "application/json");
            beast::ostream(response_.body())
                <<  "<html>\n"
                <<  "<head><title>Current time</title></head>\n"
                <<  "<body>\n"
                <<  "<h1>Current time</h1>\n"
                <<  "<p>The current time is "
                <<  my_program_state::now()
                <<  " seconds since the epoch.</p>\n"
                <<  "</body>\n"
                <<  "</html>\n";
        }
        else if(request_.target() == "/leaveLobby")
        {
            response_.set(http::field::content_type, "application/json");
            beast::ostream(response_.body())
                <<  "<html>\n"
                <<  "<head><title>Current time</title></head>\n"
                <<  "<body>\n"
                <<  "<h1>Current time</h1>\n"
                <<  "<p>The current time is "
                <<  my_program_state::now()
                <<  " seconds since the epoch.</p>\n"
                <<  "</body>\n"
                <<  "</html>\n";
        }
        else if(request_.target() == "/joinLobby")
        {
            response_.set(http::field::content_type, "application/json");
            beast::ostream(response_.body())
                <<  "<html>\n"
                <<  "<head><title>Current time</title></head>\n"
                <<  "<body>\n"
                <<  "<h1>Current time</h1>\n"
                <<  "<p>The current time is "
                <<  my_program_state::now()
                <<  " seconds since the epoch.</p>\n"
                <<  "</body>\n"
                <<  "</html>\n";
        }
        else if(request_.target() == "/startGame")
        {
            response_.set(http::field::content_type, "application/json");
            beast::ostream(response_.body())
                <<  "<html>\n"
                <<  "<head><title>Current time</title></head>\n"
                <<  "<body>\n"
                <<  "<h1>Current time</h1>\n"
                <<  "<p>The current time is "
                <<  my_program_state::now()
                <<  " seconds since the epoch.</p>\n"
                <<  "</body>\n"
                <<  "</html>\n";
        }
        else
        {
            response_.result(http::status::not_found);
            response_.set(http::field::content_type, "text/plain");
            beast::ostream(response_.body()) << "File not found\r\n";
        }
    }

    void
    write_response()
    {
        auto self = shared_from_this();

        response_.content_length(response_.body().size());

        http::async_write(
            socket_,
            response_,
            [self](beast::error_code ec, std::size_t)
            {
                self->socket_.shutdown(tcp::socket::shutdown_send, ec);
                self->deadline_.cancel();
            });
    }

    void
    check_deadline()
    {
        auto self = shared_from_this();

        deadline_.async_wait(
            [self](beast::error_code ec)
            {
                if(!ec)
                {
                    self->socket_.close(ec);
                }
            });
    }
};

void http_server(tcp::acceptor& acceptor, tcp::socket& socket) {
  acceptor.async_accept(socket,
    [&](beast::error_code ec) {
      if(!ec) {
        std::make_shared<http_connection>(std::move(socket))->start();
      }
      http_server(acceptor, socket);
    });
}

int main(int argc, char* argv[]) {
  try {
    if(argc != 3) {
      std::cerr << "Usage: " << argv[0] << " <address> <port>\n";
      std::cerr << "  For IPv4, try:\n";
      std::cerr << "    receiver 0.0.0.0 80\n";
      std::cerr << "  For IPv6, try:\n";
      std::cerr << "    receiver 0::0 80\n";
      return EXIT_FAILURE;
    }

    auto const address = net::ip::make_address(argv[1]);
    unsigned short port = static_cast<unsigned short>(std::atoi(argv[2]));

    net::io_context ioc{1};

    tcp::acceptor acceptor{ioc, {address, port}};
    tcp::socket socket{ioc};
    http_server(acceptor, socket);

    ioc.run();
  } catch(std::exception const& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }
}