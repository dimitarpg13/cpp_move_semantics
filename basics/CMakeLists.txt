cmake_minimum_required(VERSION 3.20)

# set the project name
project(CppMoveSemanticsBasics VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add the executable
add_executable(motive03 motiv03.cpp)
add_executable(motiv11 motiv11.cpp)
add_executable(customer1 customer1.cpp)
add_executable(customer2 customer2.cpp)
add_executable(customerimpl customerimpl.cpp)
add_executable(ruleofthree ruleofthree.cpp)
add_executable(ruleoffive ruleoffive.cpp)
add_executable(person person.cpp)
add_executable(personnospecialmembers personnospecialmembers.cpp)
add_executable(personcopydefault personcopydefault.cpp)
add_executable(personmovedefault personmovedefault.cpp)
add_executable(personmove personmove.cpp)
add_executable(invoicewithcustomer invoicewithcustomer.cpp)
