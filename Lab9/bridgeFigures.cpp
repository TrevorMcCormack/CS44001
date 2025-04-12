#include <iostream>


// abstract body 
// implementation
class Fill{
public:
    Fill(char fillChar) : fillChar_(fillChar) {}
    Fill(char fillChar, char borderChar) : fillChar_(fillChar), borderChar_(borderChar) {}
    virtual char getBorder() = 0;
    virtual char getInternal() = 0;
    virtual ~Fill() {}
protected:
    char fillChar_;
    char borderChar_;
};

// concrete body
class Hollow: public Fill{
public:
    Hollow(char fillChar) : Fill(fillChar) {}
    char getBorder() override { return fillChar_; }
    char getInternal() override { return ' '; }
    ~Hollow() override {}
};


// another concrete body
class Filled: public Fill {
public:
    Filled(char fillChar) : Fill(fillChar) {}
    Filled(char internalChar, char borderChar) : Fill(internalChar, borderChar) {}
    char getBorder() override { return fillChar_; }
    char getInternal() override { return fillChar_; }
    ~Filled() override {}
};

// abstract handle
// abstraction
class Figure {
public:
    Figure(int size, Fill* fill) : size_(size), fill_(fill) {}
    virtual void draw() = 0;
    virtual ~Figure(){}
protected:
    int size_;
    Fill *fill_;
};

// concrete handle
class Square: public Figure{
public:
    Square(int size, Fill* fill) : Figure(size, fill) {}
    void draw() override;
};

void Square::draw(){
    for(int i = 0; i < size_; ++i){
        for(int j = 0; j < size_; ++j)
	        if(i == 0 || j == 0 || i == size_-1 || j == size_-1 )
	            std::cout << fill_ -> getBorder();
	        else
	            std::cout << fill_ -> getInternal();
                std::cout << std::endl;
    }
}


class FullyFilled: public Filled {
public:
    FullyFilled(char internalChar, char borderChar) : Filled(internalChar, borderChar), 
                                                      borderChar_(borderChar), 
                                                      internalChar_(internalChar) {}
    char getInternal() override { return internalChar_; }
    char getBorder() override { return borderChar_; }
   // ~FullyFilled() override {};
private:
    char borderChar_;
    char internalChar_;
};


class RandomFilled: public Filled {
public:
    RandomFilled(char firstChar, char secondChar) : Filled(firstChar, secondChar),
                                                    firstChar_(firstChar), 
                                                    secondChar_(secondChar) {}
    char getInternal() override;
    char getBorder() override;
   // ~RandomFilled() override;
private:
    char firstChar_;
    char secondChar_;
};


char RandomFilled::getInternal() {
    // get random character for internal
    int characterSelection = rand() % 2; // 0 or 1
    if(characterSelection == 0) {
        return firstChar_;
    }
    else return secondChar_;
}


char RandomFilled::getBorder() {
    // get random character for border
    int characterSelection = rand() % 2; // 0 or 1
    if(characterSelection == 0) {
        return firstChar_;
    }
    else return secondChar_;
}


int main(){

  
    Fill* hollowPaintJ = new Hollow('&');
    Fill* filledPaintStar = new Filled('@');
    Fill* full = new FullyFilled('%', '!'); // first is internal second is border
    Fill* random = new RandomFilled('#', '$');

   
    Figure *smallBox = new Square(6, hollowPaintJ);
    Figure *bigBox = new Square(15, filledPaintStar);
    Figure *fullBox = new Square(5, full);
    Figure *randBox = new Square(6, random);
   
    smallBox -> draw();
    std::cout << std::endl;
    bigBox -> draw();
    std::cout << std::endl;
    fullBox -> draw();
    std::cout << std::endl;
    randBox -> draw();
   
   
   /*
   // ask user for figure parameters
   cout << "Enter fill character: "; 
            char fchar; cin >> fchar;
   cout << "Filled or hollow? [f/h] "; 
           char ifFilled; cin >> ifFilled;
   cout << "Enter size: "; int size; cin >> size;
   */
   /*
   Figure *userBox = new Square(size, ifFilled == 'f'? 
	       static_cast<Fill *>(new Filled(fchar)):
	       static_cast<Fill *>(new Hollow(fchar))
	       ); 
   */
      
   /*
   Figure *userBox = ifFilled == 'f'?
      new Square(size, new Filled(fchar)):
      new Square(size, new Hollow(fchar));
   
   
   userBox -> draw();
   cout << endl;
   */
}