#include "log.hpp"
#include <iostream>
#include <fstream>
#include <string>


void appendWord1() {
    std::string s1;
    std::cout << "Enter a word to append: ";
    std::cin >> s1;
    Logger::getInstance().report(s1);
}


void appendWord2() {
    std::string s2;
    std::cout << "Enter a second word to append: ";
    std::cin >> s2;
    Logger::getInstance().report(s2);
}


int main() {
    appendWord1();
    appendWord2();
}