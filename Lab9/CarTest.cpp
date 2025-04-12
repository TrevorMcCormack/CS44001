#include <iostream>
#include <vector>
#include <cstdlib>
#include <numeric>
#include <algorithm>
#include "CarFactory.hpp"


using std::vector;
using std::cout; using std::endl;

/*
CarFactory *getLeastBusyFactory(const vector<CarFactory*> &inFactories) {

   if (inFactories.size() == 0) return nullptr;

   auto bestSoFar = inFactories[0];

   for (size_t i = 1; i != inFactories.size(); ++i) {
      if (inFactories[i]->getNumCarsInProduction() <
	       bestSoFar->getNumCarsInProduction())
	 bestSoFar = inFactories[i];
   }
   return bestSoFar;
}
*/


// same function with an STL algorithm
CarFactory *getLeastBusyFactory(const vector<CarFactory*> &inFactories) {

   if (inFactories.size() == 0) return nullptr;

   return accumulate(inFactories.begin()+1, inFactories.end(), *inFactories.begin(),
	   [](CarFactory *a, CarFactory *b) {
	      return a->getNumCarsInProduction() <
		                          b->getNumCarsInProduction() ? a: b;
	    }
	   );	            
}


// client code
int main() {
   vector<CarFactory*> factories;
   
   srand(time(nullptr));


   factories.push_back(new FordFactory("Chicago, IL", rand()%5));   
   factories.push_back(new FordFactory("Dearborn, MI", rand()%5));
   factories.push_back(new FordFactory("Louisville, KY", rand()%5));
   factories.push_back(new ToyotaFactory("Lafayette, IN", rand()%5));

   for (size_t i=0; i < 10; ++i) {
      CarFactory *currentFactory = getLeastBusyFactory(factories);
      const Car *newCar = currentFactory->requestCar();
      cout << "Ordering " << newCar->getMake()  << " " << newCar->getModel()
	   << " from " << currentFactory->getLocation() 
	   << endl;
   }
}