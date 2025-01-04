#pragma once

#include "InjectedGlueBase.hpp"
#include "dzn-gen/logging.hh"

namespace example::glue::injected {

class Logging : public InjectedGlueBase<example::dezyne::utils::ILogging> {
public:
    Logging()
    {
        m_port.in.LogInfo = [](const std::string&) {

        };
        m_port.in.LogWarning = [](const std::string&) {

        };
        m_port.in.LogError = [](const std::string&) {

        };
    }
};

}