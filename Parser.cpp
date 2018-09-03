#include "Parser.h"
#include <fstream>
#include <string>
#include <iterator>
#include <sstream>

std::vector<std::string> split_string( const std::string& subject )
{
    std::istringstream stream{ subject };

    std::vector<std::string> container{ std::istream_iterator<std::string>{stream}, std::istream_iterator<std::string>{} };

    return container;
}


std::unique_ptr<PolygonMesh> Parser::parse( const std::string& filename )
{
    std::ifstream infile( filename );

    std::string line;

    std::vector<float> vertx;
    std::vector<float> vt_coord;
    std::vector<float> normals;

    //std::vector<float> numbers;
    // load one line at a time
    // istringstream checking first value

    while ( getline( infile, line ) )
    {
        auto split_str = split_string( line );

        if ( split_str[0] == "v" ) // vertex
        {
            
        }
        else if ( split_str[0] == "vt" ) // texture coordinate
        {
            

        }
        else if ( split_str[0] == "vn" ) // normal
        {
            
        }
        else if ( split_str[0] == "f" ) // face declaration
        {
            
        }

    }

}
