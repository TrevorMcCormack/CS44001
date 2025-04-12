#include "state.hpp"


int main() {
    Process p1;
    std::vector<Process*> processes;
    std::queue<Process*> readyQueue;
    std::queue<Process*> blockedQueue;
    bool empty = false; 

    for(int i = 0; i < 4; ++i) {
        processes.push_back(p1.clone()); // push back a clone of p1
    }

    for(auto p : processes) { // for every process in the vector
        std::cout << p -> reportState();
        p -> create();
        readyQueue.push(p); // push each process into ready queue
    }
    
    std::cout << std::endl << std::endl;
    std::cout << "---------- scheduler running -------------" << std::endl;

    while(!readyQueue.empty() || !blockedQueue.empty()) {
        if(!readyQueue.empty()) {
            int random = rand() % 3; // only 3 choices
            std::cout << "Dispatching...";
            Process* p = readyQueue.front(); // get process from ready queue
            readyQueue.pop();
            p -> dispatch();
            p -> reportState();

            if (random == 0) { // 0 is suspending
                std::cout << std::endl << "Suspending...";
                p -> suspend(); // suspend process
                readyQueue.push(p); // push onto ready queue
            }

            else if(random == 1) { // 1 is blocking
                std::cout << std::endl << "Blocking...";
                p -> block(); // block process
                blockedQueue.push(p); // push into blocked queue
            }

            else if(random == 2) { // 2 is exit
                std::cout << std::endl << "Exiting...";
                p -> exit(); // exit process
            }

            empty = false;
            std::cout << std::endl << std::endl;
        }

        else if(!blockedQueue.empty()) {
            if(!empty) {
                std::cout << "The ready queue is empty" << std::endl << std::endl;
                empty = true; // ready queue is empty
            }

            int choice = rand() % 3; // 33% chance
            
            if(choice == 0) { // only unblocked when choice is 0
                std::cout << "Unblocking...";
                Process* p = blockedQueue.front(); // get process from blocked queue
                blockedQueue.pop(); 
                p -> unblock(); // unblock process
                readyQueue.push(p); // push process back to ready queue
                empty = false; // ready queue not empty
                std::cout << std::endl << std::endl;
            }
        }
    }

    std::cout << "all processes exited." << std::endl;
    std::cout << "---------- scheduler done -------------" << std::endl;
}