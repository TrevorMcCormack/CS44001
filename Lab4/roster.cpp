#include <fstream>
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <cstdlib>


// reading a list from a fileName
void readRoster(std::list<std::string>& roster, std::string fileName, std::string courseName);  

// printing a list out
void printRoster(const std::list<std::string>& roster); 

int main(int argc, char* argv[]){

    if (argc <= 1){ 
        std::cout << "usage: " << argv[0] 
        << " list of courses, dropouts last" 
        << std::endl;
        exit(1);
    }

    std::cout << "all students, dropouts removed and sorted" << std::endl;
    std::cout << "first name last name: courses enrolled" << std::endl;

    // vector of courses of students
    std::vector <std::list<std::string>> courseStudents; 
    // vector of lists where each list contains strings

    // need to add the first name last name to the string in the list then also add the course names they are in
    // each vector entry should be a list that stores 

    for(int i=1; i < argc-1; ++i){
        std::list<std::string> roster;

        // get the .txt file 
        std::string fileName = argv[i];

        // get the course name which is before the .txt extension
        std::string courseName = fileName.substr(0, fileName.find(".txt"));
        readRoster(roster, argv[i], courseName);  
        courseStudents.push_back(move(roster));
   }

    // reading in dropouts
    std::list<std::string> dropouts; 
    readRoster(dropouts, argv[argc-1], "dropout"); 

    // master list of students
    std::list<std::string> allStudents;  

    // loop over each roster in courseStudents
    for(auto& roster : courseStudents) {
        // loop over each student in each roster
        for(const auto& student : roster) {
            // get the student name which is before the first :
            std::string studentName = student.substr(0, student.find(":"));

            // bool to check if student is found or not
            bool studentFound = false;

            // bool to check if the student is a dropout
            bool isDropout = false;

            // loop over each dropout
            for (const auto& str : dropouts) {
                // get the name of the dropout
                std::string dropout = str.substr(0, str.find(':'));
                if(studentName == dropout) {
                    isDropout = true;
                }
            }

            if(isDropout) continue; // skip adding the student if they are a dropout

            // loop over the allstudents list to check if student is already in there
            for(auto& existingStudent : allStudents) {
                std::string existingName = existingStudent.substr(0, existingStudent.find(":"));

                // check if student name already exists
                if(studentName == existingName) {
                    // if they are equal then add the name of the course and set found to be true
                    std::string courseName = student.substr(student.find(':') + 1); // +1 to go past the colon because do not want another colon
                    existingStudent += courseName;
                    studentFound = true; 
                    break;
                } 
            }
            // if a student is not found already then add them to the list
            if(!studentFound) {
                allStudents.push_back(student);
            }
        }
    }
    // sorting master list
    allStudents.sort();
    allStudents.unique();
    printRoster(allStudents);   
}

// reading in a file of names into a list of strings
void readRoster(std::list<std::string>& roster, std::string fileName, std::string courseName){
    std::ifstream course(fileName);
    std::string first, last;
    while(course >> first >> last)
        roster.push_back(first + ' ' + last + ':' + courseName + ' ');
    course.close();
}

// printing a list out
void printRoster(const std::list<std::string>& roster){
    for(const auto& str : roster)
        std::cout << str << std::endl;
}