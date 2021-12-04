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
If generated (implicitly or with =default), the copy constructor has the following behavior:

```cpp
MyClass(const MyClass& obj) *noexcept-specifier*
 : Base(obj), value(obj.value) {
}
```
