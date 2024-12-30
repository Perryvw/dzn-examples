#include "airlock.hpp"

#include <dzn/locator.hh>
#include <dzn/runtime.hh>
#include <dzn/pump.hh>

#include "dzn-gen/AirlockSystem.hh"

#include "glue/GlueDoor.hpp"
#include "glue/GlueVacuum.hpp"

#include "glue/injected/ErrorHelper.hpp"

#include "hardware.hpp"

class AirlockImpl
{
public:
    explicit AirlockImpl(AirlockDependencies dependencies)
        : m_doorInside{m_dzn_pump, dependencies.doorInside},
        m_doorOutside{m_dzn_pump, dependencies.doorOutside},
        m_vacuum{m_dzn_pump, dependencies.vacuum}
    {
        // Add runtime and pump to locator
        m_dzn_locator.set(m_dzn_runtime);
        m_dzn_locator.set(m_dzn_pump);

        // Add injected glue to locator
        m_dzn_locator.set(m_errorHelper.port());

        // Construct dezyne system
        m_system = std::make_unique<AirlockSystem>(m_dzn_locator);

        // Connect glue objects to our dezyne system
        m_doorInside.ConnectWith(m_system->doorInside);
        m_doorOutside.ConnectWith(m_system->doorOutside);
        m_vacuum.ConnectWith(m_system->vacuum);

        dzn::check_bindings(*m_system);
    }

    std::future<std::expected<void, std::string>> TransitionToInsideOpen()
    {
        std::shared_future<PromiseFC::PromiseResult> future;

        dzn::shell(m_dzn_pump, [this, &future]() {
            m_system->api.in.TransitionToInsideOpen(future);
        });

        return std::async(std::launch::async, [future = std::move(future)] -> std::expected<void, std::string> {
            auto r = future.get();
            if (r.has_value()) {
                return {};
            } else {
                return std::unexpected{r.error().ToString()};
            }
        });
    }

    std::future<std::expected<void, std::string>> TransitionToOutsideOpen()
    {
        std::shared_future<PromiseFC::PromiseResult> future;

        dzn::shell(m_dzn_pump, [this, &future]()
                   { m_system->api.in.TransitionToOutsideOpen(future); });

        return std::async(std::launch::async, [future = std::move(future)] -> std::expected<void, std::string> {
            auto r = future.get();
            if (r.has_value()) {
                return {};
            } else {
                return std::unexpected{r.error().ToString()};
            }
        });
    }

private:
    dzn::locator m_dzn_locator;
    dzn::runtime m_dzn_runtime;
    dzn::pump m_dzn_pump;

    std::unique_ptr<AirlockSystem> m_system;

    glue::GlueDoor m_doorInside;
    glue::GlueDoor m_doorOutside;
    glue::GlueVacuum m_vacuum;

    glue::injected::ErrorHelper m_errorHelper;
};

MyAirlock::MyAirlock(AirlockDependencies dependencies)
    : impl{std::make_unique<AirlockImpl>(dependencies)}
{
}

MyAirlock::~MyAirlock() = default;

std::future<std::expected<void, std::string>> MyAirlock::TransitionToInsideOpen()
{
    return impl->TransitionToInsideOpen();
}

std::future<std::expected<void, std::string>> MyAirlock::TransitionToOutsideOpen()
{
    return impl->TransitionToOutsideOpen();
}
