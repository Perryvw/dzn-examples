#pragma once

namespace glue::injected {

template<typename Port>
class InjectedGlueBase {
public:
    InjectedGlueBase()
        : m_port{{}}
    {
    }

    Port& port() {
        return m_port;
    }

protected:
    Port m_port;
};

}
