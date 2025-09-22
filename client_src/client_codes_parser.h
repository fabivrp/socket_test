#ifndef CLIENT_CODES_PARSER_H
#define CLIENT_CODES_PARSER_H

#include <algorithm>
#include <cctype>
#include <cstdint>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "../common_src/constantes.h"


class Client_Parser {
private:
    std::unordered_map<std::string, uint8_t> instruction_codes;

    std::string to_upper(std::string& instruction);


public:
    Client_Parser();

    uint8_t parse_instruction(const std::string& input);

    std::string get_name(const std::string& input);
};

#endif
