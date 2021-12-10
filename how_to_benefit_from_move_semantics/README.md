How To Benefit From Move Semantics

Avoid Objects with Names

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

When You Cannot Avoid Using Names

Thee are cases where you cannot avoid using `std::move()` because you have to give objects names. The most obvious typical examples are:
* You have to use an object multiple times. For example, you might get a value to process it twice in a function or a loop:
```cpp
 std::string str{getData()};
``` 
