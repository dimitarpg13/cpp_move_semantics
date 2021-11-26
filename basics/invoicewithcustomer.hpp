#include <string>
#include <iostream>

class Customer {
public:
  Customer() = default;
  Customer(const Customer&) = default;
  Customer& operator=(const Customer&) = default;
  Customer(Customer&&) = delete;
  Customer& operator=(Customer&&) = delete;
};

class Invoice {
  std::string id;
  Customer cust;
public:
  // no special member functions
};
