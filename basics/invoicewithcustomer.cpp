#include "invoicewithcustomer.hpp"
#include <iostream>
#include <vector>

int main() 
{
  Invoice i;
  Invoice i1{std::move(i)}; // OK, moves id, copies cust
}
