#include "server.h"


Server::Server(const std::string& port): socket(port.c_str()) {

    this->client_money = 0;
    this->cars.clear();
    this->client_cars.clear();

    std::cout << "Server started" << std::endl;
}

void Server::fill_server_info(const std::string& filename) {

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "No se pudo abrir el archivo: " << filename << std::endl;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string keyword;
        iss >> keyword;

        if (keyword == "money") {
            int value;
            if (iss >> value) {
                this->client_money = value;
            }
        } else if (keyword == "car") {
            std::string model;
            int year;
            int price;

            if (iss >> model >> year >> price) {
                bool bought = false;

                if (!(iss >> std::boolalpha >> bought)) {
                    bought = false;
                }
                this->cars.emplace_back(model, year, price, bought);
            }
        }
    }
}

Socket Server::acceptClient() { return this->socket.accept(); }


void Server::handle_client() {
    
    Socket peer_socket = acceptClient();
    Server_Protocol protocol(std::move(peer_socket));

    uint8_t code = 0;


    while (protocol.get_code(code)) {

        switch (code) {
            case SEND_USERNAME: {
                protocol.greet_client();
                protocol.send_initial_money(this->client_money);
               
                break;
            }

            case SEND_CURRENT_CAR: {
                
                protocol.send_current_car(this->current_car);
                break;
            }

            case GET_MARKET_INFO: {
             
                protocol.send_market_info(this->cars);
                break;
            }

            case BUY_CAR: {
                
                protocol.send_car_bought(this->cars, this->client_money, this->client_cars,
                                         this->current_car);
                break;
            }

            default:
                std::cerr << "Opcode desconocido: " << (int)code << std::endl;
                break;
        }
    }
}

Server::~Server() {}
