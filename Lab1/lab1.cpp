#include <iostream>
#include <fstream>
#include <string>

// function returns the index of the first difference in the line
int firstDifferenceIndex(std::string string1, std::string string2) {
    int i = 0; 
    int string1Len = string1.length(); 
    int string2Len = string2.length();
    
    // iterate over until there is a difference or i goes over the length
    while(i < string1Len && i < string2Len && string1[i] == string2[i]) {
        ++i;
    }
    // check if there are no differences then return -1 if they are the same
    if(string1Len == string2Len && i == string1Len) {
        return -1;
    }
    // else return the index of the difference
    else {
        return i;
    }
}

// function to output the filename, line number, and the ^ to show differences
void outputDifference(std::string filename1, std::string filename2, int lineNum, 
                      std::string line1, std::string line2, bool isEndOfFile) {
    // get the index of first difference 
    int differenceIndex = firstDifferenceIndex(line1, line2);

    // check if files have no differences
    if(differenceIndex == -1) {
        return; // return nothing
    }

    // converts the integer to string then gets the length of the string
    int lengthOfNumber = std::to_string(lineNum).length(); 

    // number of spaces before the carrot should be length of file name + 4 for colon and spaces 
    // then add length of the linenumber
    int numOfPreSpaces = filename1.length() + 4 + lengthOfNumber;

    // make a string that adds the difference index in number of spaces then 
    // the amount of spaces from the filename, colons, and line number
    std::string spaceString(differenceIndex + numOfPreSpaces, ' ');

    // output filename then a colon then the line number then another colon then the actual line of text
    std::cout << filename1 << ": " << lineNum << ": " << line1 << std::endl;
    std::cout << filename2 << ": " << lineNum << ": " << line2 << std::endl;
    // check if it is not the end of the file then print an end line if its not end of file
    if(!isEndOfFile) {
        std::cout << spaceString << "^" << std::endl;
    }
    
    // else it is the end of file so do not print an end line
    else {
        std::cout << spaceString << "^";
    }
}

int main(int argc, char* argv[]) {
    if(argc == 3) {
        // first file name in command prompt
        std::string filename1 = argv[1];
        // second file in command prompt
        std::string filename2 = argv[2];

        std::ifstream file1(filename1);
        std::ifstream file2(filename2);
        std::string line1;
        std::string line2;
        
        // start on line #1
        int lineNum = 1;

        while(getline(file1, line1)) {
            if(getline(file2, line2)) {
                // compare line1 and line2 and check if either file is at the end
                outputDifference(filename1, filename2, lineNum, line1, line2, file1.eof() && file2.eof());
                ++lineNum;
            }
            else {
                // compare line1 with an empty string from file2 check if file1 is at the end
                outputDifference(filename1, filename2, lineNum, line1, "", file1.eof());
                ++lineNum;
            }
        }

        // if second file has more lines than file2 then output the rest of lines from file2 as differences
        while(getline(file2, line2)) {
            // compare line2 string with an empty string from file1 and check if file2 is at the end
            outputDifference(filename1, filename2, lineNum, "", line2, file2.eof());
            ++lineNum;
        }
    }

    return 0;
}