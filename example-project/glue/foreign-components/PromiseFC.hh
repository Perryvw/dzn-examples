#pragma once

#include "dzn-gen/promise.hh"

class PromiseFC : public skel::PromiseFC {

public:
    using PromiseResult = std::expected<std::any, Dzn::Error>;

    PromiseFC(const dzn::locator& locator)
        : skel::PromiseFC(locator)
    {
    }

    void api_Start () override {
        m_promise = std::promise<PromiseResult>{};
        m_future = m_promise.get_future();
        m_pending = true;
    }
    bool api_IsPending () override {
        return m_pending;
    }

    void api_CreateResolved (std::shared_future<PromiseResult>& future) override {
        std::promise<PromiseResult> p{};
        p.set_value(std::any{});
        future = p.get_future().share();
    }

    void api_CreateResolvedData (std::any data, std::shared_future<PromiseResult>& future) override {
        std::promise<PromiseResult> p{};
        p.set_value(data);
        future = p.get_future().share();
    }

    void api_CreateFailed (Dzn::Error error, std::shared_future<PromiseResult>& future) override {
        std::promise<PromiseResult> p{};
        p.set_value(std::unexpected{error});
        future = p.get_future().share();
    }

    void api_CreatePending (std::shared_future<PromiseResult>& future) override {
        future = m_future.share();
    }

    void api_Resolve () override {
        m_promise.set_value(std::any{});
        m_pending = false;
    }
    
    void api_ResolveData (std::any data) override {
        m_promise.set_value(data);
        m_pending = false;
    }
    
    void api_Fail (Dzn::Error error) override {
        m_promise.set_value(std::unexpected{error});
        m_pending = false;
    }

private:
    std::promise<PromiseResult> m_promise;
    std::future<PromiseResult> m_future;
    bool m_pending;
};
