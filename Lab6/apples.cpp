#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <deque>
#include <string>
#include <algorithm>
#include <numeric>
#include <functional>

struct Apples{
    double weight; // oz
    std::string color;  // red or green
    void print() const { std::cout << color << ", " <<  weight << std::endl; }
};

Apples getApples(double minWeight, double maxWeight) {
    Apples apple; 
    apple.weight = minWeight + static_cast<double>(rand()) / RAND_MAX * (maxWeight - minWeight);
    apple.color = rand() % 2 == 1 ? "green" : "red";
    return apple;
}



int main() {
    srand(time(nullptr));
    const double minWeight = 3.;
    const double maxWeight = 8.;

    std::cout << "Input crate size: ";
    int size;
    std::cin >> size;

    std::vector <Apples> crate(size);

    // assign random weight and color to apples in the crate
    // replace with generate()
    // getApples is a function defined outside of main that takes in 2 doubles and gets a random weight and color
    // then returns this apple object with the random weight and color
    // lambda that captures both doubles and passes them to getApples which will return an apple
    generate(crate.begin(), crate.end(), [minWeight, maxWeight]() 
                                         { return getApples(minWeight, maxWeight); });


    // for_each() 
    std::cout << "all apples"<< std::endl;
    auto allApples = for_each(crate.begin(), crate.end(), 
                     [](Apples i) { std::cout << i.color << "," << i.weight << std::endl; });
   

    std::cout << "Enter weight to find: ";
    double toFind;
    std::cin >> toFind;


    // count_if()
    // capture toFind to use to check if heavierWeight is > to it then increment the count if it is
    int cnt = count_if(crate.begin(), crate.end(), 
                       [toFind](Apples heavierWeight) { return heavierWeight.weight > toFind; });
    std::cout << "There are " << cnt << " apples heavier than "  << toFind << " oz" <<  std::endl;


    // find_if()
    std::cout << "at positions ";
    const auto heavierWeight = [=](Apples i) { return (i.weight) > toFind;}; 
    auto weightPositions = find_if(crate.begin(), crate.end(), heavierWeight); // find element where weight is > inputted weight
    while(weightPositions != crate.end()) { // while iterator isn't at end
        std::cout << weightPositions - crate.begin() << ", "; // iterator arithmetic to find the distance from beginning
        // increment iterator so it is past the element it was at then find next element
        weightPositions = find_if(++weightPositions, crate.end(), heavierWeight); 
    }
    std::cout << std::endl;



    // max_element()
   // return an iterator pointing to the apples with the greatest weight
   // lambda compares 2 apples weight and returns if true if the first one is less than the second one
    auto maxAppleWeight = max_element(crate.begin(), crate.end(), 
                                [](Apples& a1, Apples& a2) 
                                { return a1.weight < a2.weight; });
    // makes sure the crate isnt empty before using maxAppleWeight
    if(!crate.empty()) {
        std::cout << "Heaviest apple weighs: " << maxAppleWeight -> weight << " oz" << std::endl;
    }
    else {
        std::cout << "Crate is empty" << std::endl;
    }




    // for_each() or accumulate()
    // included in the numeric header
    // lambda takes a double and an apple then returns the double + the apple's weight
    // goes over the whole crate and does this. Initialize to 0.0 since double
    double totalWeight = accumulate(crate.begin(), crate.end(), 0.0, 
                            [](double total, Apples& apple) { return total + apple.weight; });

    std::cout << "Total apple weight is: " << totalWeight << " oz" << std::endl;


    // transform();
    // adds weight to each apple
    std::cout << "How much should they grow: ";
    double toGrow;
    std::cin >> toGrow;

// bind included in the functional header
// source range is beginning to end of crate. Target range is beginning of crate
// lambda captures toGrow then takes an apple 
// then adds the apple's weight to itself + the toGrow value
    transform(crate.begin(), crate.end(), crate.begin(), 
              [toGrow](Apples& apple) { apple.weight += toGrow; 
                                       return apple; }); // return the apple 


    // remove_if()
    std::cout << "Input minimum acceptable weight: ";
    double minAccept;
    std::cin >> minAccept;
    auto originalSize = crate.size();

    // removing small apples
    crate.erase(remove_if(crate.begin(), crate.end(), 
          [minAccept](Apples& apple){ return apple.weight < minAccept; } ), 
          crate.end());

    std::cout << "removed " << originalSize - crate.size() << " elements" << std::endl;


    // bubble sort, replace with sort()
    // lambda sorts will compare the apples weights and return true when apple1 is less than apple2
    sort(crate.begin(), crate.end(), 
         [](Apples& apple1, Apples& apple2) { return apple1.weight < apple2.weight; });
    


    // for_each() possibly
    std::cout << "sorted remaining apples" << std::endl;
    for_each(crate.begin(), crate.end(), 
             [](Apples i) { std::cout << i.color << ", " <<  i.weight << std::endl; });

}
