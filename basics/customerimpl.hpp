#include <string>
#include <vector>
#include <iostream>
#include <cassert>

class Customer {
private:
  std::string name;  // name of the customer
  std::vector<int> values; // some values of the customer
public:
  Customer(const std::string& n)
    : name{n} {
       assert(!name.empty());
  }

  std::string getName() const {
    return name;
  }

  void addValue(int val) {
    values.push_back(val);
  }

  friend std::ostream& operator<< (std::ostream& strm, const Customer& cust) {
    strm << '[' << cust.name << ": ";
    for (int val : cust.values) {
      strm << val << ' ';
    }
    strm << ']';
    return strm;
  }

  // copy constructor (copy all members):
  Customer(const Customer& cust)
   : name{cust.name}, values{cust.values} {
      std::cout << "COPY " << cust.name << '\n';
  }

  // move constructor (move all members):
  Customer(Customer&& cust)     //noexcept declaration missing
    : name{std::move(cust.name)}, values{std::move(cust.values)} {
       std::cout << "MOVE " << name << '\n';
  }

  // copy assignment (assign all members):
  Customer& operator= (const Customer& cust) {
    std::cout << "COPYASSIGN " << cust.name << '\n';
    name = cust.name;
    values = cust.values;
    return *this;
  }

  // move assignment (move all members):
  Customer& operator= (Customer&& cust) { //noexcept declaration missing
    std::cout << "MOVEASSIGN " << cust.name << '\n';
    if (this != &cust) {  // move assignment to myself?
      name = std::move(cust.name);
      values = std::move(cust.values);
    }
    return *this;
  }
};
