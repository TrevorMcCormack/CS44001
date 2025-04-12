#ifndef WORDLIST_HPP
#define WORDLIST_HPP

#include <string>
#include <utility>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <cctype>

// stores the word and number of occurences 
class WordOccurrence {
public:
    WordOccurrence(const std::string& word="", int num=0) : word_(word), num_(num) {} // default constructor 
    bool matchWord(const std::string& rhs); // returns true if word matches stored
    void increment(); // increments number of occurrences
    std::string getWord() const; // returns word
    int getNum() const; // returns the number of occurences
    bool operator<(const WordOccurrence& rhs) const;

private:
    std::string word_;
    int num_;
};

class WordList{
public:
    WordList() : size_(0), wordArray_(nullptr) {}  // default contructor
    
    // add copy constructor, destructor, overloaded assignment
    WordList(const WordList&); // copy constructor
    ~WordList(); // destructor
    WordList& operator=(WordList); // overloaded assignment

    // implement comparison as friend
    friend bool equal(const WordList&, const WordList&);
    friend void swap(WordList&, WordList&);

    void addWord(const std::string&);
    void print();

    // Only use for testing purposes
    WordOccurrence * getWords() const {return wordArray_;};
    int getSize() const {return size_;};
private:
    WordOccurrence *wordArray_; // a dynamically allocated array of WordOccurrences
                                // may or may not be sorted
    int size_;
};

#endif
