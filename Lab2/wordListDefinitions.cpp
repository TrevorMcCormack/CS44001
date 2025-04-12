#include "WordList.hpp"

// definitions for WordOccurrence class

bool WordOccurrence::matchWord(const std::string& rhs) {
    // returns true if current objects word matches word passed in function
    return this->word_ == rhs;
}


void WordOccurrence::increment() {
    // incrememnt number of occurrences
    ++num_;
}


std::string WordOccurrence::getWord() const {
    // return word
    return word_;
}


int WordOccurrence::getNum() const {
    // return num
    return num_;
}


bool WordOccurrence::operator<(const WordOccurrence& rhs) const {
    // if number of occurrences are the same then return in alphabetical
    if(this->num_ == rhs.num_) {
        return this->word_ < rhs.word_;
    }
    // else occurrences are not the same so return in order of occurrences
    return this->num_ < rhs.num_;
}



// definitions for WordList class

WordList::WordList(const WordList& rhs) {
    size_ = rhs.size_; // assigns rhs size to size
    wordArray_ = new WordOccurrence[size_]; // allocates new array of size size_
    for(int i = 0; i < size_; ++i) { 
        wordArray_[i] = rhs.wordArray_[i]; // put rhs values into the wordArray_
    }
}


WordList::~WordList() {
    delete[] wordArray_; // deallocates wordArray 
}


void swap(WordList& word1, WordList& word2) {
    std::swap(word1.size_, word2.size_); // swaps the sizes
    std::swap(word1.wordArray_, word2.wordArray_); // swaps the wordArrays
}


WordList& WordList::operator=(WordList rhs) {
    swap(*this, rhs);
    return *this;
}


bool equal(const WordList& word1, const WordList& word2) {
    // compare 2 words to see if they are equal

    // check if the sizes are not the same which would mean the words are different
    if(word1.size_ != word2.size_) {
        return false;
    }

    // if sizes are the same then go over the entire word using getWord to get the word and 
    // then iterate over each letter and check if they are not equal
    for(int i = 0; i < word1.size_; ++i) {
        if(word1.wordArray_[i].getWord() != word2.wordArray_[i].getWord()) {
            return false;
        }
    }

    // words must be equal so return true
    return true;
}


void WordList::addWord(const std::string& newWord) {
    // add word to wordArray at the end of the array

    // check if the word is already in wordArray_ then increment number of occurences and return if it is
    for(int i = 0; i < size_; ++i) {
        if(wordArray_[i].matchWord(newWord)) {
            wordArray_[i].increment();
            return;
        }
    }

    // else the word needs to be added to wordArray_

    //int newSize_ = size_ + 1;
    // create a new array that has 1 more element to store the new word
    WordOccurrence *newWordArray = new WordOccurrence[size_ + 1];
    for(int i = 0; i < size_; ++i) {
        // copy every element from the old array to the new array
        newWordArray[i] = wordArray_[i];
    }
    if(size_ > 0) {
        delete[] wordArray_;
    }
    // still need to add the new word at position newSize_ then also set the # of occurrences to 1
    newWordArray[size_++] = WordOccurrence(newWord, 1);
    wordArray_ = newWordArray;
}


void WordList::print() {
    // should print every word and number of occurences

    // sort the words 
    std::sort(wordArray_, wordArray_ + size_);

    // loop over the wordArray and output each word and number of occurences
    for(int i = 0; i < size_; ++i) {
        WordOccurrence temp = wordArray_[i];
        std::string word = temp.getWord();
        int numOfOccurences = temp.getNum();

        // make sure that every line except the last line has an endline
        if(i < size_ - 1) {
            std::cout << word << " " << numOfOccurences << std::endl;
        }
        else {
            std::cout << word << " " << numOfOccurences;
        }
    }
}