#include <iostream>
#include <set>
#include <vector>
#include <string>
#include <cstdio>
#include <ctime>
#include <algorithm>

// for timed sleep
#include <thread>
#include <chrono>


// abstract mediator
class Controller{
public:
    virtual void join(class Flight*) = 0;
    virtual void leave(class Flight*) = 0;
    virtual void broadcast() = 0;
    virtual void observe() = 0;
};


// concrete mediator
// tower, runway clearance
class Tower: public Controller{
public:
    void join(class Flight*) override; 
    void leave(class Flight*) override;
    void broadcast() override;
    void observe() override;
private:
    std::set<class Flight*> waiting_;
};


class AirLine {
public:
    virtual std::string companyName() = 0;
// "AirFrance", "KLM", "Aeroflot", "Lufthansa", "Delta", "United", "Spirit" 
};

class AirFrance : public AirLine {
public: 
    std::string companyName() override { return "AirFrance"; }
};

class KLM : public AirLine {
public:
    std::string companyName() override { return "KLM"; }
};

class Aeroflot : public AirLine {
public:
    std::string companyName() override { return "Aeroflot"; }
};

class Lufthansa : public AirLine {
public:
    std::string companyName() override { return "Lufthansa"; }
};

class Delta : public AirLine {
public:
    std::string companyName() override { return "Delta"; }
};

class United : public AirLine {
public:
    std::string companyName() override { return "United"; }
};

class Spirit : public AirLine {
public:
    std::string companyName() override { return "Spirit"; }
};


// "AirFrance", "KLM", "Aeroflot", "Lufthansa", "Delta", "United", "Spirit" 
class AirLineFactory {
public:
    static AirLine* makeAirLine(std::string company) {
        if(company == "AirFrance") {
            if(!airFrance) {
                airFrance = new AirFrance;
            }
            return airFrance;
        }

        else if(company == "KLM") {
            if(!klm) {
                klm = new KLM;
            }
            return klm;
        }

        else if(company == "Aeroflot") {
            if(!aeroflot) {
                aeroflot = new Aeroflot;
            }
            return aeroflot;
        }

        else if(company == "Lufthansa") {
            if(!lufthansa) {
                lufthansa = new Lufthansa;
            }
            return lufthansa;
        }

        else if(company == "Delta") {
            if(!delta) {
                delta = new Delta;
            }
            return delta;
        }

        else if(company == "United") {
            if(!united) {
                united = new United;
            }
            return united;
        }

        else if(company == "Spirit") {
            if(!spirit) {
                spirit = new Spirit;
            }
            return spirit;
        }
        return nullptr;
    }
private:
    static AirFrance* airFrance;
    static KLM* klm;
    static Aeroflot* aeroflot;
    static Lufthansa* lufthansa;
    static Delta* delta;
    static United* united;
    static Spirit* spirit;
};

AirFrance* AirLineFactory::airFrance = nullptr;
KLM* AirLineFactory::klm = nullptr;
Aeroflot* AirLineFactory::aeroflot  = nullptr;
Lufthansa* AirLineFactory::lufthansa  = nullptr;
Delta* AirLineFactory::delta  = nullptr;
United* AirLineFactory::united  = nullptr;
Spirit* AirLineFactory::spirit  = nullptr;



// abstract colleague
class Flight {
public:
    Flight(Controller *controller) : controller_(controller), status_(Status::waiting){
        task_ = rand() % 2 ? Task::taxiing : Task::approaching;
        controller_ -> join(this);
    }


    void receive(const std::string &msg){
        if (msg.find(flightNo_) != std::string::npos || msg.find("all") != std::string::npos) {
            if(msg.find("clear") != std::string::npos){
                std::cout << flightNo_ << " roger that, ";
                std::cout << (task_ == Task::taxiing ? "taking off" : "landing") << std::endl;
                status_ = Status::cleared;
            } 
            else if(msg.find("status") != std::string::npos) {
                std::cout << flightNo_
                          << (status_ == Status::waiting ? " waiting to " : " cleared to ")   
	                      << (task_ == Task::taxiing ? "take off" : "land") << std::endl;
	        } 
            else
	            std::cout << "Tower, this is " << flightNo_ << " please repeat." << std::endl;
        }
    }


    bool isCleared() const { return status_ == Status::cleared; }


    void proceed() {
        std::this_thread::sleep_for(std::chrono::seconds(rand() % 3 + 1)); // wait a while    
        std::cout << flightNo_ << (task_ == Task::taxiing ? " took off" : " landed")
                  << ". Have a good day!" << std::endl; 
        controller_ -> leave(this);
    }

protected:
    std::string flightNo_;
    Controller *controller_;
    enum class Task {taxiing, approaching};  Task task_;
    enum class Status {waiting, cleared};    Status status_;
};

// concrete colleagues
class Airbus: public Flight{
public:
    Airbus(Tower *tower): Flight(tower) {
        static const std::vector<std::string> companies = { "AirFrance", "KLM", "Aeroflot", "Lufthansa" };

        flightNo_ = companies[rand() % companies.size()] + std::to_string(rand() % 1000);
        std::cout << flightNo_ << " requesting "
                  << (task_ == Task::taxiing ? "takeoff" : "landing") << std::endl;
    }
};

class Boeing: public Flight{
public:
    Boeing(Tower *tower): Flight(tower){
        static const std::vector<std::string> companies = { "Delta", "United", "Spirit" };

        flightNo_ = companies[rand() % companies.size()] + std::to_string(rand() % 1000);
        std::cout << flightNo_ << " requesting " << (task_ == Task::taxiing ? "takeoff" : "landing") 
                  << std::endl;
    }
};


// member functions for Tower
void Tower::broadcast() {
    std::cout << "Tower: ";
    std::string msg;
    getline(std::cin, msg);

    if(!msg.empty())
        for(auto f : waiting_) f -> receive(msg);
}


void Tower::observe() {
    auto findCleared = [](Flight *f){return f -> isCleared();};
    
    auto toProceed = std::find_if(waiting_.begin(), waiting_.end(), findCleared);
    
    while (toProceed != waiting_.end()){ // found a cleared flight
        (*toProceed) -> proceed();
        toProceed = std::find_if(waiting_.begin(), waiting_.end(), findCleared);
        if(toProceed != waiting_.end())
            std::cout << "MAYDAY! MAYDAY! MAYDAY! " << std::endl; // more than a single flight cleared
    }
}


void Tower::join(Flight *f) {
    waiting_.insert(f);
}


void Tower::leave(Flight *f) {
    waiting_.erase(f);
    delete f;
}

int main(){
    srand(time(nullptr));
    Tower jfk;

    new Boeing(&jfk);
    new Airbus(&jfk);
    new Boeing(&jfk);
    new Airbus(&jfk);

    while(true){
        jfk.broadcast();
        jfk.observe();

        if(rand() % 2){
            if (rand() % 2) {
                new Boeing(&jfk);
            }
            else {
                new Airbus(&jfk);
            }
        }
    }
}