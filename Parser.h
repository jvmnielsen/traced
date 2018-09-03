#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include "PolygonMesh.h"


class Parser
{


public:
    Parser();
    ~Parser();

    std::unique_ptr<PolygonMesh> parse( const std::string& filename );

private:
    //std::ifstream m_file_in;
    
};

