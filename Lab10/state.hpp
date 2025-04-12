#ifndef STATE_HPP
#define STATE_HPP

#include <iostream>
#include <string>
#include <queue>
#include <vector>


class State; // forward class declaration

class Process {
public:
    Process();
    Process(State* state);
    Process* clone();
    void create();
    void dispatch();
    void block();
    void unblock();
    void suspend();
    void exit();
    std::string reportState();
    void changeState(State* state) { state_ = state; }
private:
    State* state_;
    int id_;
    static int nextID_;
};


class State {
public:
    virtual void create(Process*) {};
    virtual void dispatch(Process*) {};
    virtual void block(Process*) {};
    virtual void unblock(Process*) {};
    virtual void suspend(Process*) {};
    virtual void exit(Process*) {};
    virtual std::string reportState() = 0; // pure virtual
    void changeState(Process* p, State* s) { p -> changeState(s); }
};


class New : public State {
public:
    static State* instance() {
        static State* onlyInstance = new New;
        return onlyInstance;
    }

    void create(Process*) override;
    std::string reportState() override { return "created"; }

private:
    New() {};
    New(const New&) = delete;
};


class Ready : public State {
public:
    static State* instance() {
        static State* onlyInstance = new Ready;
        return onlyInstance;
    }

    void dispatch(Process*) override;
    std::string reportState() override { return "READY"; }
private:
    Ready() {};
    Ready(const Ready&) = delete;
};


class Running : public State {
public:
    static State* instance() {
        static State* onlyInstance = new Running;
        return onlyInstance;
    }

    void block(Process*) override;
    void suspend(Process*) override;
    void exit(Process*) override;
    std::string reportState() override { return "RUNNING"; }
private:
    Running() {};
    Running(const Running&) = delete;
};


class Blocked : public State {
public:
    static State* instance() {
        static State* onlyInstance = new Blocked;
        return onlyInstance;
    }

    void unblock(Process*) override;

    std::string reportState() override { return "BLOCKED"; }
private:
    Blocked() {};
    Blocked(const Blocked&) = delete;
};


class Exit : public State {
public:
    static State* instance() {
        static State* onlyInstance = new Exit;
        return onlyInstance;
    }
    std::string reportState() override { return "exited"; }
private:
    Exit() {};
    Exit(const Exit&) = delete;
};


int Process::nextID_ = 0;

Process::Process() {
    state_ = New::instance();
    ++nextID_;
}


Process::Process(State* s) {
    state_ = s;
    id_ = nextID_;
    ++nextID_;
}


Process* Process::clone() {
    return new Process(state_); // clone process with the current state
}


void Process::create()   { state_ -> create(this);   }
void Process::dispatch() { state_ -> dispatch(this); }
void Process::block()    { state_ -> block(this);    }
void Process::unblock()  { state_ -> unblock(this);  }
void Process::suspend()  { state_ -> suspend(this);  }
void Process::exit()     { state_ -> exit(this);     }


std::string Process::reportState() {
    std::string intString = std::to_string(id_);
    std::string state = state_ -> reportState();
    return ("\nPID " + intString + " state: " + state);
}


void New::create(Process* p) {
    changeState(p, Ready::instance());
    std::cout << p -> reportState();
}


void Ready::dispatch(Process* p) {
    changeState(p, Running::instance()); // change to running 
    std::cout << p -> reportState();
}


void Running::block(Process* p) {
    changeState(p, Blocked::instance()); // change to block
    std::cout << p -> reportState();
}


void Running::suspend(Process* p) {
    changeState(p, Ready::instance()); // after suspending process is ready
    std::cout << p -> reportState();
}


void Blocked::unblock(Process* p) {
    changeState(p, Ready::instance()); // process is ready after unblocked
    std::cout << p -> reportState();
}


void Running::exit(Process* p) {
    changeState(p, Exit::instance());
    std::cout << p -> reportState();
}

#endif
