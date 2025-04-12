// drink class to be used in Coffee Shack lab
// Mikhail Nesterenko
// 11/7/2022
#ifndef DRINK_HPP
#define DRINK_HPP

#include <string>
#include <iostream>
#include <list>
#include <vector>
#include <ctime>


enum class DrinkType { small, medium, large };

class Drink {
public:
    Drink(DrinkType type = DrinkType::small, int price = 0) : type_(type), price_(price){}
    virtual int getPrice() const { return price_; }
    virtual std::string getName() const { return name_; }
    virtual std::string getIngredients() const;
private: 
    int price_;
    DrinkType type_;
    std::string name_;
    std::string ingredients;
};


class Sugar : public Drink {
public:
    Sugar(Drink* drink) : drink_(drink) {}
    int getPrice() const override { return drink_ -> getPrice() + 1; }
    std::string getIngredients() const override { return drink_ -> getIngredients() + "sugar, "; }
private:
    const Drink* drink_;
};


class Cream : public Drink {
public:
    Cream(Drink* drink) : drink_(drink) {}
    int getPrice() const override { return drink_ -> getPrice() + 2; }
    std::string getIngredients() const override { return drink_ -> getIngredients() + "cream, "; }
private:
    const Drink* drink_;
};


class Honey : public Drink {
public:
    Honey(Drink* drink) : drink_(drink) {}
    int getPrice() const override { return drink_ -> getPrice() + 3; }
    std::string getIngredients() const override { return drink_ -> getIngredients() + "honey, "; }
private:
    const Drink* drink_;
};


std::string Drink::getIngredients() const {
    if(type_ == DrinkType::small) {
        return "small coffee with ";
    }
    else if(type_ == DrinkType::medium) {
        return "medium coffee with ";
    }
    else if(type_ == DrinkType::large) {
        return "large coffee with ";
    }
    else {
        return "small coffee with "; // default is small
    }
}


#endif
