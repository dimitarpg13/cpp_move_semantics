#include <string>
#include <vector>
#include <iostream>
#include <cassert>

class Customer {
private:
  std::string name;         // name of the customer
  std::vector<int> values;  // some values of the customer
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
      strm << val << '.';
    }
    strm << ']';
    return strm;
  }
};   
