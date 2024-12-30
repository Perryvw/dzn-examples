#include "GlueVacuum.hpp"

namespace glue {

GlueVacuum::GlueVacuum(dzn::pump& pump, IVacuumHardware& hardware)
    : m_pump{pump}
    , m_hardware{hardware}
{
}

void GlueVacuum::ConnectWith(IVacuum &port)
{
    m_port = port;
    
    // Bind calls from Dezyne to our glue:

    port.in.Evacuate = [this](Dzn::Error& error) noexcept {
        try // Event handlers must NEVER throw
        {
            Evacuate();
        } 
        catch (std::exception& e)
        {
            error.AddMessage(e.what());
            return ::Result::Error;
        }
        return ::Result::Ok;
    };

    port.in.Vent = [this](Dzn::Error& error) noexcept {
        try // Event handlers must NEVER throw
        {
            Vent();
        } 
        catch (std::exception& e)
        {
            error.AddMessage(e.what());
            return ::Result::Error;
        }
        return ::Result::Ok;
    };

    // Subscribe to events from the hardware to send into Dezyne:

    m_hardware.ObserveStateChanged([this](IVacuumHardware::VacuumState newState) {
        if (newState == IVacuumHardware::VacuumState::Ambient)
        {
            m_pump([this] {
                m_port->get().out.Vented();
            });
        }
        else if (newState == IVacuumHardware::VacuumState::Vacuum)
        {
            m_pump([this] {
                m_port->get().out.Evacuated();
            });
        }
        else
        {
            m_pump([this, newState] {
                Dzn::Error error;
                error.AddMessage("Vacuum state changed to unknown state: " + std::to_string(static_cast<int>(newState)));
                m_port->get().out.Fail(error);
            });
        }
    });

    // Make sure we connected everything correctly
    port.dzn_check_bindings();
}

void GlueVacuum::Evacuate()
{
    // Call hardware
    m_hardware.Evacuate();
}

void GlueVacuum::Vent()
{
    // Call hardware
    m_hardware.Vent();
}

}
