#include <fstream>
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <set>

class Student{
public:
    Student(std::string firstName, std::string lastName): 
        firstName_(firstName), lastName_(lastName) {}
 
    // move constructor, not really needed, generated automatically
    Student(Student && org) noexcept:
        firstName_(std::move(org.firstName_)),
        lastName_(std::move(org.lastName_))
    {}
    // force generation of default copy constructor
    Student(const Student & org) = default;
    
    std::string print() const { return firstName_ + ' ' + lastName_; }

    // needed for unique() and for remove()
    friend bool operator== (Student left, Student right){
        return left.lastName_ == right.lastName_ &&
	    left.firstName_ == right.firstName_;
    }

    // needed for sort()
    friend bool operator< (Student left, Student right){
        return left.firstName_ < right.firstName_ ||
	    (left.firstName_ == right.firstName_ && 
	    left.lastName_ < right.lastName_);
    }
private:
    std::string firstName_;
    std::string lastName_;
};

// reading a set from a fileName
void readRoster(std::set<Student>& allStudents, std::string fileName);  
// printing a set out
void printRoster(const std::set<Student>& allStudents); 



int main(int argc, char* argv[]) {
    std::set<Student> allStudents;

    if (argc <= 1) { 
        std::cout << "usage: " << argv[0] << " list of courses, dropouts last" << std::endl; 
        exit(1);
    }

    std::cout << "Currently enrolled students" << std::endl;

    for(int i = 1; i < argc - 1; ++i) {
        std::string fileName = argv[i];
        readRoster(allStudents, fileName);
    }


    // reading in dropouts
    std::set<Student> dropouts; 
    readRoster(dropouts, argv[argc-1]); 

    // loop over each student in the dropout set
    for(const auto& dropout : dropouts) {
        // erase each student from the set that is in the dropout set
        allStudents.erase(dropout);
    }

    printRoster(allStudents);
}


// take in a set of students  
void readRoster(std::set<Student>& allStudents, std::string fileName){
    std::ifstream course(fileName);
    std::string first, last;

    while(course >> first >> last) {
        Student student(first, last);
        // inserts each student into the set
        allStudents.insert(student);
    }
    course.close();
}


// printing a list out
void printRoster(const std::set<Student>& allStudents) {
    // iterate over each student in the set
    for(const auto& student : allStudents) {
        // prints each student's name from the set
        std::cout << student.print() << std::endl;
    }
}