// traversing genealogical tree using Composite and Visitors
// Mikhail Nesterenko
// 04/13/2023

#include <iostream>
#include <string>
#include <vector>

class Person { // component
public:
    Person(std::string firstName, Person *spouse, Person *father, Person *mother) :
        firstName_(firstName), spouse_(spouse), father_(father), mother_(mother){}
    const std::string & getFirstName() const { return firstName_; }
    void setFirstName(const std::string firstName) { firstName_ = firstName; }
    Person *getSpouse() const { return spouse_; }
    void setSpouse(Person *spouse){ spouse_ = spouse; }
    Person *getFather() const { return father_; }
    Person *getMother() const { return mother_; }

    virtual void accept(class PersonVisitor *) = 0;  
    virtual ~Person() {}
private:
    std::string firstName_;
    Person *spouse_;
    Person *father_;
    Person *mother_;
};


// leaf
// man has a last name 
class Man: public Person{
public:
    Man(std::string lastName, std::string firstName, Person *spouse, Person *father, Person *mother) : 
        lastName_(lastName), 
        Person(firstName, spouse, father, mother) {}
    const std::string & getLastName() const { return lastName_; }
    void accept(class PersonVisitor *visitor) override;
private:
    const std::string lastName_;
};

// composite
// woman has a list of children
class Woman: public Person{
public: 
    Woman(std::vector<Person *> children, std::string firstName, Person *spouse, Person *father, Person *mother) : 
        children_(children),
        Person(firstName, spouse, father, mother) {}
    const std::vector<Person *> & getChildren() const { return children_; }
    void setChildren(const std::vector<Person *> &children){ children_ = children;}
    void accept(class PersonVisitor *visitor) override;
private:
    std::vector<Person *> children_;
}; 

// abstract visitor
class PersonVisitor{
public:
    virtual void visit(Man*) = 0;
    virtual void visit(Woman*) = 0;
    virtual ~PersonVisitor(){}
};

// visitor method for the leaf
void Man::accept(PersonVisitor *visitor) {
    visitor -> visit(this);
}

// visitor method for the component: exectues visits and
// defines traversal 
void Woman::accept(PersonVisitor *visitor){ 
   // children traversal through mother only
   // father's children are not traversed so as not 
   // to traverse them twice: for mother and father

    visitor -> visit(this);
   

    for(auto child : children_) 
        child -> accept(visitor);   

}

// concrete visitors
// the last name for a man is stored in object
// the last name a woman is determined by her 
// spouse if she is married
// or by her father if she is not
class NamePrinter: public PersonVisitor{
public:
    void visit(Man *m) override {
        std::cout << m -> getFirstName() << " " << m -> getLastName() << std::endl;
    }
    void visit(Woman *w) override {
        std::cout << w -> getFirstName() << " ";
        if (w -> getSpouse() != nullptr)
            std::cout << static_cast<Man *>(w -> getSpouse()) -> getLastName();
        else if (w -> getFather() != nullptr)
            std::cout << static_cast<Man *> (w -> getFather()) -> getLastName();
        else
            std::cout << "Doe";
        std::cout << std::endl;
    }
};


class ChildrenPrinter: public PersonVisitor{
public:
    void visit(Man *m) override {
        std::cout << m -> getFirstName() << ": ";
        Woman *spouse = static_cast<Woman *>(m -> getSpouse());
        if(spouse != nullptr)
            printNames(spouse->getChildren());
        std::cout << std::endl;
    }
    void visit(Woman *w) override {
        std::cout << w -> getFirstName() << ": ";
        printNames(w -> getChildren());
        std::cout << std::endl;
    }
private:
    void printNames(const std::vector<Person *> &children){
        for(const auto c : children)
            std::cout << c -> getFirstName() << ", ";
    }
};


class NameUpdater : public PersonVisitor {
public:
    void visit(Man*) override;
    void visit(Woman*) override;
    std::string firstPartOfName(const std::string&);
};


std::string NameUpdater::firstPartOfName(const std::string& name) {
    size_t spaceIndex = name.find(' ');
    if(spaceIndex != std::string::npos) { // if the space is found
        return name.substr(0, spaceIndex); // get the string from the beginning to the space
    }

    return name;
}


void NameUpdater::visit(Man* m) {
    if(m -> getFather() != nullptr) {
        std::string fathersName = firstPartOfName(m -> getFather() -> getFirstName());
        std::string newName = m -> getFirstName() + " " + fathersName;
        m -> setFirstName(newName);

        std::cout << m -> getFirstName() << " " << m -> getLastName() << std::endl;
    }
}


void NameUpdater::visit(Woman* w) {
    if(w -> getMother() != nullptr) { // if woman has a mother 
        std::string mothersName = firstPartOfName(w -> getMother() -> getFirstName());
        const std::string newName = w -> getFirstName() + " " + mothersName;
        w -> setFirstName(newName);

        std::cout << w -> getFirstName() << " ";\
    }

    else {
        std::cout << w -> getFirstName() << " ";
    }
            
    // get the last name
    if (w -> getSpouse() != nullptr) {
        std::cout << static_cast<Man *>(w -> getSpouse()) -> getLastName();
    }
    else if (w -> getFather() != nullptr) {
        std::cout << static_cast<Man *> (w -> getFather()) -> getLastName();
    }
    else {
        std::cout << "Doe";
    }

    std::cout << std::endl;    
}


// demonstrating the operation
int main(){

   // setting up the genealogical tree      
   // the tree is as follows
   //    
   //
   //       James Smith  <--spouse-->   Mary 
   //	                                  |
   //	                                 children -------------------------
   //	                                  |              |                |
   //	                                  |              |                |
   //	   William Johnson <--spouse-> Patricia      Robert Smith       Linda
   //	                                  |
   //	                                 children------------
   //	                                  |                 |
   //                                     |                 |
   //	   Jennifer  <--spouse-->  Michael Johnson      Barbara
   //	       |
   //	     children
   //	       |
   //          |
   //	     Susan


    // first generation
    Man *js = new Man("Smith", "James", nullptr, nullptr, nullptr);
    Woman *ms = new Woman({}, "Mary", nullptr, nullptr, nullptr);
    ms -> setSpouse(js); js -> setSpouse(ms);

    // second generation
    Woman *ps = new Woman({}, "Patricia",  nullptr, js, ms);
    Man *wj = new Man("Johnson", "William", nullptr, nullptr, nullptr);
    ps -> setSpouse(wj); wj -> setSpouse(ps);

    std::vector<Person *> marysKids  = {ps,
                        new Man("Smith", "Robert", nullptr, js, ms),
                        new Woman({}, "Linda", nullptr, js, ms)};
    ms -> setChildren(marysKids);

    // third generation
    Man *mj = new Man("Johnson", "Michael", nullptr, wj, ps);
    std::vector<Person *> patsKids   = {mj, new Woman({}, "Barbara", nullptr, wj, ps)}; 
    ps -> setChildren(patsKids);

    Woman *jj = new Woman({}, "Jennifer", nullptr, nullptr, nullptr);
    std::vector<Person *> jensKids = {new Woman({}, "Susan", nullptr, mj ,jj)};

    jj -> setSpouse(mj); 
    mj -> setSpouse(jj);
    jj -> setChildren(jensKids);
    

    // defining two visitors
    ChildrenPrinter *cp = new ChildrenPrinter;
    NamePrinter *np = new NamePrinter;
    NameUpdater *nu = new NameUpdater;

    // executing the traversal with the composite
    // and the specific visitor

    
    std::cout << "\nNAME LIST\n";
    ms -> accept(np);
    std::cout << std::endl << std::endl;

    std::cout << "\nNEW NAME LIST\n";
    ms -> accept(nu);
    std::cout << std::endl << std::endl;
    /*
    std::cout << "CHILDREN LIST\n";
    ms -> accept(cp);
        
    std::cout << "\nJAMES' CHILDREN\n";
    js -> accept(cp);
    */
}