#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <deque>
#include <string>
#include <algorithm>
#include <functional>
#include <numeric>

struct Peaches{
    double weight; // oz
    bool ripe;  // ripe or not
    void print() const { std::cout << (ripe ? "ripe" : "green") << ", " <<  weight << std::endl; }
};


Peaches getPeaches(double minWeight, double maxWeight) {
    Peaches peach;
    peach.weight = minWeight + static_cast<double>(rand()) / RAND_MAX * (maxWeight - minWeight); 
    peach.ripe = rand() % 2 == 1; // assign true or false
    return peach;
}


int main(){
    srand(time(nullptr));
    const double minWeight = 3.;
    const double maxWeight = 8.;

    std::cout << "Input basket size: ";
    int size;
    std::cin >> size;

    std::vector <Peaches> basket(size);

    // assign random weight and ripeness peaches in the basket
    // replace with generate()
    // use getPeaches function defined outside of main to generate the random ripe and weight
    generate(basket.begin(), basket.end(), 
             [minWeight, maxWeight](){ return getPeaches(minWeight, maxWeight); });
    

   
    // for_each() possibly
    std::cout << "all peaches"<< std::endl;
    auto allPeaches = for_each(basket.begin(), basket.end(), 
             [](Peaches i){ std::cout << (i.ripe ? "ripe" : "green") << ", " << i.weight << std::endl; });


    // moving all the ripe peaches from basket to peck
    // remove_copy_if() with back_inserter()/front_inserter() or equivalents
    std::deque<Peaches> peck;

    // get each peach that is ripe then use back_inserter to insert the ripe peach into the deque
    // this makes the peck hold only ripe peaches
    auto nonRipePeaches = remove_copy_if(basket.begin(), basket.end(), back_inserter(peck),
                                           [](Peaches& peach){ return !peach.ripe; });

    // need to remove the ripe peaches from the vector so it is only nonripe peaches
    // remove_if uses lambda to check if the peach is ripe
    // erase all the ripe peaches 
    basket.erase(remove_if(basket.begin(), basket.end(), 
                           [](Peaches& peach) { return peach.ripe; }), 
                 basket.end());

    // for_each() possibly
    // prints all the peaches that are in the basket which are only non ripe ones
    std::cout << "peaches remaning in the basket" << std::endl;
    auto remainingPeaches = for_each(basket.begin(), basket.end(),
                                    [](Peaches i){ std::cout << (i.ripe ? "ripe" : "green") << ", " 
                                                   << i.weight << std::endl; });

    std::cout << std::endl;

    // for_each() possibly
    std::cout << "peaches moved to the peck"<< std::endl;
    auto peckPeaches = for_each(peck.begin(), peck.end(), 
                                [](Peaches i) { std::cout << (i.ripe ? "ripe" : "green") << ", " 
                                                << i.weight << std::endl; });


    // prints every 3rd peach in the peck
    const int space = 3; 
    std::cout << "\nevery " << space << "\'rd peach in the peck"<< std::endl;

    // replace with advance()/next()/distance()
    // no explicit iterator arithmetic
    auto it = peck.begin(); 
    // start at the 3rd element 
    advance(it, space - 1);

    // get the number of peaches
    int numberOfPeaches = distance(peck.begin(), peck.end());

    // distance between the beginning and where the iterator is pointing
    while(distance(peck.begin(), it) < numberOfPeaches){
        it -> print();

        // move the iterator 3 positions so it can print the next 3rd peach
        it = next(it, space);
    }
    

    // putting all small ripe peaches in a jam
    // use a binder to create a functor with configurable max weight
    // accumulate() or count_if() then remove_if()
    
    
    const double weightToJam = 10.0;
    double jamWeight = 0.0; 
    // start at the beginning to the end with initial weight of 0.0 bind is the callback of what to accumulate
    // _1 is the free parameter sum and _2 is the free parameter of a peach object
    // accumulate will invoke the callable object which is now the bound lambda which will sum the weight
    // binding the lambda makes the operation of summing it bound to the accumulate function
    jamWeight = accumulate(peck.begin(), peck.end(), jamWeight, 
                           bind([weightToJam](double sum, Peaches& peach)
                                { if(peach.weight < weightToJam) return sum + peach.weight;
                                return sum; },
                                std::placeholders::_1, std::placeholders::_2));
            

    // remove each peach from peck that is less than the weightToJam
    peck.erase(remove_if(peck.begin(), peck.end(), 
                         [weightToJam](Peaches peach) { return peach.weight < weightToJam; }),
               peck.end());

    std::cout << "Weight of jam is: " << jamWeight << std::endl;

}
