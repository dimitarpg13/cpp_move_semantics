#include "customer.hpp"
#include <iostream>
#include <random>
#include <utility> //for std::move()

int main()
{
  // create a customer with some initial values:
  Customer c{"Wolfgang Amadeus Mozart"};
  for (int val : {0, 8, 15}) {
    c.addValue(val);
  }
  std::cout << "c: " << c << '\n';    // print value of initialized c

  // insert the customer twice into a collection of customers:
  std::vector<Customer> customers;
  customers.push_back(c);             // copy into the vector
  customers.push_back(std::move(c));  // move into the vector
  std::cout << "c: " << c << '\n';    // print value of moved-from c

  // print all customers in the collection:
  std::cout << "customers:\n";
  for (const Customer& cust : customers) {
    std::cout << "  " << cust << '\n';  
  }
}
