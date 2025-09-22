#include "client_protocol.h"

Client_Protocol::Client_Protocol(const std::string& hostname, const std::string& port):
        socket(hostname.c_str(), port.c_str()) {}

void Client_Protocol::process_send_username_intruction(const std::string& username) {

    send_username(username);
    recieve_money();
}


void Client_Protocol::process_get_current_car_intruction() {

    get_current_car();
    recieve_current_car();
}


void Client_Protocol::process_get_market_info_intruction() {

    get_market_info();
    recieve_market();
}


void Client_Protocol::process_buy_car_intruction(const std::string& car_name) {

    buy_car(car_name);
    recieve_car();
}


void Client_Protocol::send_username(const std::string& username) {

    uint8_t code = SEND_USERNAME;
    std::string name = (username);

    this->socket.sendall(&code, sizeof(code));


    uint16_t name_len = static_cast<uint16_t>(name.size());
    uint16_t name_lenght = htons(name_len);
    this->socket.sendall(&name_lenght, sizeof(name_lenght));
    this->socket.sendall(name.data(), name.size());
}

void Client_Protocol::recieve_money() {

    uint8_t code;
    uint32_t net_money;


    if (!this->socket.recvall(&code, sizeof(code))) {
        std::cerr << "Error: no se pudo recibir el código" << std::endl;
        return;
    }


    if (code != SEND_INITIAL_MONEY) {
        std::cerr << "Código recibido incorrecto: " << (int)code << std::endl;
        return;
    }


    if (!this->socket.recvall(&net_money, sizeof(net_money))) {
        std::cerr << "Error: no se pudo recibir el saldo" << std::endl;
        return;
    }


    uint32_t balance = ntohl(net_money);


    std::cout << "Initial balance: " << balance << std::endl;
}


void Client_Protocol::get_current_car() {

    int8_t code = SEND_CURRENT_CAR;
    this->socket.sendall(&code, sizeof(code));
}


void Client_Protocol::get_market_info() {
    int8_t code = GET_MARKET_INFO;
    this->socket.sendall(&code, sizeof(code));
}


void Client_Protocol::buy_car(const std::string& car_name) {
    uint8_t code = BUY_CAR;
    std::string name = (car_name);

    this->socket.sendall(&code, sizeof(code));

    uint16_t name_len = static_cast<uint16_t>(name.size());
    uint16_t name_lenght = htons(name_len);

    this->socket.sendall(&name_lenght, sizeof(name_lenght));
    this->socket.sendall(name.data(), name.size());
}

void Client_Protocol::recieve_current_car() {

    uint8_t code;
    if (!socket.recvall(&code, sizeof(code))) {
        std::cerr << "Error recibiendo código\n";
        return;
    }

    if (code == SEND_ERROR_MESSAGE) {


        recieve_message();
        return;
    } else {


        uint16_t name_len_be;
        socket.recvall(&name_len_be, sizeof(name_len_be));
        uint16_t name_len = ntohs(name_len_be);


        std::string model(name_len, '\0');
        socket.recvall(model.data(), name_len);


        uint16_t year_be;
        socket.recvall(&year_be, sizeof(year_be));
        uint16_t year = ntohs(year_be);


        uint32_t price_be;
        socket.recvall(&price_be, sizeof(price_be));
        uint32_t price = ntohl(price_be);

        double price_dollars = static_cast<double>(price) / 100.0;


        std::cout << "Current car: " << model << ", year: " << year << ", price: " << std::fixed
                  << std::setprecision(2) << price_dollars << '\n';
    }
}

void Client_Protocol::recieve_market() {
    uint8_t code;
    if (!socket.recvall(&code, sizeof(code))) {
        std::cerr << "Error leyendo código de respuesta\n";
        return;
    }
    if (code != SEND_MARKET_INFO) {
        std::cerr << "Código inesperado: " << static_cast<int>(code) << '\n';
        return;
    }


    uint16_t num_cars_be;
    if (!socket.recvall(&num_cars_be, sizeof(num_cars_be))) {
        std::cerr << "Error leyendo cantidad de autos\n";
        return;
    }
    uint16_t num_cars = ntohs(num_cars_be);


    for (uint16_t i = 0; i < num_cars; ++i) {

        uint16_t name_len_be;
        socket.recvall(&name_len_be, sizeof(name_len_be));
        uint16_t name_len = ntohs(name_len_be);


        std::string model(name_len, '\0');
        socket.recvall(model.data(), name_len);


        uint16_t year_be;
        socket.recvall(&year_be, sizeof(year_be));
        uint16_t year = ntohs(year_be);


        uint32_t price_be;
        socket.recvall(&price_be, sizeof(price_be));
        uint32_t price_cents = ntohl(price_be);
        double price = static_cast<double>(price_cents) / 100.0;


        std::cout << model << ", year: " << year << ", price: " << std::fixed
                  << std::setprecision(2) << price << '\n';
    }
}

void Client_Protocol::recieve_car() {

    uint8_t code;
    if (!socket.recvall(&code, sizeof(code))) {
        std::cerr << "Error recibiendo código\n";
        return;
    }
    if (code == SEND_ERROR_MESSAGE) {
        recieve_message();
        return;
    }


    uint16_t name_len_be;
    socket.recvall(&name_len_be, sizeof(name_len_be));
    uint16_t name_len = ntohs(name_len_be);


    std::string model(name_len, '\0');
    socket.recvall(model.data(), name_len);


    uint16_t year_be;
    socket.recvall(&year_be, sizeof(year_be));
    uint16_t year = ntohs(year_be);


    uint32_t price_be;
    socket.recvall(&price_be, sizeof(price_be));
    uint32_t price = ntohl(price_be);


    uint32_t balance_be;
    socket.recvall(&balance_be, sizeof(balance_be));
    uint32_t balance = ntohl(balance_be);

    double price_dollars = static_cast<double>(price) / 100.0;


    std::cout << "Car bought: " << model << ", year: " << year << ", price: " << std::fixed
              << std::setprecision(2) << price_dollars << '\n'
              << "Remaining balance: " << balance << '\n';
}


void Client_Protocol::recieve_message() {


    uint16_t msg_len_be;
    if (!socket.recvall(&msg_len_be, sizeof(msg_len_be))) {
        std::cerr << "Error leyendo longitud\n";
        return;
    }
    uint16_t msg_len = ntohs(msg_len_be);


    std::string message(msg_len, '\0');
    if (!socket.recvall(message.data(), msg_len)) {
        std::cerr << "Error leyendo mensaje\n";
        return;
    }

    std::cout << "Error: " << message << '\n';
}


Client_Protocol::~Client_Protocol() {


    socket.shutdown(1);
    // socket.close();
}
