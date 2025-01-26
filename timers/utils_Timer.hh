#pragma once

#include <dzn/pump.hh>

#include "dzn-gen/timer.hh"

namespace utils {

// Inherit and implement the Dezyne-generated skeleton code
class Timer : public skel::Timer {

public:
    Timer(const dzn::locator& locator)
        : skel::Timer(locator)
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
        // Every timer needs its own unique ID, use the ptr to this object as its ID
        return reinterpret_cast<size_t>(this);
    }

    dzn::pump& pump;
};

}
