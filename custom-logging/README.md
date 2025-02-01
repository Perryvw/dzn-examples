By default, Dezyne will log its tracing information to the standard output. If you want to redirect this logging, this is possible by simply adding a custom ostream to the locator:

```cpp
dzn::runtime runtime{};
dzn::locator locator{};

locator.set(runtime);
locator.set<std::ostream>(customLoggingStream);

DezyneGeneratedSystem system{locator};
```

You can implement `customLoggingSteam` any way you want (as long as it inherits std::ostream). In [custom-dzn-logging.hpp](custom-dzn-logging.hpp) is an implementation taking a logging callback as constructor argument, which you could use to redirect your logging however required:

```cpp
CustomDznLogging customLoggingStream{[&logger](const std::string& message) {
    // Write to trace level of our logger
    logger.LogTrace(message);
}};

locator.set<std::ostream>(customLoggingStream);
```