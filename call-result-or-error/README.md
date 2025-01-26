Calling a function that can fail is a common concept in programming. There are different ways of dealing with this, for example using exceptions, or dedicated types like [Rust's std::result](https://doc.rust-lang.org/std/result/) or [C++'s std::expected](https://en.cppreference.com/w/cpp/utility/expected).

Since Dezyne has neither exceptions nor structured return types, this pattern is implemented with an enum return and out parameters. The idea is to mimic structured result objects using the language functions available:

The short summary of how this works is:

```dzn
enum Result
{
    Ok,
    Error
};

// 'void' function that has to do something or fail
Result FunctionThatCanFail(out ErrorType error)
{
    if (<some condition>)
    {
        return Result.Ok;
    }
    else
    {
        error = <error value>;
        return Result.Error;
    }
}

// function that has a return value of ResultType
Result FunctionReturningDataThatCanFail(out ResultType result, out ErrorType error)
{
    if (<some condition>)
    {
        result = <result value>;
        return Result.Ok;
    }
    else
    {
        error = <error value>;
        return Result.Error;
    }
}
```

Dezyne abstracts away data, also for errors so these are usually stored as external types with helpers to modify them.

# Error objects

One common way to store and propagate error information is using some external data structure and an injected helper interface:

```dzn
extern Error $Dzn::Error$;

namespace utils {

// Definition of injectable interface containing helper functions to modify our error object
interface ErrorHelper
{
    // Add a message to the stack trace of the error
    in void AddMessage(inout Error error, in string message);

    behavior
    {
        on AddMessage: {}
    }
}

}
```

Usage of this pattern then looks like this in code:

```dzn
Result TryAB(out Error error) {

    // error already contains the failure info from a, add some more info
    if (a.Try(error) == Result.Error) {
        errorHelper.AddMessage(error, $"AB failed while trying A"$);
        return Result.Error;
    }

    if (b.Try(error) == Result.Error) {
        // error already contains the failure info from b, add some more info
        errorHelper.AddMessage(error, $"AB failed while trying B"$);
        return Result.Error;
    }

    return Result.Ok;
}
```

The C++ implementation of Dzn::Error can be found in `Error.hpp`.

An example of C++ code calling a Dezyne function returning a Result can be found in `example-usage.cpp`.

An example of C++ glue returning a Result and Error object into Dezyne can be found in `example-glue.cpp`
