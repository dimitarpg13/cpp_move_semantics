#include <iostream>
#include <cstddef>
#include <cstring>
#include <utility>

class rule_of_three
{
   char* cstring; // raw pointer used as a handle to a dynamically-allocated memory block
   rule_of_three(const char* s, std::size_t n) // to avoid counting twice
   : cstring(new char[n]) // allocate
   {
     std::memcpy(cstring, s, n); // populate
   }
public:
  rule_of_three(const char* s = "")
  : rule_of_three(s, std::strlen(s) + 1)
  {}
  ~rule_of_three() // I. destructor
  {
    delete[] cstring; // deallocate
  }
  rule_of_three(const rule_of_three& other) // II. copy constructor
  : rule_of_three(other.cstring)
  {}
  rule_of_three& operator=(const rule_of_three& other) // III. copy assignment
  {
    if (this == &other) 
      return *this;

    std::size_t n{std::strlen(other.cstring) + 1};
    char* new_cstring = new char[n]; // allocate
    std::memcpy(new_cstring, other.cstring, n); // populate
    delete[] cstring; // deallocate
    cstring = new_cstring;

    return *this; 
  }
public:
  operator const char *() const { return cstring; } // accessor
};

class rule_of_five
{
  char* cstring; // raw pointer used as a handle to a dynamically-allocated memory block
public:
  rule_of_five(const char* s = "")
  : cstring(nullptr)
  {
    if (s) {
      std::size_t n = std::strlen(s) + 1;
      cstring = new char[n];  // allocate
      std::memcpy(cstring, s, n); // populate
    }
  }

  ~rule_of_five()
  {
    delete[] cstring; // deallocate
  }

  rule_of_five(const rule_of_five& other) // copy constructor
  : rule_of_five(other.cstring)
  {}

  rule_of_five(rule_of_five&& other) noexcept // move constructor
  : cstring(std::exchange(other.cstring, nullptr)) // replaces the value of other.cstring with  
  {}                                               // nullptr and initializes cstring with the
                                                   // former value

  rule_of_five& operator=(const rule_of_five& other) // copy assignment
  {
    return *this = rule_of_five(other);
  }

  rule_of_five& operator=(rule_of_five&& other) noexcept
  {
    std::swap(cstring, other.cstring);
    return *this;
  } 
  // alternatively, replace both assignment operators with
  // rule_of_five& operator=(rule_of_five other) noexcept
  // {
  //   std::swap(cstring, other.cstring);
  //   return *this;
  // }
public:
  operator const char *() const { return cstring; } // accessor
};

int main()
{
  rule_of_five o1{"abc"};
  std::cout << o1 << ' ';
  auto o2{ o1 };  // I. uses copy constructor
  std::cout << o2 << ' ';
  rule_of_five o3("def");
  std::cout << o3 << ' ';
  o3 = o2;  //  III. uses copy assignment
  std::cout << o3 << ' ';
}  // <- II. all destructors are called 'here'  
