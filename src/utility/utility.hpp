#pragma once

#include <chrono>
#include <string>
#include <iostream>
#include <mutex>


static std::mutex message_mutex;
// Execution time of scope in milliseconds
class Timer
{
public:
    explicit Timer(std::string message)
        : m_message(std::move(message))
        , m_start(std::chrono::high_resolution_clock::now()) {
    }

    auto WriteMessage(std::string_view message) noexcept -> void
    {
        std::scoped_lock<std::mutex> guard(message_mutex);
        auto finish = std::chrono::high_resolution_clock::now();
        std::cout << m_message << std::chrono::duration_cast<std::chrono::duration<int, std::milli>>(finish - m_start).count()
            << message.data();
    }

    ~Timer()
    {
        WriteMessage(std::string_view{" milliseconds\n"});
    }

private:
    //std::mutex m_mutex;
    std::string m_message;
    std::chrono::steady_clock::time_point m_start;
};