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
            for (auto& message : std::ranges::reverse_view(messages))
            {
                ss << message << std::endl;
            }

            return ss.str();
        }
    private:
        std::vector<std::string> messages;
    };
}
