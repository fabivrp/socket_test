#ifndef CLIENT_PROTOCOL_H
#define CLIENT_PROTOCOL_H

#include <cstdint>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include <arpa/inet.h>

#include "../common_src/constantes.h"
#include "../common_src/socket.h"

#include "client_codes_parser.h"


class Client_Protocol {
private:
    Socket socket;

    // Fuinciones:
    void send_username(const std::string& username);
    void recieve_money();

    void get_current_car();
    void recieve_current_car();

    void get_market_info();
    void recieve_market();

    void buy_car(const std::string& car_name);
    void recieve_car();

    void recieve_message();

public:
    Client_Protocol(const std::string& hostname, const std::string& port);

    void process_get_current_car_intruction();
    
    void process_get_market_info_intruction();
    
    void process_buy_car_intruction(const std::string& car_name);
    
    void process_send_username_intruction(const std::string& username);
    
    ~Client_Protocol();
};

#endif
