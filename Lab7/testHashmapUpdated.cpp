#include "hashmap.hpp"
#include <iostream>
#include <string>
#include <cassert>


int main() {

    //
    // <int, int> hashmap test
    //
    hashmap<int, int> myHash;
   
    // test inserts
    std::pair<const std::pair<const int, int>*, bool> result;

    result = myHash.insert(std::make_pair(4, 40));
    assert(result.second);
    assert(result.first->first == 4);
    assert(result.first->second == 40);

    result = myHash.insert(std::make_pair(105, 1050));
    assert(result.second);
    assert(result.first->first == 105);
    assert(result.first->second == 1050);

    result = myHash.insert(std::make_pair(6, 60));
    assert(result.second);
    assert(result.first->first == 6);
    assert(result.first->second == 60);

    result = myHash.insert(std::make_pair(107, 1070));
    assert(result.second);
    assert(result.first->first == 107);
    assert(result.first->second == 1070);

    result = myHash.insert(std::make_pair(90, 900));
    assert(result.second);
    assert(result.first->first == 90);
    assert(result.first->second == 900);

    result = myHash.insert(std::make_pair(95, 950));
    assert(result.second);
    assert(result.first->first == 95);
    assert(result.first->second == 950);

    result = myHash.insert(std::make_pair(6, 70)); // insert with a duplicate key
    assert(!result.second);
    assert(result.first->first == 6);
    assert(result.first->second == 60);


    // searching map
    // x will have type hashmap<int, int>::value_type*
    auto x = myHash.find(4);
    assert(x != nullptr);
    assert(x->second == 40);

    x = myHash.find(5);
    assert(x == nullptr);


    // test deletes
    result = myHash.erase(4); // delete with next element in same bucket
    assert(result.second);
    assert(result.first->first == 105);
    assert(result.first->second == 1050);

    result = myHash.erase(107); // delete the last element in a bucket
    assert(result.second);
    assert(result.first->first == 90);
    assert(result.first->second == 900);

    result = myHash.erase(6);
    assert(result.second);
    assert(result.first->first == 90);
    assert(result.first->second == 900);

    result = myHash.erase(105);
    assert(result.second);
    assert(result.first->first == 90);
    assert(result.first->second == 900);

    result = myHash.erase(95); // erase the last element in the container
    assert(result.second);
    assert(result.first== nullptr);

    result = myHash.erase(90); // erased all elements in container
    assert(result.second);
    assert(result.first == nullptr);

    result = myHash.erase(5); // erase key not in container
    assert(!result.second);
    // first is unspecified


    //
    // <integer, string> hashmap test
    //
    hashmap<int, std::string> employees;

    std::pair<typename hashmap<const int, std::string>::Element*, bool> result1;

   
    // add tests for insert using the <integer, string> hashmap

    result1 = employees.insert(std::make_pair(1, "john"));
    assert(result1.second);
    assert(result1.first->first == 1);
    assert(result1.first->second == "john");

    result1 = employees.insert(std::make_pair(2, "bob"));
    assert(result1.second);
    assert(result1.first->first == 2);
    assert(result1.first->second == "bob");

    result1 = employees.insert(std::make_pair(3, "mikhail"));
    assert(result1.second);
    assert(result1.first->first == 3);
    assert(result1.first->second == "mikhail");

    result1 = employees.insert(std::make_pair(4, "joe"));
    assert(result1.second);
    assert(result1.first->first == 4);
    assert(result1.first->second == "joe");

    result1 = employees.insert(std::make_pair(5, "bill"));
    assert(result1.second);
    assert(result1.first->first == 5);
    assert(result1.first->second == "bill");

    result1 = employees.insert(std::make_pair(6, "austin"));
    assert(result1.second);
    assert(result1.first->first == 6);
    assert(result1.first->second == "austin");

    result1 = employees.insert(std::make_pair(1, "william")); // insert with duplicate key
    assert(!result1.second);
    assert(result1.first->first == 1);
    assert(result1.first->second == "john");


    // add tests for find using the <integer, string> hashmap

    auto y = employees.find(1);
    assert(y != nullptr);
    assert(y->second == "john");

    y = employees.find(2);
    assert(y != nullptr);
    assert(y->second == "bob");

    y = employees.find(3);
    assert(y != nullptr);
    assert(y->second == "mikhail");

    y = employees.find(4);
    assert(y != nullptr);
    assert(y->second == "joe");

    y = employees.find(5);
    assert(y != nullptr);
    assert(y->second == "bill"); 

    y = employees.find(6);
    assert(y != nullptr);
    assert(y->second == "austin");

    y = employees.find(7);
    assert(y == nullptr);

    // add tests for erase using the <integer, string> hashmap

    result1 = employees.erase(1); 
    assert(result1.second);
    assert(result1.first->first == 2);
    assert(result1.first->second == "bob");

    result1 = employees.erase(2); 
    assert(result1.second);
    assert(result1.first->first == 3);
    assert(result1.first->second == "mikhail");

    result1 = employees.erase(3); 
    assert(result1.second);
    assert(result1.first->first == 4);
    assert(result1.first->second == "joe");

    // add tests for [] operator using the <integer, string> hashmap

    assert(employees[4] == "joe");
    assert(employees[5] == "bill");
    assert(employees[6] == "austin");

    // add tests for rehash

    assert(employees.size() == 3); // 3 left since erased the other 3
    employees.rehash(102);
    assert(employees.getNumBuckets() == 102); // numBuckets should change
    assert(employees.size() == 3); // size doesnt change

    employees.rehash(110);
    assert(employees.getNumBuckets()  == 110); // numBuckets should change
    assert(employees.size() == 3); // size doesnt change

    employees.rehash(150);
    assert(employees.getNumBuckets()  == 150); // numBuckets should change
    assert(employees.size() == 3); // size doesnt change

    // check the hashmap is still correct and all operations still work after rehash

// check find works
    y = employees.find(4);
    assert(y != nullptr);
    assert(y->second == "joe");

// check find works
    y = employees.find(5);
    assert(y != nullptr);
    assert(y->second == "bill");

// check find works
    y = employees.find(6);
    assert(y != nullptr);
    assert(y->second == "austin");

// check insert
    result1 = employees.insert(std::make_pair(1, "william")); // insert with duplicate key
    assert(result1.second);
    assert(result1.first->first == 1);
    assert(result1.first->second == "william");

// make sure erase works
    result1 = employees.erase(1);
    assert(result1.second);
    assert(result1.first->first == 4);
    assert(result1.first->second == "joe");

// make sure erase works
    result1 = employees.erase(4);
    assert(result1.second);
    assert(result1.first->first == 5);
    assert(result1.first->second == "bill");

// check []
    assert(employees[5] == "bill");
    assert(employees[6] == "austin");

    std::cout << "Tests passed" << std::endl;
}
