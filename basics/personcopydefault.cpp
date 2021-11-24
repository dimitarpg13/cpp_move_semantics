#include "personcopydefault.hpp"
#include <iostream>
#include <vector>

int main()
{
  std::vector<Person> coll{"Wolfgang Amadeus Mozart",
                           "Johann Sebastian Bach",
                           "Ludwig van Beethoven"};
  std::cout << "capacity: " << coll.capacity() << '\n';
  Person p{"Pjotr Iljitsch Tshaikowski"};
  std::cout << "Copying Person p.." << std::endl;
  coll.push_back(p);  // OK: copies p
  std::cout << "Copying Person p using fallback.." << std::endl;
  coll.push_back(std::move(p)); // OK: copies p
}
