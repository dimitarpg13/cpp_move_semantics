#include "personmovedefault.hpp"
#include <iostream>
#include <vector>

int main()
{
  //std::vector<Person> coll{"Wolfgang Amadeus Mozart",
  //                         "Johann Sebastian Bach",
  //                         "Ludwig van Beethoven"};
  //std::cout << "capacity: " << coll.capacity() << '\n';
  std::vector<Person> coll;
  Person p{"Pjotr Iljitsch Tshaikowski"};

  // Attempting to compile the two lines of code commented below produces the 
  // following error message:
  //
  // note: in instantiation of member function 'std::vector<Person>::push_back' requested here
  // coll.push_back(p);  // OK: copies p
  //     ^
  // note: copy constructor is implicitly deleted because 'Person' has a user-declared
  //  move constructor
  // Person(const Person&&) = default;
  // ^
  //
  //std::cout << "Copying Person p.." << std::endl;
  //coll.push_back(p);  // OK: copies p
  std::cout << "Moving Person p.." << std::endl;
  coll.push_back(std::move(p)); // OK: moves p
}
