#pragma once

#include "InjectedGlueBase.hpp"
#include "dzn-gen/result.hh"

namespace example::glue::injected {

class ErrorHelper : public InjectedGlueBase<example::dezyne::utils::ErrorHelper> {
public:
    ErrorHelper()
    {
        m_port.in.AddMessage = [](Dzn::Error& e, std::string message) 
        {
            e.AddMessage(message);
        };
        m_port.in.Clear = [](Dzn::Error& e)
        {
            e = Dzn::Error{};
        };
    }
};

}