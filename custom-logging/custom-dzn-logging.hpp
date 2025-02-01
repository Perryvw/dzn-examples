#pragma once

#include <array>
#include <functional>
#include <streambuf>
#include <string>

constexpr int BUFFER_SIZE = 1024;

using LogFunction_t = std::function<void(const std::string&)>;

class CustomDznLogging : public std::ostream
{
    class CustomDznLoggingBuf : public std::streambuf
    {
    public:
        explicit CustomDznLoggingBuf(const LogFunction_t& logFunc)
            : m_buf({0})
            , m_index(0)
            , m_logFunc(logFunc)
        {        
        }

    protected:
        int sync() override
        {
            return (m_index > 0) ? internalSync() : 0;
        }

        std::char_traits<char>::int_type overflow(std::char_traits<char>::int_type c) override
        {
            switch (c)
            {
                case std::char_traits<char>::eof():
                    return 0;

                case '\n':
                    internalSync();
                    break;

                default:
                    m_buf[m_index] = std::char_traits<char>::to_char_type(c);
                    m_index++;
                    if (m_index == (BUFFER_SIZE - 1))
                    {
                        internalSync();
                    }
                    break;
            }

            return c;
        }

    private:
        int internalSync()
        {
            m_buf[m_index] = '\0';
            m_index = 0;

            // Write to custom log function
            m_logFunc(m_buf.data());

            return 0;
        }

        std::array<char, BUFFER_SIZE> m_buf;
        int m_index;

        const LogFunction_t m_logFunc;
    };

public:
    explicit CustomDznLogging(const LogFunction_t& logFunc)
        : std::ostream{&m_buf}
        , m_buf{logFunc}
    {
    }

private:
    CustomDznLoggingBuf m_buf;
};
