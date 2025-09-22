#ifndef CLIENT_H
#define CLIENT_H

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
#include "client_protocol.h"


class Client {
private:
    Client_Protocol protocol;
    Client_Parser parser;

public:
    Client(const std::string& hostname, const std::string& port);

    void process_petition(const std::string& buffer);

    ~Client();
};

#endif
