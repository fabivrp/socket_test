#include "car.h"



Car::Car(){
        this->model = INVALID_CAR_MODEL;
        this->is_bought = false;
        this->price = INVALID_CAR_PRICE;
        this->year_of_fabrication = INVALID_YEAR_OF_FABRICATION;
}
Car::Car(const std::string& model, int year, int price, bool is_bought):
        model(model), year_of_fabrication(year), price(price), is_bought(is_bought) {}


std::string Car::get_model() const { return model; }

int Car::get_year_of_fabrication() const { return year_of_fabrication; }

int Car::get_price() const { return price; }

bool Car::get_is_bought() const { return is_bought; }


bool Car::is_car_valid() const { return (this->model != INVALID_CAR_MODEL); }


void Car::set_model(const std::string& new_model) { model = new_model; }

void Car::set_year_of_fabrication(int new_year) { year_of_fabrication = new_year; }

// void Car::set_price(int new_price) { price = new_price; }

void Car::set_is_bought(bool bought) { is_bought = bought; }
