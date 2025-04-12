#include <iostream>

// target interface
class Square {
public:
    virtual void draw() const = 0;
    virtual ~Square(){}
};

// adaptee/implementer
class LegacyRectangle {
public:
    LegacyRectangle(int topLeftX, 
            int topLeftY, 
            int bottomRightX, 
            int bottomRightY): 
        topLeftX_(topLeftX), 
        topLeftY_(topLeftY), 
        bottomRightX_(bottomRightX), 
        bottomRightY_(bottomRightY){}

    void oldDraw() const {
        for(int i = 0; i < bottomRightY_; ++i){
            for(int j = 0; j < bottomRightX_; ++j)
                if(i >= topLeftY_  && j >= topLeftX_ )
                    std::cout << '*';
                else
                    std::cout << ' ';
            std::cout << std::endl;
        }   
    }

    void move(int topLeftX, int topLeftY, int bottomRightX, int bottomRightY) {
        topLeftX_ = topLeftX;
        topLeftY_ = topLeftY;
        bottomRightX_ = bottomRightX;
        bottomRightY_ = bottomRightY;
    }


    int getTopLeftX() { return topLeftX_; }
    int getTopLeftY() { return topLeftY_; } 
    int getBottomRightX() { return bottomRightX_; }
    int getBottomRightY() { return bottomRightY_; }


// defining top/left and bottom/right coordinates 
private: 
    int topLeftX_;
    int topLeftY_;
    int bottomRightX_;
    int bottomRightY_;
};

// adapter uses multiple inheritance to inherit
// interface and implementation
class SquareAdapter: public Square, 
		             private LegacyRectangle {
public:
    SquareAdapter(int size): LegacyRectangle(0,0,size,size){};
    void draw() const override {
        oldDraw();
    }


    int size() {
        return getBottomRightX() - getTopLeftX(); 
    }


    void resize(int newSize) {
        move(getTopLeftX(), getTopLeftY(), newSize, newSize);
    }

};


int main() {
    std::cout << "Enter size for square: ";
    int size;
    std::cin >> size;
    std::cout << std::endl;


    Square *square = new SquareAdapter(size);
    square -> draw();

    SquareAdapter* s = new SquareAdapter(size); 
    std::cout << "Current size: " << s -> size() << std::endl;

    std::cout << "Enter new size: ";
    std::cin >> size;
    std::cout << std::endl;
    s -> resize(size);
    s -> draw();

    std::cout << "New size: " << s -> size();
}