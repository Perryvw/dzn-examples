#pragma once

#include "dzn-gen/promise.hh"

namespace utils {

class PromiseFC : public skel::PromiseFC {

public:
    using PromiseResult = std::expected<std::any, Dzn::Error>;

    PromiseFC(const dzn::locator& locator)
        : skel::PromiseFC(locator)
    {
    }

    void api_Start () override {
        // Start the action, create a promise we have to resolve later
        m_promise = std::promise<PromiseResult>{};
        // Create a shared future we can share by copying
        m_future = m_promise.get_future().share();
        m_pending = true;
    }
    bool api_IsPending () override {
        return m_pending;
    }

    void api_CreateResolved (std::shared_future<PromiseResult>& future) override {
        // Create an already-resolved future
        std::promise<PromiseResult> p{};
        p.set_value(std::any{});
        future = p.get_future().share();
    }

    void api_CreateResolvedData (std::any data, std::shared_future<PromiseResult>& future) override {
        // Create an already-resolved future with data
        std::promise<PromiseResult> p{};
        p.set_value(data);
        future = p.get_future().share();
    }

    void api_CreateFailed (Dzn::Error error, std::shared_future<PromiseResult>& future) override {
        // Create an already-failed future
        std::promise<PromiseResult> p{};
        p.set_value(std::unexpected{error});
        future = p.get_future().share();
    }

    void api_CreatePending (std::shared_future<PromiseResult>& future) override {
        // Create a pending future by copying our shared future
        future = m_future;
    }

    void api_Resolve () override {
        // Resolve our promise, unblocking any waiting clients
        m_promise.set_value(std::any{});
        m_pending = false;
    }
    
    void api_ResolveData (std::any data) override {
        // Resolve our promise with data, unblocking any waiting clients
        m_promise.set_value(data);
        m_pending = false;
    }
    
    void api_Fail (Dzn::Error error) override {
        // Reject our promise with an error, unblocking any waiting clients
        m_promise.set_value(std::unexpected{error});
        m_pending = false;
    }

private:
    std::promise<PromiseResult> m_promise;
    std::shared_future<PromiseResult> m_future;
    bool m_pending;
};

}
