#pragma once

#include "InjectedGlueBase.hpp"
#include "dzn-gen/logging.hh"

#include "logging.hpp"

namespace example::glue::injected {

class Logging : public InjectedGlueBase<example::dezyne::utils::ILogging> {
public:
    explicit Logging(ILogger& logger)
    {
        m_port.in.LogInfo = [&logger](const std::string& message) {
            logger.Info(message);
        };
        m_port.in.LogWarning = [&logger](const std::string& message) {
            logger.Warning(message);
        };
        m_port.in.LogError = [&logger](const std::string& message) {
            logger.Error(message);
        };
    }
};

}