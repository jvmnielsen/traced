#include <chrono>
#include <string>
#include <iostream>

class Timer
{
public:
    Timer(const std::string& message) : m_message(message), m_start(std::chrono::high_resolution_clock::now())
    {
    }
    
    ~Timer()
    {
        auto finish = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = finish - m_start;
        
        std::cout << m_message << duration.count();
    }

private:
    std::string m_message;
    std::chrono::steady_clock::time_point m_start;
};