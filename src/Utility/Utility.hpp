#pragma once

#include <chrono>
#include <string>
#include <iostream>

template< typename T >
class Timer
{
public:
    Timer(std::string message)
        : m_message(std::move(message))
        //, m_unit(std::move(unit))
        , m_start(std::chrono::high_resolution_clock::now())
    {
    }
    
    ~Timer()
    {
        auto finish = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = finish - m_start;
        
        std::cout << m_message << std::chrono::duration_cast<std::chrono::duration<int, T>>(finish - m_start).count()
                    << '\n';
    }

private:
    std::string m_message;
    //std::chrono::duration_values m_unit;
    std::chrono::steady_clock::time_point m_start;
};