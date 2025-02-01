This repository contains examples and often occurring patterns encountered while developing software with [Dezyne](https://dezyne.org/).

> Note: These examples were made using Dezyne 2.18.3 and C++23, and may require changes for other versions.

## Project Template

For a minimal project setup see the [MinimalTemplate directory](minimal-template).

## Examples

For a more complete example of a Dezyne project using the various patterns and utilities, see the [ExampleProject directory](example-project).

## Useful utilities and patterns

The other directories contain often-occuring patterns that might be useful while writing Dezyne code.

* [Result type and error information](call-result-or-error) for making functions able to succeed/fail, and pass around/extend error information
* [Timers](timers) for adding timing functionality and timeout fallbacks to your models
* [Custom logging stream](custom-logging) to redirect the Dezyne tracing to your own logging
* [Promise synchronization](promise-synchronization) to provide promises/futures and synchronize on the completion of long running async actions
