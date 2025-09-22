#include "client_codes_parser.h"

Client_Parser::Client_Parser():
        instruction_codes({{"USERNAME", SEND_USERNAME},
                           {"GET_CURRENT_CAR", SEND_CURRENT_CAR},
                           {"GET_MARKET", GET_MARKET_INFO},
                           {"BUY_CAR", BUY_CAR}}) {}


std::string Client_Parser::get_name(const std::string& input) {
    size_t pos = input.find(' ');
    return input.substr(pos + 1);
}


std::string Client_Parser::to_upper(std::string& instruction) {
    std::transform(instruction.begin(), instruction.end(), instruction.begin(), ::toupper);
    return instruction;
}

uint8_t Client_Parser::parse_instruction(const std::string& input) {
    size_t pos = input.find(' ');

    std::string instruction = input;
    if (pos != std::string::npos) {
        instruction = input.substr(0, pos);
    }

    instruction = to_upper(instruction);

    auto it = instruction_codes.find(instruction);
    if (it != instruction_codes.end()) {

        return it->second;
    }

    return INVALID_CODE;
}
