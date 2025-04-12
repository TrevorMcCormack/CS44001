#ifndef BARISTA_HPP
#define BARISTA_HPP

#include "drink.hpp"

class Customer;

class Barista {
public:
    Barista(Barista* next = nullptr) : next_(next) {}
    virtual Drink* makeOrder(char choice, std::list<Drink*>& drinks) { 
        return next_ -> makeOrder(choice, drinks); 
    }
    char takeOrder();
    std::string customerName();

    void registerCustomer(Customer* customer) { customers.push_back(customer); }
    void deregisterCustomer(int index) { customers.erase(customers.begin() + index); }
    void notifyCustomer() const;
    bool finished() { return customers.empty(); }
    void finishOrder();

protected:
    std::vector<Customer*> customers;
    int customerIndex;
private:
    Barista* next_;
};


class Customer {
public: 
    Customer(const std::string& name, Barista* barista, Drink* drink) :
    name_(name), barista_(barista), drink_(drink)  { }

    std::string getName() const { return name_; }
    Drink* getDrink() const { return drink_; } 

    void notifyCustomer() const;
private:
    std::string name_;
    Barista* barista_;
    Drink* drink_;
};


class JuniorBarista : public Barista {
public:
    JuniorBarista(Barista* next = nullptr) : Barista(next) { }
    Drink* makeOrder(char choice, std::list<Drink*>& drinks) override;
};


class SeniorBarista : public Barista {
public:
    SeniorBarista(Barista* next = nullptr) : Barista(next) { }
    Drink* makeOrder(char choice, std::list<Drink*>& drinks) override;
};


class Manager : public Barista {
public:
    Manager(Barista* next = nullptr) : Barista(next) { }
    Drink* makeOrder(char choice, std::list<Drink*>& drinks) override;
};


/*
-------------------------------------------------------------------------------------------------------------
Function Definitions
-------------------------------------------------------------------------------------------------------------
*/

std::string Barista::customerName() {
    return customers[customerIndex] -> getName();
}


char Barista::takeOrder() {
    char ingredient;
    std::cout << "Would you like to add [s]ugar, [c]ream, [h]oney, or [d]one? ";
    std::cin >> ingredient;
    return tolower(ingredient);
}


Drink* JuniorBarista::makeOrder(char choice, std::list<Drink*>& drinks) {
    std::cout << "Welcome to Coffee Shack, can I get you [l]arge, [m]edium, [s]mall? ";
    char size;
    std::cin >> size;
    int price = 0;
    DrinkType type;

    if(tolower(size) == 'l') {
        type = DrinkType::large;
        price = 10;
    }
    else if(tolower(size) == 'm') {
        type = DrinkType::medium;
        price = 7;
    }
    else { 
        type = DrinkType::small; // default is small
        price = 5;
    }
    drinks.push_back(new Drink(type, price)); 
    choice = this -> takeOrder();

    if(choice == 'd') { // junior barista can only make with no ingredients
        std::cout << "Junior Barista making drink" << std::endl;
        return drinks.back();
    }

    return Barista::makeOrder(choice, drinks);
}


Drink* SeniorBarista::makeOrder(char choice, std::list<Drink*>& drinks) {
    while(choice != 'h' && choice != 'd') {
        if(choice == 's') {
            drinks.push_back(new Sugar(drinks.back()));
        }
        else if(choice == 'c') {
            drinks.push_back(new Cream(drinks.back()));
        }

        choice = this -> takeOrder();
    }
    if(choice == 'd') {
        std::cout << "Senior Barista making drink" << std::endl;
        return drinks.back();
    }
    return Barista::makeOrder(choice, drinks);
}


Drink* Manager::makeOrder(char choice, std::list<Drink*>& drinks) {
    while(choice != 'd') {
        if(choice == 's') {
            drinks.push_back(new Sugar(drinks.back()));
        }
        else if(choice == 'c') {
            drinks.push_back(new Cream(drinks.back()));
        }
        else if (choice == 'h') {
            drinks.push_back(new Honey(drinks.back()));
        }
        choice = this -> takeOrder();
    }
    std::cout << "Manager making drink" << std::endl;
    return drinks.back();
}


void Barista::notifyCustomer() const {
    for(auto customer : customers) {
        customer -> notifyCustomer();
    }
}


void Customer::notifyCustomer() const {
    std::cout << name_ << ", your " << drink_ -> getIngredients() << "is ready. It will be $"
              << drink_ -> getPrice() << ", please." << std::endl;
    std::cout << "This is " << name_ << ", I got my coffee, thank you!" << std::endl << std::endl;
}


void Barista::finishOrder() {
    if(!customers.empty()) {
        customerIndex = rand() % customers.size();
        Customer* customer = customers[customerIndex];
        customer -> notifyCustomer();
        deregisterCustomer(customerIndex);  
    }
} 

#endif
