#pragma once

#include <optional>

#include <dzn/pump.hh>

#include "hardware.hpp"
#include "dzn-gen/IDoorExternal.hh"

namespace glue {

class GlueDoor {
public:
    explicit GlueDoor(dzn::pump&, IDoorHardware&);

    // Connect a dezyne port to our hardware
    void ConnectWith(IDoorExternal& port);

private:
    // Implementations of event handlers
    void Open();
    void Close();

    dzn::pump& m_pump;

    IDoorHardware& m_hardware;
    std::optional<std::reference_wrapper<IDoorExternal>> m_port;
};

}
