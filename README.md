# Callable_cpp
## Description
A c++ `Callable_c<return_type, args...>` concept. It's 40 lines of the most gut-wrenchingly terse template metaprogramming you could imagine. I'm putting this out there hoping to spare someone a few hours of intense pain. Works with function pointers/references, lambdas, std::bind, functors...
Essentially, if `declval<Whatever_callable_type>()(Arg_types...)` is well formed and returns `Return_type`, then `Callable_c<Whatever_callable_type, Return_type, Arg_types...>` is true.
## Usage
Just include the header and use `Callable_c`: the arguments are

- the type to check, omitted when declaring, say, a function parameter type, and necessary when using `Callable_c` as a boolean expression
- the return type
- the argument types
## Examples
#### With regular functions:
``` c++
int test_function(int) { return 0; }

std::cout << Callable_c<decltype(test_function), int(int)> << std::endl;   // outputs: true
std::cout << Callable_c<decltype(test_function), int(char)> << std::endl;  // outputs: true (implicitly convertible)
std::cout << Callable_c<decltype(test_function), int(void*)> << std::endl; // outputs: false (void* is not implicitly convertible to int)
```
#### With `std::function`
``` c++
std::function<void(int, int, std::string)> test_std_function;

std::cout << Callable_c<decltype(test_std_function), void(int, int, std::string)> << std::endl; // outputs: true
std::cout << Callable_c<decltype(test_std_function), char(int, int, std::string)> << std::endl; // outputs: false (char is not equal to int)
std::cout << Callable_c<decltype(test_std_function), int(int, int, std::string)> << std::endl;  // outputs: false (void is not equal to int)
```
#### With `std::bind`
``` c++
auto test_bind = std::bind(test_function, std::placeholders::_1);

std::cout << Callable_c<decltype(test_bind), int(int)> << std::endl;   // outputs: true
std::cout << Callable_c<decltype(test_bind), int(char)> << std::endl;  // outputs: true (implicitly convertible)
std::cout << Callable_c<decltype(test_bind), int(void*)> << std::endl; // outputs: false (void* is not implicitly convertible to int)
```
## Differences from `std::invocable`
What changes is that `Callable_c` also checks the return type. Although, unlike with arguments, which are accepted even if the type is implicitly convertible, the return type has to be exactly the same. If you want, you can just go to what is now line 27 (I know for a fact that I will forget to update this readme) and switch the `std::is_same_v` with `std::is_convertible_v`. Or use any other condition.
