#include "drink.hpp"
#include "barista.hpp"


int main() {
    srand(time(nullptr));

    JuniorBarista *barista = new JuniorBarista(new SeniorBarista(new Manager));
    int customers = 0;

    while(customers < 3 || !(barista -> finished())) {
        int ready = rand() % 2;

        if(ready == 0 && customers < 3) {
            char choice;
            std::list<Drink *> drinks;
            std::string name;

            barista -> makeOrder(choice, drinks);

            std::cout << "Can I get your name? ";
            std::cin >> name;

            Customer *customer = new Customer(name, barista, drinks.back());
            barista -> registerCustomer(customer);   
            std::cout << "Please wait, " << name << std::endl << std::endl;
            ++customers;
        } 
        else if (ready == 1 && !(barista -> finished())) {
            barista -> finishOrder();
        }
    }
    
    return 0;
}