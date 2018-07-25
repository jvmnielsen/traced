#pragma once
#include <cstddef>

struct Pixel
{
    Pixel();
    Pixel( unsigned char r, 
           unsigned char g, 
           unsigned char b, 
           unsigned char a )
        : m_r( r )
        , m_g( g )
        , m_b( b )
        , m_a( a )
    {
    }
    
    ~Pixel();

    unsigned char m_r;
    unsigned char m_g;
    unsigned char m_b;
    unsigned char m_a;

};

