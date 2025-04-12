#include "WordList.hpp"

int main(int argc, char* argv[]) {
    std::string fileName = argv[1];
    std::ifstream file(fileName); 

    // making sure file is open
    if(!file.is_open()) {
        std::cerr << "Cannot open file" << std::endl;
        return 0;
    }
    
    WordList w;
    std::string word = "";
    char ch;
    

    // need to store the full word into a string
    // needs to save its spot in the file
    while(file.get(ch)) {
        if(isalnum(ch)) {
            // add each character to the word
            word += ch;
        }
        // else if word is not empty then add the word since there must be space or punctuation
        // avoids adding empty words 
        else if(!word.empty()) {
            // add the word
            w.addWord(word);
            // empty the word so a new one can be stored
            word = "";     
        }
    }
    
    // add remaining word out of the loop 
    if(!word.empty()) {
        w.addWord(word);
    }
    
    // close the file
    file.close();

    // print the wordlist 
    w.print();
    return 0;
}