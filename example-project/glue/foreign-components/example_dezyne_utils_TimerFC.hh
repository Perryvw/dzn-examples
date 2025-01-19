#pragma once

#include <dzn/pump.hh>

#include "dzn-gen/timer.hh"

namespace example::dezyne::utils {

class TimerFC : public skel::TimerFC {

public:
    TimerFC(const dzn::locator& locator)
        : skel::TimerFC(locator)
        , pump{locator.get<dzn::pump>()}
    {
    }

    void api_Start (std::chrono::milliseconds durationMs) override {
        // Use the Dezyne pump
        pump.handle(id(), durationMs.count(), [this]{
            api.out.Timeout();
        });
    }
    
    void api_Cancel () override {
        pump.remove(id());
    }

private:
    size_t id() const {
        return reinterpret_cast<size_t>(this);
    }

    dzn::pump& pump;
};

}
