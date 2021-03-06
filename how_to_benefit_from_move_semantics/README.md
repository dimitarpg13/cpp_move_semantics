## How To Benefit From Move Semantics

### Avoid Objects with Names

Move semantics allows us to optimize using a value from a source that no longer needs that value. If compilers automatically detect that a value is used from an object that is at the end of its lifetime, they will automatically switch to move semantics. This is the case when:
* we pass a temporary object that will automatically be destroyed after the statement
* we return a local object by value
In addition, we can force move semantics by marking an object with `std::move()`.
A first consequence of move semantics is the following: **avoid objects with names**.
Instead of
```cpp
MyType x{42, "hello"};
foo(x); // x not used afterwards
```
it would be better to write:
```cpp
foo(MyType{42, "hello"});
```
to automatically enable move semantics.
Of course, the advice to avoid objects with names might conflict with other important style guidelines such as readability and maintainability of source code. Instead of having a complex statement, it might be better to use multiple statements. In that case, you should use `std::move()` if you no longer need an object (and know that copying the object might take significant time):
```cpp
foo(std::move(x));
```

### When You Cannot Avoid Using Names

Thee are cases where you cannot avoid using `std::move()` because you have to give objects names. The most obvious typical examples are:
* You have to use an object multiple times. For example, you might get a value to process it twice in a function or a loop:
```cpp
std::string str{getData()};
...
coll1.push_back(str);   // copy (still need the value of str)
coll2.push_back(std::move(str)); // move (no longer need the value of str)
``` 
The same applies when inserting the value in the same collection twice or calling two different functions storing the value somewhere.
* You have to deal with a parameter. The most common example of this is the following loop:
```cpp
// read and store line by line from myStream in coll
std::string line;
while (std::getline(myStream, line)) {
  coll.push_back(std::move(line));  // move (no longer need the value of line)
}
```

#### Avoid Unnecessary `std::move()`

Returning a local object by value automatically uses move semantics if supported. However, just to be safe, programmers might try to force this with an explicit `std::move()`:

```cpp
std::string foo()
{
  std::string s;
  ...
  return std::move(s); // BAD: do not do this
}
```
Remember that `std::move() is just a static_cast` to an rvalue reference. Therefore, `std::move(s)` is an expression that yields the type `std::string&&`. However, this no longer matches the return type and therefore disables the `return value optimization`, which usually allows the returned object to be used as a return value. For types where move semantics is not implemented, this might even force the copying of the return value instead of just using the returned object as the return value.
Therefore, if you return local objects by value, do not use `std::move()`:

```cpp
 std::string foo()
 {
   std::string s;
   ...
   return s;  // best performance (return value optimization or move)
 }
```

Using ```std::move()``` when you already have a temporary object is at least redundant. For a function `createString()` that returns an object by value, you should just use the return value:

```cpp
 std::string s{createString()};  // OK
```

instead of marking it with `std::move()` again:

```cpp
 std::string s{std::move(createString())};  // BAD: do not do this
```  

Compilers might (have options to) warning about any counterproductive or unnecessary use of `std::move()`. For example, *gcc* has the options `-Wpessimizing-move` (enabled with `-Wall`) and `-Wredundant-move` (enabled with `-Wextra`). 
  There are applications, though, where a `std::move()` in a return statement might be appropriate. One example is moving out the value of a member. Another example is returning a parameter with move semantics. 


### Initialize Members with Move Semantics

A surprising consequence of the way the move semantics is defined is that you can benefit from it even in trivial classes with members of types that benefit from move semantics (such as string members or containers).
 Let us look at this with a simple example.

#### Initialize Members the Classical Way

Consider a class with two string members, which we can initialize in the constructor. Such a class will typically be implemented like this:

`basics/initclassic.hpp`
```cpp
#include <string>

class Person {
private:
 std::string first; // first name
 std::string last;  // last name
public:
 Person(const std::string& f, const std::string& l)
  : first{f}, last{l} {
 }
 ...
};
```

Now let us look what happens when we initialize an object of this class with two string literals:

```cpp
 Person p{"Ben", "Cook"};
```
The compiler finds that the provided constructor can perform the initialization. However, the types of the parameters do not fit. Therefore the compiler generates code to first create two temporary `std::string`s, which are initialized by the values of the two string literals, and binds the parameters `f` and `l` to them: 

<img src="images/class_layout_pic1.png" width="473" height="300">

In general (if the small string optimization (SSO) is not available or the string is too long), this means that code is generated that allocates memory for the value of each `std::string`. 
 However, the temporary strings created are not used directly as members `first` or `last`. Instead, they are used to initialize these members. Unfortunately, the move semantics is not used here for two reasons:
* The parameters `f` and `l` are objects with names that exist for a longer period than the initialization of the members (you can still use them in the body of the constructor).
* The parameters are declared to be `const`, which disables the move semantics even if we use `std::move`. 

As a consequence, the copy constructor for strings is called on each member initialization, again allocating memory for the values:

<img src="images/class_layout_pic2.png" width="473" height="300">

At the end of the constructor, the temporary strings are destroyed:

<img src="images/class_layout_pic3.png" width="473" height="300">

This means that we have four memory allocations although only two are necessary. Using move semantics we can do better.

#### Using non-`const` Lvalue References?

Why we simply not use non-`const` lvalue references here:

```cpp
class Person {
 ...
 Person(std::string& f, std::string& l) 
  : first{std::move(f)}, last{std::move(l)} {
 }
 ...
};
```

However, passing `const std::string` and temporary objects (e.g. created from type conversion) would not compile:

```cpp
 Person p{"Ben", "Cook"}; // ERROR: cannot bind non-constant lvalue reference to temporary
```

In general, non-const lvalue reference does not bind to a temporary object. Therefore, this constructor cannot bind `f` and `l` to temporary strings created from passed string literals. 

#### Initialize Members via Moved Parameters Passed by Value

With move semantics there is now a simple alternative way for constructors to initialize members:
the constructor takes each argument by value and moves it into the member:

`basics/initmove.cpp`

```cpp
#include <string>

class Person {
 private:
  std::string first; // first name
  std::string last;  // last name
 public:
  Person(std::string f, std::string l) 
   : first{std::move(f)}, last{std::move(l)} {
  }
 ...
};
```

This single constructor takes all possible arguments and ensures that we have only one allocation for each argument. 
For example, if we pass two string literals:
```cpp
 Person p{"Ben", "Cook"}; 
```
we first use them to initialize the parameters `f` and `l`:

<img src="images/class_layout_pic1.png" width="473" height="300">

By using `std::move()`, we move the values of the parameters to the members. First, the member `first` steals the value from `f`. Then the member `last` steals the value from `l`:

<img src="images/class_layout_pic4.png" width="473" height="300">

Again, at the end of the constructor, the temporary strings are destroyed. This time it takes less time because the destructors of the strings no longer have to free allocated memory:

<img src="images/class_layout_pic5.png" width="473" height="300">

This way to initialize the members also works fine if we pass `std::string`s:

* if we pass two existing strings without marking them with `std::move()`, we copy the names to the parameters and move them to the members:

```cpp
 std::string name1{"Jane"}, name2{"White"};
 ...
 Person p{name1, name2}; //OK, copy names into parameters and move them to the members
``` 

* if we pass two strings where the value is no longer needed, we do not need any allocation at all:

```cpp
 std::string firstname{"Jane"};
 ...
 Person p{std::move(firstname), // OK, move names via parameters to members
          getLastnameAsString()};
```

In this case we move the passed strings twice: once to initialize the parameters `f` and `l` and once to move the values of `f` and `l` to the members.
Provided a move is cheap, with this implementation of only one constructor any initialization is possible and cheap.

#### Initialize Members via Rvalue References

There are even more ways to initialize the members of a Person, using multiple constructors.

##### Using Rvalue References

To support move semantics, we can declare a parameter as a non-`const` rvalue reference. This allows the parameter to steal the value from a passed temporary object or an object marked with `std::move()`.
 Consider we declare the constructor as follows:

```cpp
 class Person {
  ...
  Person(std::string&& f, std::string&& l) 
   : first{std::move(f)}, last{std::move(l)} {
  }
  ...
 };
```

This initialization would also work for our passed string literals:

```cpp
 Person p{"Ben", "Cook"};
```

Again, because the constructor needs strings, we would create two temporary strings to which `f` and `l` bind:

<img src="images/class_layout_pic1.png" width="473" height="300">

And because we have non-`const` references, we can modify them. In this case, we mark them with `std::move()` so that the initialization of the members can steal the values. First, the member `first` steals the value from `f`. Then the member `last` steals the value from `l`:

<img src="images/class_layout_pic4.png" width="473" height="300">

Again, at the end of the constructor, the temporary strings are destroyed without the need to free allocated memory:

<img src="images/class_layout_pic5.png" width="473" height="300">

However, this constructor does not work in all cases.

#### Overloading for Rvalue and Lvalue references
