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

The generated move constructor first passes the source object, marked with `std::move()` to pass through its move semantics, to the best matching move constructor of the base class(es). The best matching move constructor usually is the one with the same declaration (declared with `&&`). However, if that is not available it might call the next best matching constructor (e.g. a move constructor template or even a copy constructor). Afterwards, it moves all members of its class (again using the best match).

 The generated move constructor is declared as `noexcept` if all called move/copy operations (the copy or move constructors of all base classes and the copy or move constructors of all members) give this guarantee.

Copy Assignment Operator

The copy assignment operator is automatically generated when all of the following apply:
* No move constructor is user-declared
* No move assignment operator is user-declared
If generated (implicitly or with `=default`), the copy assignment operator approximately has the following behavior:

```cpp
MyClass& operator= (const MyClass& obj) noexcept {
 Base::operator=(obj);  // - perform assignments for base class members
 value = obj.value;     // - assign new members
 return *this;
}
```

The generated copy assignment operator first calls the best matching assignment operator of the base class(es) for the passed source object (remember that assignment operators in contrast to copy constructors are *not* called on a top-down basis; they call the base class assignment operator(s) in the implementation). Afterwards it assigns all members of its class (again using the best match).
 Note that the generated assignment operator does not check for assignments of objects to themselves. If this is critical, you have to implement the operator yourself.
 In addition, the generated copy assignment operator is declared as `noexcept` if all assignment operations (the assignment of the base class members and the assignment of the new members) give this guarantee.

Move Assignment Operator

The move assignment operator is autmatically generated when all of the following apply:
* No copy constructor is user-declared
* No move constructor is user-declared
* No copy assignment operator is user-declared
* No destructor is user-declared

If generated (implicitly or with `=default`), the move assignment operator approximately has the following behavior:

```cpp
MyClass& operator= (MyClass&& obj) noexcept {
 Base::operator=(std::move(obj));  // - perform move assignments for base class members
 value = std::move(obj.value);     // - move assign new members
 return *this;
}
```

The generated move assignment operator first calls the best matching move assignment operator of the base class(es) for the passed source object, marked with `std::move()` to pass through its move semantics. Afterwards it move assigns all members of its class (again using the best match).

 You might wonder why we still use members of the source object `obj` after the object was marked with `std::move()`:

```cpp
 Base::operator=(std::move(obj)); // - perform move assignments for base class members
```

However, in this case we mark the object for the specific context of a base class, which cannot see the members introduced in this class. Therefore, the dervied members have a valid but unspecified state but we can still use the values of the new members.
 The generated assignment operator also does not check for assignments of objects to themselves. Thus, in its default behavior, the operator will move assign each member to itself, which usually means that members receive a valid but unspecified value. If this is critical, you have to implement the operator yourself. 
 In addition, the generated move assignment operator is declared as `noexcept` if all called assignment operations (the assignments for base class members and the assignments for new members) give this guarantee. 

Other Special Member Functions

Other special member functions do not play such an important role for move semantics:
* Destructors are nothing special with move semantics except that their declaration disables the automatic generation of move operations.
* The default constructor (the "not-so-special" special member function) is still automatically generated if no other constructor is declared. That is, the declaration of a move constructor disables the generation of a default constructor. 

However, note that these special member functions play role when talking about moved-from states. The state of the default constructor is often the "natural" state of a moved-from object and a moved-from object always should be destructible.

The Rule of Five or Three

Weather and which special member functions are automatically generated depends on a combination of several of the rules just described. Before C++11, this guideline was called the **Rule of Three** : The guideline was to either declare all htree (copy constructor, assignment operator and destructor) or none of them. Since C++11, the rule has become the **Rule of Five**, which is usually formulated as: The guideline is to either declare all five (copy constructor, move constructor, copy assignment operator, move assignment operator, and destructor) or none of them.

Here, *declaring* means:
* Either to implement ({...})
* Or to declare as defaulted (=default)
* Or to declare as deleted (=delete)

That is, when one of these special member functions is either implemented or defaulted or deleted, you should implement or default or delete all four other special member functions.
 A we saw, to enable copy semantics only you should `=default` the copying special member functions without declaring the special move member functions (deleting and defaulting the special move member functions would not work, implementing them makes the class unnecessarily complicated). This option is recommended in particular if the generated move semantics creates invalid states.

When applying this Rule of Five it also turned sometimes that sometimes the programmers use it to add declarations for new move operations without understanding what this means. Programmers were just declaring move operations with `=default` because copy operations were implemented and they wanted to follow the Rule of Five. Therefore here is the Rule Of Five Or Three:

* if you declare the copy constructor, move constructor, copy assignment operator, move assignment operator, or destructor *think very carefully* how to deal with the other special member functions. 

Summary:

* Move semantics is not passed through
* For every class, the move constructor and move assignment operator are automatically generated (unless there is no way to do so)
* User-declaring a copy constructor, copy assignment operator, or destructor disables the automatic support of move semantics in a class. This does not impact the support in derived classes.
* User-declaring a move constructor or move assignment operator disables the automatic support of copy semantics in a class. You get move-only types (unless these special move member functions are deleted). 
* Never =delete a special move member function
* Do not declare a destructor if there is no specific need. There is no general need in classes derived from a polymorphic base class. 
