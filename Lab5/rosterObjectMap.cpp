#include <fstream>
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <map>

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

// reading a map from a fileName
void readRoster(std::map<Student, std::list<std::string>>& allStudents, std::string fileName);  
// printing a map out
void printRoster(const std::map<Student, std::list<std::string>>& allStudents); 



int main(int argc, char* argv[]) {
    std::map<Student, std::list<std::string>> allStudents;

    if (argc <= 1) { 
        std::cout << "usage: " << argv[0] << " list of courses, dropouts last" << std::endl; 
        exit(1);
    }

    std::cout << "all students, dropouts removed and sorted" << std::endl;
    std::cout << "first name last name: courses enrolled" << std::endl;

    for(int i = 1; i < argc - 1; ++i) {
        std::string fileName = argv[i];
        readRoster(allStudents, fileName);
    }


    // reading in dropouts
    std::map<Student, std::list<std::string>> dropouts; 
    readRoster(dropouts, argv[argc-1]); 

    // loop over each student in the dropout 
    for(const auto& dropout : dropouts) {
        // erase each student from the map where the name is in the dropout list accessed by .first
        allStudents.erase(dropout.first);
    }

    printRoster(allStudents);
}


// take in a map of students(first and last name) and a list of strings which will be course names 
void readRoster(std::map<Student, std::list<std::string>>& allStudents, std::string fileName){
    std::ifstream course(fileName);
    std::string courseName = fileName.substr(0, fileName.find(".txt"));
    std::string first, last;
    while(course >> first >> last) {
        Student student(first, last);
        // pushes each course name with the student(first and last name)
        allStudents[student].push_back(courseName);
    }
    course.close();
}


// printing a list out
void printRoster(const std::map<Student, std::list<std::string>>& allStudents) {
    // iterate over each student in the map
    for(const auto& student : allStudents) {
        // .first will get the students name then .print prints the first and last name
        std::cout << student.first.print() << ":";
        // loop over each course in the list which is in student.second
        for(const auto& courseName : student.second) {

            std::cout << courseName << " ";
        }
        std::cout << std::endl; // print endl after each student is done
    }
}