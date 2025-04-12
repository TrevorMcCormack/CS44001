#include "CarFactory.hpp"


class CarLot {
public:
    CarLot();
    //Car *testDriveCar(){ return car4sale_; }

    // if a car is bought, requests a new one
    Car *buyCar(){ 
        Car *bought = cars[currentIndex];
        cars[currentIndex] = factories_[rand() % factories_.size()] -> requestCar(); // replace bought car
        return bought;
    }

    Car* nextCar();
    int lotSize() const { return cars.size(); }
		     
private:
    int currentIndex;
    std::vector<Car*> cars;
    std::vector<CarFactory *> factories_;
};


CarLot::CarLot(){
    // creates 2 Ford factories and 2 Toyota factories 
    factories_.push_back(new FordFactory());   
    factories_.push_back(new ToyotaFactory());
    factories_.push_back(new FordFactory());
    factories_.push_back(new ToyotaFactory());

    // gets the first car for sale
    for(int i = 0; i < 8; ++i) {
        cars.push_back(factories_[rand() % factories_.size()] -> requestCar()); // push back 8 random cars
    }
}


Car* CarLot::nextCar() {
    Car* currentCar = cars[currentIndex];
    currentIndex = (currentIndex + 1) % cars.size(); // move to the next car or back to 0 if last car
    return currentCar;
}


CarLot *carLotPtr = nullptr; // global pointer instantiation

// test-drives a car
// buys it if Toyota
void toyotaLover(int id){
    if (carLotPtr == nullptr)
        carLotPtr = new CarLot();

    ToyotaFactory* toyota = new ToyotaFactory();
    Car* desiredCar = toyota -> requestCar();

    bool found = false;
    int attempts = 0;
    while(attempts < carLotPtr -> lotSize() && !found) {
        Car *toBuy = carLotPtr -> nextCar(); 
        std::cout << "Jill Toyoter " << id << std::endl;
        std::cout << "test driving " << toBuy->getMake() << " " << toBuy->getModel();

        if ((toBuy -> getMake() == "Toyota") && (toBuy -> getModel() == desiredCar -> getModel())){
            std::cout << " love it! buying it!" << std::endl;
            carLotPtr -> buyCar();
            found = true;
        } 
        else {
            std::cout << " did not like it!" << std::endl;
        }
        ++attempts;
    }
}

// test-drives a car
// buys it if Ford
void fordLover(int id){
    if (carLotPtr == nullptr)
        carLotPtr = new CarLot();

    FordFactory* ford = new FordFactory();
    Car* desiredCar = ford -> requestCar();

    int attempts = 0;
    bool found = false;

    while(attempts < carLotPtr -> lotSize() && !found) {
        Car *toBuy = carLotPtr -> nextCar();
    
        std::cout << "Jack Fordman " << id << std::endl;
        std::cout << " test driving " << toBuy -> getMake() << " " << toBuy -> getModel();
    
        if ((toBuy -> getMake() == "Ford") && (toBuy -> getModel() == desiredCar -> getModel())){
            std::cout << " love it! buying it!" << std::endl;
            carLotPtr -> buyCar();
            found = true;
        } 
        else {
            std::cout << " did not like it!" << std::endl;
        }
        ++attempts;
    }
}



int main() {
    srand(time(nullptr));

    const int numBuyers = 20;
    for(int i = 0; i < numBuyers; ++i)

        if(rand()% 2 == 0)
	        toyotaLover(i);
        else
	        fordLover(i);
}