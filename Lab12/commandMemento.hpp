// implements command history (multiple undos)
// demonstrates command pattern
// Mikhail Nesterenko
// 11/7/2024

#ifndef COMANDMEMENTO_HPP
#define COMANDMEMENTO_HPP

#include <iostream>
#include <vector>
#include <string>
#include <stack>


// receiver
class Document {
public:
    Document(const std::vector<std::string> & lines={}): lines_(lines){}

   // actions
    void insert(int line, const std::string &str) {
        const int index = line - 1;
        if (index <= lines_.size()) {
            lines_.insert(lines_.begin() + index, str);
            successful_ = true;
        } 
        else
            std::cout << "line out of range" << std::endl; 
    }

    const std::string remove(int line) {
        const int index = line - 1;
        std::string deletedLine = "";
        if(index < lines_.size()) {
            // deletedLine = lines_[index];
            deletedLine = *(lines_.begin()+index); 
            lines_.erase(lines_.begin() + index); 
            successful_ = true;
        }
        else
            std::cout << "line out of range" << std::endl; 
        return deletedLine;
    } 

    void show() const {
        for(int i = 0; i < lines_.size(); ++i) {
            std::cout << i + 1 << ". " << *(lines_.begin() + i) << std::endl; 
        }
    }

    bool getSuccessful() { return successful_; }
    void resetSuccessful() { successful_ = false; }

private:
    std::vector<std::string> lines_; 
    bool successful_ = false;
}; 

// abstract command
class Command {
public:
    Command(Document *doc) : doc_(doc){}
    virtual void execute() = 0; 
    virtual void unexecute() = 0; 
    virtual void show() = 0;
    virtual ~Command(){}
    virtual Command* clone() = 0;
protected:
    Document *doc_; 
}; 

// two concrete commands
class InsertCommand : public Command {
public:
    InsertCommand(Document *doc, int line, const std::string &str) : Command(doc), line_(line),  str_(str) {}
    void execute() override { doc_ -> insert(line_, str_); }
    void unexecute() override { doc_ -> remove(line_); }
    void show() override { std::cout << "insert \"" << str_ << "\" at line " << line_ << std::endl; }
    Command* clone() override { return new InsertCommand(doc_, line_, str_); }


private:
    int line_; 
    std::string str_; 
}; 


class EraseCommand : public Command {
public:
    EraseCommand(Document *doc, int line) : Command(doc), line_(line), str_("") {}
    void execute() override { str_ = doc_ -> remove(line_); }
    void unexecute() override { doc_ -> insert(line_, str_); }
    void show() override { std::cout << "erase line " << line_ << std::endl; }
    Command* clone() override { return new EraseCommand(doc_, line_); }
private:
    int line_;
    std::string str_;
};

// client ?
class DocumentWithHistory {
public:
    DocumentWithHistory(const std::vector<std::string> & doc={}) : doc_(doc){}

    void insert(int line, const std::string & str) {
        Command *com = new InsertCommand(&doc_, line, str); 
        com -> execute(); 
        if(doc_.getSuccessful()) {
            doneCommands_.push(com); 
            doc_.resetSuccessful();
        }
    }

    void erase(int line) {
        Command *com = new EraseCommand(&doc_, line);
        com -> execute();
        if(doc_.getSuccessful()) {
            doneCommands_.push(com); 
            doc_.resetSuccessful();
        }    
    }

    void undo() {
        if(!doneCommands_.empty()) {
            Command *com = doneCommands_.top(); 
            doneCommands_.pop(); 
            com -> unexecute();
            delete com; // don't forget to delete command
        } 
        else
            std::cout << "no commands to undo" << std::endl; 
    }

    void history() {
        int stackSize = doneCommands_.size();
        std::stack<Command*> temp = doneCommands_;
        std::stack<Command*> reverse;
        while(!temp.empty()) {
            reverse.push(temp.top());
            temp.pop();
        }

        if(stackSize != 0) {
            int i = 1;      
            while(!reverse.empty()) { 
                std::cout << i << ". ";
                reverse.top() -> show();
                reverse.pop(); 
                ++i;
            }
            std::cout << std::endl;
        }
        else {
            std::cout << "no commands" << std::endl << std::endl;
        }
    }

    void redo(int command) {
        if(command > doneCommands_.size() || command <= 0) {
            std::cout << "Invalid command" << std::endl;
            return;
        }
        
        std::stack<Command*> temp = doneCommands_;
        std::stack<Command*> reverse;   
        while(!temp.empty()) {
            reverse.push(temp.top());
            temp.pop();
        }

        Command* selectedCommand = nullptr;
        int i = 1;

        while (!reverse.empty() && selectedCommand == nullptr) {
            if (i == command) {
                selectedCommand = reverse.top() -> clone(); 
            } 
            else {
                reverse.pop(); 
                ++i;
            }
        }

        if (selectedCommand && i == command) {            
            selectedCommand -> show();            
            selectedCommand -> execute();
            doneCommands_.push(selectedCommand); 
        } 
        else {
            std::cout << "Command cannot be redone" << std::endl;
        }

        std::cout << std::endl;
    }

    void show() const { doc_.show(); }
    class Memento *checkpoint();   
    void rollback(const class Memento *);

private:
    Document doc_;
    std::stack<Command*> doneCommands_; 
}; 


class Memento {
public:
    Memento(const DocumentWithHistory& doc): doc_(doc) {}
    const DocumentWithHistory& getState() const { return doc_; }  
private:
    const DocumentWithHistory doc_;
};
 

Memento *DocumentWithHistory::checkpoint() { // needs to empty the command stack 
    while(!doneCommands_.empty()) { // history of commands should be cleared after a checkpoint 
        doneCommands_.pop();
    }
    return new Memento(*this);
}


void DocumentWithHistory::rollback(const Memento *mem){
    *this = mem -> getState();  // copying back
}


#endif
