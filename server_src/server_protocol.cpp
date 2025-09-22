#include "server_protocol.h"

// ---------------- Constructor ----------------
Server_Protocol::Server_Protocol(Socket&& peer_socket): socket(std::move(peer_socket)) {}


// ---------------- Métodos públicos ----------------
uint8_t Server_Protocol::get_code(uint8_t& code) {


    ssize_t n = this->socket.recvall(&code, sizeof(code));
    if (n == 0) {
        return false;  // el cliente cerró
    }

    return true;
}
void Server_Protocol::greet_client() {

    uint16_t name_lenght;

    if (!this->socket.recvall(&name_lenght, sizeof(name_lenght))) {
        std::cerr << "Error leyendo longitud\n";
    }
    uint16_t name_len = ntohs(name_lenght);

    std::string username(name_len, '\0');
    if (!this->socket.recvall(username.data(), name_len)) {
        std::cerr << "Error leyendo username\n";
    }


    std::string message = GREETINGS_MESSAGE + username;
    std::cout << message << std::endl;
}


void Server_Protocol::send_initial_money(const int& client_money) {
    uint8_t code = SEND_INITIAL_MONEY;

    this->socket.sendall(&code, sizeof(code));


    uint32_t money = static_cast<uint32_t>(client_money);


    uint32_t converted_money = htonl(money);

    this->socket.sendall(&converted_money, sizeof(converted_money));

    std::string message = INITIAL_BALANCE_MESSAGE + std::to_string(client_money);

    std::cout << message << std::endl;
}


void Server_Protocol::send_market_info(std::vector<Car>& cars) {

    std::vector<Car> available;
    available.reserve(cars.size());

    std::copy_if(cars.begin(), cars.end(), std::back_inserter(available),
                 [](const Car& c) { return !c.get_is_bought(); });


    uint8_t code = SEND_MARKET_INFO;
    this->socket.sendall(&code, sizeof(code));


    uint16_t count_be = htons(static_cast<uint16_t>(available.size()));
    this->socket.sendall(&count_be, sizeof(count_be));


    for (const auto& car: available) {

        uint16_t name_len = htons(static_cast<uint16_t>(car.get_model().size()));
        this->socket.sendall(&name_len, sizeof(name_len));


        this->socket.sendall(car.get_model().data(), car.get_model().size());


        uint16_t year_be = htons(static_cast<uint16_t>(car.get_year_of_fabrication()));
        this->socket.sendall(&year_be, sizeof(year_be));


        uint32_t price_cents = static_cast<uint32_t>(car.get_price() * 100);
        uint32_t price_be = htonl(price_cents);
        this->socket.sendall(&price_be, sizeof(price_be));
    }


    std::cout << available.size() << MARKET_SENT_MESSAGE << std::endl;
}

std::string Server_Protocol::recieve_car_bought() {
    uint16_t name_lenght;

    if (!this->socket.recvall(&name_lenght, sizeof(name_lenght))) {
        std::cerr << "Error leyendo longitud\n";
    }
    uint16_t name_len = ntohs(name_lenght);

    std::string car_name(name_len, '\0');
    if (!this->socket.recvall(car_name.data(), name_len)) {
        std::cerr << "Error leyendo username\n";
    }

    return car_name;
}

bool Server_Protocol::purchase_car(Car& car, int& client_money, std::vector<Car>& client_cars,
                                   Car& current_car) {
    bool successful_purchase = UNSUCCESSFUL_PURCHASE;
    if ((car.get_price() <= client_money) and !car.get_is_bought()) {
        car.set_is_bought(true);
        client_money -= car.get_price();
        client_cars.push_back(car);
        current_car = car;
        successful_purchase = SUCCESSFUL_PURCHASE;
    }

    return successful_purchase;
}

void Server_Protocol::send_car_bought(std::vector<Car>& cars, int& client_money,
                                      std::vector<Car>& client_cars, Car& current_car) {

    uint8_t code = SEND_CAR_BOUGHT;

    std::string model_requested = recieve_car_bought();

    auto it = std::find_if(cars.begin(), cars.end(), [&model_requested](const Car& car) {
        return car.get_model() == model_requested;
    });

    if (purchase_car(*it, client_money, client_cars, current_car)) {
        const Car& car = *it;


        this->socket.sendall(&code, sizeof(code));


        uint16_t name_len = htons(static_cast<uint16_t>(car.get_model().size()));
        this->socket.sendall(&name_len, sizeof(name_len));


        this->socket.sendall(car.get_model().data(), car.get_model().size());


        uint16_t year = htons(static_cast<uint16_t>(car.get_year_of_fabrication()));
        this->socket.sendall(&year, sizeof(year));


        uint32_t price_cents = static_cast<uint32_t>(car.get_price() * 100);
        uint32_t price_be = htonl(price_cents);
        this->socket.sendall(&price_be, sizeof(price_be));


        uint32_t money_be = htonl(client_money);
        this->socket.sendall(&money_be, sizeof(money_be));

        std::cout << "New cars name: " << car.get_model()
                  << " --- remaining balance: " << client_money << std::endl;
    } else {

        std::cout << "Error: " << INSUFFICIENT_FUNDS_MESSAGE << std::endl;

        uint8_t error_code = SEND_ERROR_MESSAGE;
        this->socket.sendall(&error_code, sizeof(error_code));

        std::string err_msg = INSUFFICIENT_FUNDS_MESSAGE;
        uint16_t len_be = htons(static_cast<uint16_t>(err_msg.size()));
        this->socket.sendall(&len_be, sizeof(len_be));
        this->socket.sendall(err_msg.data(), err_msg.size());
    }
}

void Server_Protocol::send_current_car(const Car& current_car) {

    uint8_t code = GET_CURRENT_CAR;


    if (current_car.get_model() != INVALID_CAR_MODEL) {
        const Car& car = current_car;


        this->socket.sendall(&code, sizeof(code));


        uint16_t name_len = htons(static_cast<uint16_t>(car.get_model().size()));
        this->socket.sendall(&name_len, sizeof(name_len));


        this->socket.sendall(car.get_model().data(), car.get_model().size());


        uint16_t year = htons(static_cast<uint16_t>(car.get_year_of_fabrication()));
        this->socket.sendall(&year, sizeof(year));


        uint32_t price_cents = static_cast<uint32_t>(car.get_price() * 100);
        uint32_t price_be = htonl(price_cents);
        this->socket.sendall(&price_be, sizeof(price_be));

        std::cout << "Car " << car.get_model() << " " << car.get_price() << " "
                  << car.get_year_of_fabrication() << " sent" << std::endl;
    } else {

        std::cout << "Error: " << NO_CAR_BOUGHT_MESSAGE << std::endl;

        uint8_t error_code = SEND_ERROR_MESSAGE;
        this->socket.sendall(&error_code, sizeof(error_code));

        std::string err_msg = NO_CAR_BOUGHT_MESSAGE;
        uint16_t len_be = htons(static_cast<uint16_t>(err_msg.size()));
        this->socket.sendall(&len_be, sizeof(len_be));
        this->socket.sendall(err_msg.data(), err_msg.size());
    }
}


void Server_Protocol::send_error_message() {
    // TODO:por hacer
}

// ---------------- Destructor ----------------
Server_Protocol::~Server_Protocol() { socket.shutdown(1); }
