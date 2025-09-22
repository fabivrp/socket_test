#include "client.h"

Client::Client(const std::string& hostname, const std::string& port):
        protocol(hostname, port), parser() {}


void Client::process_petition(const std::string& buffer) {

    uint8_t code = this->parser.parse_instruction(buffer);

    switch (code) {
        case SEND_USERNAME: {

            this->protocol.process_send_username_intruction(this->parser.get_name(buffer));

            break;
        }
        case SEND_CURRENT_CAR:

            this->protocol.process_get_current_car_intruction();

            break;

        case GET_MARKET_INFO:

            this->protocol.process_get_market_info_intruction();

            break;

        case BUY_CAR:

            this->protocol.process_buy_car_intruction(this->parser.get_name(buffer));

            break;

        default:

            std::cerr << "Código desconocido: " << (int)code << std::endl;
            break;
    }
}


Client::~Client() {}
