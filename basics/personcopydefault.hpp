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
  Person(const Person&) = default;
  Person& operator=(const Person&) = default;
  //...
};
