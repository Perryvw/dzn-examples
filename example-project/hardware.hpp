#pragma once

#include <functional>

/* Not directly relevant to the example, just to serve as an example of how
 * other code in the software could plug into the dezyne sytem */

class IDoorHardware {
public:
    enum class DoorState {
        Unknow,
        Open,
        Closed
    };

    virtual void Open() = 0;
    virtual void Close() = 0;
    virtual void ObserveStateChanged(std::function<void(DoorState)>) = 0;
};

class IVacuumHardware {
public:
    enum class VacuumState {
        Unknown,
        Ambient,
        Vacuum
    };

    virtual void Evacuate() = 0;
    virtual void Vent() = 0;
    virtual void ObserveStateChanged(std::function<void(VacuumState)>) = 0;
};
