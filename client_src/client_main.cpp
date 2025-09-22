#include <exception>
#include <fstream>
#include <iostream>
#include <string>

#include "../common_src/socket.h"

#include "client.h"
#include "client_protocol.h"


int main(int argc, char* argv[]) {

    if (argc != 4) {
        std::cerr << "Uso: " << argv[0] << " <hostname> <servicio> <nombre-archivo>" << std::endl;
        return 1;
    }

    const char* host = argv[1];
    const char* service = argv[2];
    const char* filename = argv[3];


    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo " << filename << std::endl;
        return 1;
    }

    Client client(host, service);
    
    std::string line;

    while (std::getline(file, line)) {

        client.process_petition(line);
    }


    return 0;
}
