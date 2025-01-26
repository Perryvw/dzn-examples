#pragma once

#include <ranges>
#include <sstream>
#include <string>
#include <vector>

namespace Dzn {
    class Error {
    public:
        void AddMessage(const std::string& message) {
            messages.emplace_back(message);
        }

        std::string ToString() const {
            std::stringstream ss;

            // Reverse iterate, first message is on top
            for (int i = messages.size() - 1; i >= 0; i--) {
                ss << messages[i];
                if (i > 0) {
                    ss << std::endl;
                }
            }

            return ss.str();
        }
    private:
        std::vector<std::string> messages;
    };
}