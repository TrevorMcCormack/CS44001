#ifndef LIST_HPP_
#define LIST_HPP_

#include <iostream>

// the pointer part of every list is the same
// lists differ by data part
template <typename T>
class node{
public:

  node(): next_(nullptr) {}

  // functions can be inlined
  T getData() const { return data_; }
  void setData(const T& data){ data_=data; }

  // or can be defined outside
  node<T>* getNext() const;
  void setNext(node<T> *const);

private:
  T data_;
  node<T> *next_;
};

//
// member functions for node
//
template <typename T>
node<T>* node<T>::getNext() const {
  return next_;
}

template<typename T>
void node<T>::setNext(node<T> *const next){
   next_=next;
}



// class for collection
template <typename T>
class Collection {
    public:
        Collection() { head = nullptr; };
        void add(const T&);
        void remove(const T&);
        T last();
        void print();

        template <typename U> 
        friend bool equal(const Collection<U>&, const Collection<U>&);

    private:
        node<T> *head;
};



// class definitions for collection class

template <typename T>
void Collection<T>::add(const T& item) {
    // adds an item to the collection does not check for duplicates. appends to the back

    node<T> *temp = new node<T>; // create a temp new node of item
    temp -> setData(item); // set items data into the new node's data_ that temp is pointing to
    temp -> setNext(nullptr); // set nullptr to the new node's next_ pointer 

    // use nullptr since this node should not point to another node since it should be the last node

    // check if collection is empty
    if(head == nullptr) {
        // then assign head to point at the new node since it is the first
        head = temp;
    }
    // else there are already nodes
    else {
        // point to the next node
        temp -> setNext(head); // makes temp's next_ point to head to link the nodes
        head = temp; // set head to point at the new temp node 
        // after this next_ will point to the old head node and head will point to the new temp node
    }
}


template <typename T>
void Collection<T>::remove(const T& item) {
    // removes an item from the collection. removes all instances of this item from the collection. deallocate properly
    // start at head and check if the item is equal to the item inside each node remove if it is
    // if it is not equal then move on to the next node using next_
    // need a temp pointer to point to each node to access the data inside to check it

    node<T>* current = head; // create node that stores head
    node<T>* previous = nullptr;

    // make sure that current is not pointing at nullptr
    while(current != nullptr) {
        // checks if the item that is stored in the head node is equal to the remove item
        if(current -> getData() == item) {
            // if they are equal then remove the node. 
            // set head to be the node at next_ 
            // delete the node
            
            if(current == head) {
                // if current is at the beginning then move the head to the next node
                head = current -> getNext();
            }
            // else it is not the current node so move to the next node
            else {
                // current's next_ will point to the next node then set where that next_ is pointing to previous's next_
                previous -> setNext(current -> getNext());
            }
            node<T>* temp = current; // set temp node = to the current node
            current = current -> getNext(); // make current point to the next node
            delete temp; // delete the node
        } 

        // else item is not stored so move to the next node
        else {
            // make the previous node go to the next node
            previous = current; 
            // move the current node to the next node
            current = current -> getNext();
        }
    }
    // when current is nullptr return
    return;
}


template <typename T>
T Collection<T>::last() {
    // returns the last item in the collection

    return head -> getData();
}


template <typename T>
void Collection<T>::print() {
    // prints all the items in the collection
    
    node<T> *temp = head; // initialize temp to be the first node

    // make sure it isnt empty
    while(temp != nullptr) {
        // get data_ in the temp node which is head then print data
        std::cout << temp -> getData() << std::endl; 
        temp = temp -> getNext(); // get next_ from the temp node which was head's next and have temp point to that next node
    }
}


template <typename T> 
bool equal(const Collection<T> &lOne, const Collection<T> &lTwo) {
    node<T>* listOne = lOne.head; // set a new node to equal the head node of list one
    node<T>* listTwo = lTwo.head; // set a new node to equal the head node of list two

    // check if either list is nullptr which would mean one is longer than the other
    while(listOne != nullptr && listTwo != nullptr) {
        // check if the data in listOne != listTwo
        if(listOne -> getData() != listTwo -> getData()) {
            return false;
        }
        else {
            // else they are equal so move onto the next node in each list to check those nodes
            listOne = listOne -> getNext(); // get next node is first list
            listTwo = listTwo -> getNext(); // get next node in second list
        }
    }
    // both lists are equal
    return true;
} 


#endif // LIST_HPP_
