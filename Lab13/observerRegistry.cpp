#include <set>
#include <map>
#include <iostream>
#include <string>
#include <algorithm>

class Observer;
class Subject;

class EventRegistry{
public:
    static void registerObserver(std::string, Observer*);
    static void deregisterObserver(std::string, Observer*);
    static void handleMessage(std::string);
private:
    static std::map<std::string, std::set<Observer*>> observerMap_;
};

// initialize the static map
std::map<std::string, std::set<Observer*>> EventRegistry::observerMap_;


class Observer{
public:
    Observer(const std::string &name) : name_(name) {}
    std::string getName() const { return name_; }
    void subscribe(std::string s) { EventRegistry::registerObserver(s, this); } 
    void unsubscribe(std::string s){ EventRegistry::deregisterObserver(s, this); }
    void handleMessage(std::string);
private:
    std::string name_;
};

class Subject{
public:
    Subject(const std::string &name) : name_(name) {}
    std::string getName() const { return name_; }
    void generateMessage() {
        std::cout << name_ << " sent a message" << std::endl;
        EventRegistry::handleMessage(name_);
    }
private:
    std::string name_;
};

void EventRegistry::registerObserver(std::string s, Observer* o){
    observerMap_[s].insert(o);
    std::cout << o -> getName() << " subscribed to " << s << std::endl;
}

void EventRegistry::deregisterObserver(std::string s, Observer* o){
    observerMap_[s].erase(o);
    std::cout << o -> getName() << " unsubscribed from " << s << std::endl;
}

void EventRegistry::handleMessage(std::string s){
    for (auto e : observerMap_[s]) {
        e -> handleMessage(s);
    }
}

void Observer::handleMessage(std::string s) {
    std::cout << name_ << " received message from " << s << std::endl;
}


int main() {
    Subject  sue("Subject Sue"),  steve("Subject Steve");
    Observer olaf("Observer Olaf"), olga("Observer Olga");

    olaf.subscribe(sue.getName()); 
    olaf.subscribe(steve.getName());

    olga.subscribe(sue.getName());

    std::cout << std::endl;

    sue.generateMessage();
    steve.generateMessage();
    std::cout << std::endl;
    
    olga.unsubscribe(sue.getName());
    sue.generateMessage();   
}