This project contains a complete example project using Dezyne and C++. It integrates many of the patterns described in this repository, integrated into a building and working C++ project, including tests.

This project creates a library containing the business logic to safely operate the hardware of a vacuum loadlock. All business logic is written in Dezyne, with a little C++ wrapper around it to provide a simple interface, hiding the implementation details. This library would be included into a larger C++ application also controlling other parts of the machine hardware. Other components could use their own Dezyne implementation communicating to this component via the C++ interfaces. Alternatively, bigger components could have much larger parts of their behavior in a single Dezyne system, increasing safety (and complexity). The choice is up to the software architecture.

## Contents

* **dzn-src**: Source files in Dezyne containing the main business logic
* **glue**: C++ source files for glue to connect the Dezyne generated code to the C++ environment
* **airlock.hpp/airlock.cpp**: The main entry point of this library, containing the top-level c++ wrapper to be used by clients.
* **test**: GoogleTest unit tests testing the library, mocking hardware.

## Used patterns:

* Result/Error pattern
* Armoring of interfaces to be robust to unexpected behavior
* Promise pattern for asynchronous actions
* Custom logging sink for Dezyne tracing
* Timers
