#include <iostream>
#include <cstddef>
#include <utility>
#include <functional>
#include <vector>
#include <list>

//////////////////////////////////////////
// hash function implemented as a class
//////////////////////////////////////////

// any Hash Class must provide 
// two methods: hash() and numBuckets().
template <typename T>
class DefaultHash {
public:
    DefaultHash(size_t numBuckets = defaultNumBuckets);
    size_t hash(const T& key) const;
    size_t numBuckets() const { return numBuckets_; }

private:
    // default number of buckets in the hash
    static const size_t defaultNumBuckets = 101; 
    size_t numBuckets_;
};

template <typename T>
DefaultHash<T>::DefaultHash(size_t numBuckets): numBuckets_(numBuckets) {}


// uses the division method for hashing.
// treats the key as a sequence of bytes, sums the ASCII
// values of the bytes, and mods the total by the number
// of buckets.
// note, this function does not work for C++ strings
template <typename T>
size_t DefaultHash<T>::hash(const T& key) const {
    size_t res = 0;
    for (size_t i = 0; i < sizeof(key); ++i) {
        const unsigned char b = 
        *(reinterpret_cast<const unsigned char *>(&key) + i);
        res += b;
    }
    return res % numBuckets_;
}


////////////////////////////////////////////////
// container class
////////////////////////////////////////////////

template <typename Key, typename Value, 
          typename Compare = std::equal_to<Key>,
          typename Hash = DefaultHash<Key>>
class hashmap{

public:
    typedef std::pair<const Key, Value> Element;

    // constructor
    // invokes constructors for comparison and hash objects
    hashmap(const Compare& comp = Compare(), 
	    const Hash& hash = Hash());

    Element* find(const Key& x);      // returns pointer to element with key x,
                                     // nullptr if not found

    size_t size() const { return size_; };

    size_t getNumBuckets() const;

    std::pair<Element*, bool> insert(const Element& x);    // inserts the key/value pair 

    std::pair<Element*, bool> erase(const Key& x);         // erases element with key x, if exists

    Value& operator[] (const Key& x); // returns reference on value of
                                     // element with key, inserts if does not exist
    void rehash(size_t n);
private:

    // helper function for various searches
    typename std::list<Element>::iterator findElement(const Key& x, const size_t bucket);

    size_t size_;   // number of elements in the container
    Compare comp_;  // comparison functor, equal_to by default
    Hash hash_;     // hash functor 

    // hash contents: vector of buckets
    // each bucket is a list containing key->value pairs
    std::vector<std::list<Element>> elems_;
};


////////////////////////////////////////////////
// container member functions
////////////////////////////////////////////////

// Construct elems_ with the number of buckets.
template <typename Key, typename Value, typename Compare, typename Hash>
hashmap<Key, Value, Compare, Hash>::hashmap(const Compare& comp, const Hash& hash):
    size_(0), comp_(comp), hash_(hash) {
        elems_ = std::vector<std::list<Element>>(hash_.numBuckets());
    }


// helper function
template <typename Key, typename Value, typename Compare, typename Hash>
typename std::list<std::pair<const Key, Value>>::iterator // return type
hashmap<Key, Value, Compare, Hash>::findElement(const Key& x, size_t bucket) {
    // look for the key in the bucket
    for (auto it =  elems_[bucket].begin(); it != elems_[bucket].end(); ++it)
        if (comp_(it -> first, x))
	        return it;

    return elems_[bucket].end(); // element not found
}


// returns a pointer to the element with key x
// returns nullptr if no element with this key
template <typename Key, typename Value, typename Compare, typename Hash>
typename hashmap<Key, Value, Compare, Hash>::Element* // return value type
hashmap<Key, Value, Compare, Hash>::find(const Key& x) {

    size_t bucket = hash_.hash(x);
    auto it = findElement(x, bucket);    // use the findElement() helper   

    if (it != elems_[bucket].end())
        // found the element. Return a pointer to it.
        return &(*it); // dereference the iterator to list 
                     // then take the address of list element
   
    else // didn't find the element -- return nullptr
        return nullptr;
}


// finds the element with key x, inserts an
// element with that key if none exists yet. Returns a reference to
// the value corresponding to that key.
template <typename Key, typename Value, typename Compare, typename Hash>
std::pair<typename hashmap<Key, Value, Compare, Hash>::Element*, bool>  // return type
hashmap<Key, Value, Compare, Hash>::insert(const Element& x) {
    bool result = false;
    size_t bucket = hash_.hash(x.first);   
    auto it = findElement(x.first, bucket);    // try to find the element

    // if not found, insert a new one.
    if (it == elems_[bucket].end()) {
        ++size_;
        elems_[bucket].push_back(x);
        result = true;
        it = findElement(x.first, bucket);
    } 

    return std::make_pair(&(*it), result);
}


// removes the Element with key x, if it exists
template <typename Key, typename Value, typename Compare, typename Hash>
std::pair<typename hashmap<Key, Value, Compare, Hash>::Element*, bool>
hashmap<Key, Value, Compare, Hash>::erase(const Key& x) {
    size_t bucket = hash_.hash(x);
    auto it = findElement(x, bucket);    // try to find the element

    if (it != elems_[bucket].end()) {    // the element exists, erase it
        --size_;    // decrement size since element will be erased

        if(size_ == 0) { // if the hashmap is empty after decrementing size
            elems_[bucket].erase(it); // erase the element and return a pair where the pointer is nullptr
            return std::make_pair(nullptr, true);
        }
        it = elems_[bucket].erase(it); // remove the current element and get new iterator

        if(it == elems_[bucket].end()) { // if there are no other elements in the bucket
            int nextBucket = bucket + 1; // move to the next bucket

            while(nextBucket < hash_.numBuckets() && elems_[nextBucket].empty()) { // find the next non empty bucket
                ++nextBucket;
            }

            if(nextBucket < hash_.numBuckets()) {
                it = elems_[nextBucket].begin(); // set it to the beginning of non empty bucket
            }
            else {
                return std::make_pair(nullptr, true);
            }
        }

        return std::make_pair(&(*it), true);
    }
    else {
        return std::make_pair(nullptr, false);
    }

}


// returns reference to value of element with key x,
// inserts if does not exist
template <typename Key, typename Value, typename Compare, typename Hash>
Value& hashmap<Key, Value, Compare, Hash>::operator[] (const Key& x) {
    auto pair = insert(std::make_pair(x, Value()));
    return pair.first -> second;
}


template <typename Key, typename Value, typename Compare, typename Hash>
void hashmap<Key, Value, Compare, Hash>::rehash(size_t n) {
    if(n <= hash_.numBuckets()) { // no need to rehash so return
        return;
    }
    hash_ = DefaultHash<Key>(n); // update the hash to use new num of buckets n
    std::vector<Element> v;

    for(auto &list : elems_) { // get each list from elems
        for(auto &e : list) { // get each element from the list
            v.push_back(e); // add each element to v
        }
    }

    elems_.clear(); // empty elems

    // resize elems_ to new num of buckets
    elems_ = std::vector<std::list<Element>>(hash_.numBuckets()); 

    for(auto &e : v) {
        size_t t = hash_.hash(e.first) % hash_.numBuckets(); // calculate the bucket index
        elems_[t].push_back(e); // insert the element back into its new bucket
    } 
}


template <typename Key, typename Value, typename Compare, typename Hash>
size_t hashmap<Key, Value, Compare, Hash>::getNumBuckets() const {
    return hash_.numBuckets();
}