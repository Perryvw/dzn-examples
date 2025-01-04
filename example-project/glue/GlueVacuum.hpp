#pragma once

#include <optional>

#include <dzn/pump.hh>

#include "hardware.hpp"
#include "dzn-gen/IVacuumExternal.hh"

namespace example::glue {

class GlueVacuum {
public:
    explicit GlueVacuum(dzn::pump&, IVacuumHardware&);

    // Connect a dezyne port to our hardware
    void ConnectWith(example::dezyne::IVacuumExternal& port);

private:
    // Implementations of event handlers
    void Evacuate();
    void Vent();

    dzn::pump& m_pump;
    IVacuumHardware& m_hardware;
    std::optional<std::reference_wrapper<example::dezyne::IVacuumExternal>> m_port;
};

}
