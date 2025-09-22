#ifndef SERVER_PROTOCOL_H
#define SERVER_PROTOCOL_H

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


class Server_Protocol {
private:
    Socket socket;

    bool purchase_car(Car& carint, int& client_money, std::vector<Car>& client_cars,
                      Car& current_car);

public:

    explicit Server_Protocol(Socket&& peer_socket);

    void greet_client();

    void send_error_message();
    
    uint8_t get_code(uint8_t& code);
    
    std::string recieve_car_bought();
    
    void send_market_info(std::vector<Car>& cars);
    
    void send_current_car(const Car& current_car);
    
    void send_initial_money(const int& client_money);
    
    void send_car_bought(std::vector<Car>& cars, int& client_money, std::vector<Car>& client_cars,
                         Car& current_car);

    Server_Protocol(const Server_Protocol&) = delete;
    Server_Protocol& operator=(const Server_Protocol&) = delete;
    Server_Protocol(Server_Protocol&&) = default;
    Server_Protocol& operator=(Server_Protocol&&) = default;


    ~Server_Protocol();
};

#endif
