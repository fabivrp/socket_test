#ifndef CAR_H
#define CAR_H

#include <string>
#include "constantes.h"

class Car {
private:
    std::string model;
    int year_of_fabrication;
    int price;
    bool is_bought;

public:


    Car();
    Car(const std::string& model, int year, int price, bool is_bought = false);

    
    std::string get_model() const;
    int get_year_of_fabrication() const;
    int get_price() const;
    bool get_is_bought() const;
    bool is_car_valid() const;

   
    void set_model(const std::string& new_model);
    void set_year_of_fabrication(int new_year);
    //void set_price(int new_price);
    void set_is_bought(bool bought);
};

#endif // CAR_H
