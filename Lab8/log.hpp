#ifndef LOG_HPP
#define LOG_HPP

#include <iostream>
#include <fstream>
#include <string>


class Logger {
public:
    static Logger& getInstance();
    void report(const std::string);

private:
    std::ofstream logFile; 
    Logger() {  logFile.open("log.txt", std::fstream::out | std::fstream::app); } // opens file
    Logger(const Logger&);
};


Logger& Logger::getInstance() {
    static Logger instance;
    return instance;    
}


void Logger::report(const std::string s1) {
    // write message to logFile
    logFile << s1 << std::endl;
}
 

#endif
