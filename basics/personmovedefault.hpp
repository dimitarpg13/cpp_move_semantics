#include <string>
#include <iostream>

class Person {
private:
  std::string name;
public:
  Person(const char* n)
  : name{n} {
  }

  std::string getName() const {
    return name;
  }

  // print out when we copy or move
  Person(Person&&) = default;
  Person& operator=(Person&&) = default;
  //...
};
