#include "Parser.h"
#include <fstream>
#include <string>
#include <iterator>
#include <sstream>
#include "Vec2.h"
#include <memory>

std::vector<std::string> split_string( const std::string& subject )
{
    std::istringstream stream{ subject };

    std::vector<std::string> container{ std::istream_iterator<std::string>{stream}, std::istream_iterator<std::string>{} };

    return container;
}

std::vector<std::string> split_string( std::string& subject, const std::string& delimiter )
{
	size_t pos = 0;
	std::string token;

	std::vector<std::string> container;

	while ((pos = subject.find(delimiter)) != std::string::npos)
	{
		token = subject.substr(0, pos);
		container.emplace_back(token);

		subject.erase( 0, pos + delimiter.length() );
	}

	container.emplace_back(subject);

	return container;
}

void Parser::load_file(const std::string& filename)
{
	std::ifstream infile(filename);

	std::string line;

	while (getline(infile, line))
	{
		auto split_str = split_string(line);

        if (split_str.empty()) // checks will crash with empty vector
            continue;

		if (split_str[0] == "v") // vertex
		{
			m_vertex.push_back(Vecf(std::stof(split_str[1]),
				std::stof(split_str[2]),
				std::stof(split_str[3])));
		}
		/*
	    else if (split_str[0] == "vt") // texture coordinate
		{
			//m_texture_coord.emplace_back(Vec2f(std::stof(split_str[1]),
			//	std::stof(split_str[2])));

		} */
		else if (split_str[0] == "vn") // normal
		{
			m_normals.push_back(
                Vecf(
                    std::stof(split_str[1]),
				    std::stof(split_str[2]),
				    std::stof(split_str[3])));
		}
		else if (split_str[0] == "f") // face declaration, we assume only triangles are used
		{

            //auto separated_str = split_string(split_str, "/");
			std::vector<int> vertex_ordering;
            std::vector<int> texture_coord_ordering;
		    std::vector<int> normal_ordering;
            
			//std::vector<int> face;

            for (size_t i = 1; i < split_str.size(); i++) // skip first
            {
                if (split_str.size() > 4)
                    throw std::string("Loaded  face is not a triangle!");

                auto ordering = split_string(split_str[i], "/");
                // faces are ordered 1/1/1 (vertex, texture, normal)
                m_vertex_ordering.push_back( std::stol(ordering[0]) );
                if (!ordering[1].empty())
                    m_texture_coord_ordering.push_back( std::stol( ordering[1] ) );
      

                m_normal_ordering.push_back( std::stol( ordering[2] ) );
            }

            /*
			m_faces.push_back( // assuming triangles
				Vec3i(std::stol(split_str[1]),
					  std::stol(split_str[2]),
					  std::stol(split_str[3]))); */




			//m_faces.emplace_back(face);
		}
        
	}
}
std::unique_ptr<PolygonMesh> Parser::construct_mesh()
{
	auto mesh_ptr = std::make_unique<PolygonMesh>();
	
	for (size_t i = 0; i < m_vertex_ordering.size(); i += 3)
	{
        // WARNING: .obj is 1-indexed
        mesh_ptr->add_polygon(
            std::make_shared<Polygon>( // first element of each Vec3i
                m_vertex[m_vertex_ordering[i]-1],
                m_vertex[m_vertex_ordering[i + 1]-1],
                m_vertex[m_vertex_ordering[i + 2]-1],
                m_normals[m_normal_ordering[i]-1],
                m_normals[m_normal_ordering[i + 1]-1],
                m_normals[m_normal_ordering[i + 2]-1],
                true, Vecf(0.18f), Diffuse));
	}

	return mesh_ptr;
}

std::unique_ptr<PolygonMesh> Parser::parse( const std::string& filename )
{
    try
    {
        load_file(filename);
    } 
    catch (std::string& msg)
    {
        std::cout << msg << '\n';
    }
	
	auto mesh_ptr = construct_mesh();

	return mesh_ptr;
}
