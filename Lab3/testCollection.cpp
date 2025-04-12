#include "List.hpp" // list template
#include <cassert>

int main(){

    // manipulating integers
   Collection<int> cone, emptyCol;
   std::cout << "Integer collection: ";
   std::cout << "adding 1 ";  cone.add(1);
   assert(cone.last() == 1);
   std::cout << "adding 2 ";  cone.add(2);
   assert(cone.last() == 2);
   std::cout << "adding 3 ";  cone.add(3);
   assert(cone.last() == 3);
   std::cout << "adding duplicate 2 "; cone.add(2);
   assert(cone.last() == 2);
   std::cout << std::endl;
   
   cone.print();

   std::cout << "removing 2 " ; cone.remove(2);
   std::cout << std::endl;;

   cone.print();

   assert(cone.last() == 3);
   assert(equal(cone, cone));
   cone.remove(3);
   cone.remove(1);

   assert(equal(cone, emptyCol));
   
   // uncomment when you debugged the code above
   

   // manipulating strings
   std::string sa[] = {"yellow", "orange", "green", "blue"};
   Collection<std::string> ctwo;

   std::cout << "adding an array of strings ";
   for(auto s : sa)
      ctwo.add(s);

   std::cout << "String collection: ";
   ctwo.print();


   // manipulating character collections

   // individal collections
   Collection<char> a2g, h2n, o2u;
   std::cout << "Populating individual character collections" << std::endl;
   for(char c='a'; c <='g'; ++c) {
      a2g.add(c);
      assert(a2g.last() == c);
   }
   for(char c='h'; c <='n'; ++c) h2n.add(c);
   for(char c='o'; c <='u'; ++c) o2u.add(c);

   
   assert(!equal(a2g, h2n));

   // collection of collections
   Collection<Collection<char>> cpile;

   std::cout << "Adding individual collections " << std::endl;
   cpile.add(a2g);
   assert(equal(cpile.last(), a2g));
   cpile.add(h2n);
   assert(equal(cpile.last(), h2n));
   cpile.add(o2u);
   assert(equal(cpile.last(), o2u));

   // printing characters from last collection added
   std::cout << "Last added character collection: ";
   cpile.last().print();
   
}