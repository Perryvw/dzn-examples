#include "GlueDoor.hpp"

namespace glue {

GlueDoor::GlueDoor(dzn::pump& pump, IDoorHardware& hardware)
    : m_pump{pump}
    , m_hardware{hardware}
{
}

void GlueDoor::ConnectWith(IDoorExternal& port)
{
    m_port = port;

    // Bind calls from Dezyne to our glue:

    port.in.Open = [this](Dzn::Error& error) noexcept {
        try // Event handlers must NEVER throw
        {
            Open();
        } 
        catch (std::exception& e)
        {
            error.AddMessage(e.what());
            ::Result::Error;
        }
        return ::Result::Ok;
    };

    port.in.Close = [this](Dzn::Error& error) noexcept {
        try // Event handlers must NEVER throw
        {
            Close();
        } 
        catch (std::exception& e)
        {
            error.AddMessage(e.what());
            return ::Result::Error;
        }
        return ::Result::Ok;
    };

    // Subscribe to events from the hardware to send into Dezyne:

    m_hardware.ObserveStateChanged([this](IDoorHardware::DoorState newState) {
        if (newState == IDoorHardware::DoorState::Open)
        {
            m_pump([this] {
                m_port->get().out.Opened();
            });
        }
        else if (newState == IDoorHardware::DoorState::Closed)
        {
            m_pump([this] {
                m_port->get().out.Closed();
            });
        }
        else
        {
            m_pump([this, newState] {
                Dzn::Error error;
                error.AddMessage("Door state changed to unknown state: " + std::to_string(static_cast<int>(newState)));
                m_port->get().out.Fail(error);
            });
        }
    });

    // Make sure we connected everything correctly
    port.dzn_check_bindings();
}

void GlueDoor::Open()
{
    // Call hardware
    m_hardware.Open();
}

void GlueDoor::Close()
{
    // Call hardware
    m_hardware.Close();
}

}
