Exact Rules for Generated Special Member Functions

Let us summarize the rules for special member functions - when they are generated and how they behave. As an example, assume we have the following derived class:

```cpp
class MyClass : public Base
{
 private:
  MyType value;
  ...
};
```
The one thing missing here is the `noexcept` specification, which we will introduce later. Here are the guarantees for the various special member functions:

Copy Constructor

The copy constructor is automatically generated when all of the following apply:
* No move constructor is user-declared
* No move assignment operator is user-declared
If generated (implicitly or with `=default`), the copy constructor has the following behavior:

```cpp
MyClass(const MyClass& obj) noexcept
 : Base(obj), value(obj.value) {
}
```

The generated copy constructor first passes the source object, to the best matching copy constructor of the base class(es). Remember that copy constructors are always called on a top-down basis. It prefers the copy constructor with the same declaration (usually declared `const&`), but if that is not available it might call the next best matching constructor (e.g. a copy constructor template). Afterwards, it copies all members of its class (again using the best match). Afterwards, it copies all members of its class (again using the best match).
  The generated copy constructor is declared as `noexcept` if all copy operations (the copy constructors of all base classes and the copy constructors of all members) give this guarantee.

Move Constructor

The move constructor is automatically generated when all of the following apply:
* No copy constructor is user-declared
* No copy assignment operator is user-declared
* No move assignment operator is user-declared
* No destructor is user-declared
If generated (implicitly or with `=default`), the move constructor has the following behavior:

```cpp
MyClass(MyClass&& obj) noexcept
 : Base(std::move(obj)), value(std::move(obj.value)) {
}
```

The generated move constructor first passes the source object, marked with `std::move()` to pass through its move semantics, to the best matching move constructor of the base class(es). The best matching move constructor usually is the one with the same declaration (declared with `&&`).
