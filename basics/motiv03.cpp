#include <string>
#include <vector>

std::vector<std::string> createAndInsert() 
{
   std::vector<std::string> coll; // create vector of strings
   coll.reserve(3);               // reserve memory of 3 elements
   std::string s = "data";

   coll.push_back(s);             // insert a string object
   coll.push_back(s+s);           // insert a temporary string
   coll.push_back(s);             // insert a string

   return coll;                   // return a vector of strings
}

int main() 
{
  std::vector<std::string> v; // create an empty vector of strings
  v = createAndInsert();
}
