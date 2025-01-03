#pragma once

#include "dzn-gen/timer.hh"

class TimerFC : public skel::TimerFC {

public:
    TimerFC(const dzn::locator& locator)
        : skel::TimerFC(locator)
    {
    }

    void api_Start (std::chrono::milliseconds durationMs) override {

    }
    
    void api_Cancel () override {
        
    }

private:

};
