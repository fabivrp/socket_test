#include <exception>
#include <fstream>
#include <iostream>
#include <string>

#include "../common_src/socket.h"

#include "server.h"
#include "server_protocol.h"

int main(int argc, const char* argv[]) {
    try {
        if (argc != 3) {
            std::cerr << "Uso: " << argv[0] << " <puerto> <nombre-archivo>" << std::endl;

            return 1;
        }

        const char* port = argv[1];
        const std::string filename = argv[2];

        Server server(port);
        server.fill_server_info(filename);
        server.handle_client();

    } catch (const std::exception& err) {
        std::cerr << "Error: " << err.what() << std::endl;
        return 1;
    }
}
