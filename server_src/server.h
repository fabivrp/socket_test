#ifndef SERVER_H
#define SERVER_H

#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include <arpa/inet.h>

#include "../common_src/car.h"
#include "../common_src/constantes.h"
#include "../common_src/socket.h"

#include "server_protocol.h"


class Server {
private:
    Socket socket;
    int client_money;
    Car current_car;
    std::vector<Car> cars;
    std::vector<Car> client_cars;


    Socket acceptClient();

public:

    explicit Server(const std::string& port);
    
    void handle_client();
    
    void fill_server_info(const std::string& filename);


    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;
    Server(Server&&) = default;
    Server& operator=(Server&&) = default;


    ~Server();
};

#endif
