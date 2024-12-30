#pragma once

#include <expected>
#include <future>
#include <memory>

#include "hardware.hpp"

// Forward declare so we don't pollute scope with dzn types
class AirlockImpl;

// Dependency injection for required
struct AirlockDependencies {
    IDoorHardware& doorInside;
    IDoorHardware& doorOutside;
    IVacuumHardware& vacuum;
};

class MyAirlock {
public:
    explicit MyAirlock(AirlockDependencies dependencies);
    ~MyAirlock();

    std::future<std::expected<void, std::string>> TransitionToInsideOpen();

    std::future<std::expected<void, std::string>> TransitionToOutsideOpen();

private:
    std::unique_ptr<AirlockImpl> impl;
};
