#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> createAndInsert()
{
   std::vector<std::string> coll; // create vector of strings
   coll.reserve(3);   // reserve memory for 3 elements
   std::string s = "data"; // create string object

   coll.push_back(s);            // insert string object
   coll.push_back(s+s);          // insert temporary string
   coll.push_back(std::move(s)); // insert string (we no longer need the value of s)

   return coll;
}

int main()
{
   std::vector<std::string> v;   // create empty vector of strings
   v = createAndInsert();        // assign returned vector of strings
   for (auto i = v.begin(); i != v.end(); ++i)
           std::cout << *i << ' ';
}
