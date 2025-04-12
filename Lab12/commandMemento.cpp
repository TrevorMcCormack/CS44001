#include "commandMemento.hpp"

int main() {
    DocumentWithHistory doc({
            "Lorem Ipsum is simply dummy text of the printing and typesetting",
            "industry. Lorem Ipsum has been the industry's standard dummy text",
            "ever since the 1500s, when an unknown printer took a galley of",
            "type and scrambled it to make a type specimen book. It has",
            "survived five centuries."});

    char option;
   
    do {
        static Memento *safeState = nullptr;

        doc.show();
        std::cout << std::endl;

        std::cout << "Enter option (i)nsert line, (e)rase line, (u)ndo, (c)heckpoint, roll(b)ack, (h)istory, (r)edo: ";
        std::cin >> option;

        int line; 
        std::string str; 
        switch(option) {
        case 'i':
            std::cout << "line number to insert: ";
            std::cin >> line;
            std::cout << "line to insert: ";
            std::cin.get();
            getline(std::cin, str);
            doc.insert(line, str);
            break;
        case 'e':
            std::cout << "line number to remove: ";
            std::cin >> line;
            doc.erase(line);
            break;
        case 'u':
            doc.undo();
            break;
        case 'c':
            safeState = doc.checkpoint();
            break;
        case 'b':
            if (safeState != nullptr){
                doc.rollback(safeState);
                delete safeState;
                safeState = nullptr;
            }
            else {
                std::cout << "No checkpoint created" << std::endl;
            }
            break;
        case 'h':
            doc.history();
            break;

        case 'r':
            int number;
            std::cout << "command to redo: ";
            std::cin >> number;
            doc.redo(number);
            break;
        }
    }
    while(option == 'i' || option == 'e' || option == 'u' || option == 'c' || option == 'r' || option == 'h' || option == 'b');
}