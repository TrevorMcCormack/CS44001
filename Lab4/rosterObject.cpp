#include <fstream>
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <cstdlib>
#include <algorithm>

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
    
    // need to change to print coursenames too
    std::string print() const;

    // function to check if name is already in the list
    bool nameExists(const Student& student) {
        return firstName_ == student.firstName_ && lastName_ == student.lastName_;
    }  

    //void addCourse(const std::list<std::string> courseName);
    void addCourse(const std::string& courseName);

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
    std::list<std::string> courseNames_; // create a list of strings to contain all courses for a student
    std::string firstName_;
    std::string lastName_;
};

// reading a list from a fileName
void readRoster(std::list<Student>& roster, std::string fileName, std::string courseName);  
// printing a list out
void printRoster(const std::list<Student>& roster); 



int main(int argc, char* argv[]){

    if (argc <= 1) { 
        std::cout << "usage: " << argv[0] << " list of courses, dropouts last" << std::endl; 
        exit(1);
    }

    std::cout << "all students, dropouts removed and sorted" << std::endl;
    std::cout << "first name last name: courses enrolled" << std::endl;

    // vector of courses of students
    std::vector <std::list<Student>> courseStudents; 

    for(int i=1; i < argc-1; ++i){
        std::list<Student> roster;
        // get the .txt file 
        std::string fileName = argv[i];

        // get the course name which is before the .txt extension
        std::string courseName = fileName.substr(0, fileName.find(".txt"));

        readRoster(roster, argv[i], courseName);  
        courseStudents.push_back(std::move(roster)); 
    }


    // reading in dropouts
    std::list<Student> dropouts; 
    readRoster(dropouts, argv[argc-1], "dropout"); 

    std::list<Student> allStudents;  // master list of students


   // Merge all student rosters into the master list
    for (auto& roster : courseStudents) {
        // loop over the roster
        for (auto& student : roster) {
            // check if the student is already in allStudents\
            // find returns an iterator to the student if it is found else it'll return end()
            auto existingStudent = std::find(allStudents.begin(), allStudents.end(), student);

            // if student already exists then existing student will be an iterator thats pointing to end()
            if (existingStudent != allStudents.end()) {
                // so add the course to the existing student
                // find the substring from print after the colon which would be course names
                // find the colon then 1 index past to avoid the colon and just get course name
                // then call addCourse on the existing student to add the new course to them
                existingStudent -> addCourse(student.print().substr(student.print().find(":") + 1));
            } 
            else {
                // student does not exist so add the student
                allStudents.push_back(student);
            }
        }
    }

    allStudents.sort(); // sorting master list
    allStudents.unique(); // eliminating duplicates
   
    for (const auto& str : dropouts) { // removing individual dropouts
        allStudents.remove(str);
    }

    printRoster(allStudents);
}

/*------------------------------------------------*/


// Definitions for Student class and other functions

std::string Student::print() const { 
    std::string student = firstName_ + " " + lastName_ + ":";

    // loop over each course name using an iterator
    for(auto it = courseNames_.begin(); it != courseNames_.end(); ++it) {
        // add each course name to the student
        student += *it;

        // if the next iterator position is not the end
        if(std::next(it) != courseNames_.end()) {
            // add a space on every course except the last course. avoids double spaces
            student += " ";
        } 
    }
    // return student which is name then all courses
    return student;
}


// function to add a course to the list
//void Student::addCourse(const std::list<std::string> courses) {
void Student::addCourse(const std::string& courseName) {
        // tries to find the course name in the list
        // find will return an iterator to the found course name or end() if it is not found
        // so only push_back the course if it is not already found in the list
    if(std::find(courseNames_.begin(), courseNames_.end(), courseName) == courseNames_.end()) {
        courseNames_.push_back(courseName);
    }
}


void readRoster(std::list<Student>& roster, std::string fileName, std::string courseName){
    std::ifstream course(fileName);
    std::string first, last;
    while(course >> first >> last) {
        Student student(first, last);
        student.addCourse(courseName);
        roster.push_back(student);
    }
    course.close();
}

// printing a list out
void printRoster(const std::list<Student>& roster){
    for(const auto& student : roster)
        std::cout << student.print() << std::endl;
}