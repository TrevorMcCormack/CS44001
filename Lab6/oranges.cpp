#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <string>
#include <map>

enum class Variety {orange, pear, apple};
std::vector<std::string> colors = {"red", "green", "yellow"};

struct Fruit{
    std::string color; // red, green or orange
};


int main(){
    srand(time(nullptr));
    std::multimap<Variety, Fruit> tree;
    int size = rand() % 100 + 1; 

    // random fruit and color selection
    for(int i = 0; i < size; ++i){
        Variety v = static_cast<Variety>(rand() % 3); // get the random fruit
        std::string color = colors[rand() % 3]; // get the random color
        Fruit fruit{ color };   // make a fruit object that has the random color
        tree.emplace(v, fruit);
    }


    // use upper and lower bound for the range rather than begin and end
    auto lowerBound = tree.lower_bound(Variety::orange); // first element that is an orange
    auto upperBound = tree.upper_bound(Variety::orange); // first element greater than orange

    // printing colors of oranges
    std::cout << "Colors of the oranges: ";

    // iterate over the multimap
    for(auto it = lowerBound; it != upperBound; ++it) {
        // if the first element of the pair which is the variety == orange
        if(it -> first == Variety::orange) {
            // print out the second element of the pair which is a fruit so use .color to access the color
            std::cout << it -> second.color << ", ";
        }
    }
    std::cout << std::endl;
}
