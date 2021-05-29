# Callable_cpp
A c++ `Callable_c<return_type, args...>` concept. It's 40 lines of the most gut-wrenchingly terse template metaprogramming you could imagine. I'm putting this out there hoping to spare someone a few hours of intense pain. Works with function pointers/references, lambdas, std::bind, functors...
Essentially, if `declval<Whatever_callable_type>()(Arg_types...)` is well formed and returns `Return_type`, then `Callable_c<Whatever_callable_type, Return_type, Arg_types...>` is true.

Usage: just include the header and use `Callable_c`: the arguments are

- the type to check, omitted when declaring, say, a function parameter type, and necessary when using `Callable_c` as a boolean expression
- the return type
- the argument types

AFAIK, the `Return_type(Arg_types...)` notation is not possible with concepts as it would require partial specializations, which AFAIK aren't a thing in concepts. If there's a way to do this, feel free to add a pull request.
