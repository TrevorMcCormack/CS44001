#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <queue>


struct Lemon {
    double quality;
    friend bool operator<(const Lemon& left, const Lemon& right) {
        return left.quality < right.quality;
    }
};

const int maxBoxSize = 30;
const int highestQuality = 10;

int main() {
    srand(time(nullptr));
    std::vector <Lemon> box(rand() % maxBoxSize + 1); // random box sizes
    std::priority_queue<Lemon> lemonQueue;

    // random quality assingment
    for (auto& lemon : box) {
        lemon.quality = static_cast<double>(rand()) / RAND_MAX * highestQuality;
        lemonQueue.push(lemon);
    }


    std::cout << "Here are the lemons (best first): ";
    while(!lemonQueue.empty()) {
        Lemon bestLemon = lemonQueue.top();
        std::cout << bestLemon.quality << ", ";
        lemonQueue.pop();
    }
    
    std::cout << std::endl;
}
