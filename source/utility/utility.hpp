#pragma once

#include <chrono>
#include <string>
#include <iostream>

// Execution time of scope in milliseconds
class Timer
{
public:
    explicit Timer(std::string message)
        : m_message(std::move(message))
        , m_start(std::chrono::high_resolution_clock::now()) {
    }
    
    ~Timer()
    {
        auto finish = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = finish - m_start;
        
        std::cout << m_message << std::chrono::duration_cast<std::chrono::duration<int, std::milli>>(finish - m_start).count()
                    << " milliseconds\n";
    }

private:
    std::string m_message;
    std::chrono::steady_clock::time_point m_start;
};