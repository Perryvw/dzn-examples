#pragma once

#include <string>

/* Not directly relevant to the example, just to serve as an example of how
 * an existing logging interface might be integrated into dezyne code */
class ILogger {
public:
    virtual void Info(const std::string& message) const = 0;
    virtual void Warning(const std::string& message) const = 0;
    virtual void Error(const std::string& message) const = 0;
};