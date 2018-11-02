#include "Parser.h"
#include <fstream>
#include <string>
#include <iterator>
#include <sstream>
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

void Parser::LoadFile(const std::string& filename)
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
			m_vertex.emplace_back(Point3f(std::stof(split_str[1]),
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
			m_normals.emplace_back(
                Vec3f{
                    std::stof(split_str[1]),
				    std::stof(split_str[2]),
				    std::stof(split_str[3])});
		}
		else if (split_str[0] == "f") // face declaration, we assume only triangles are used
		{
            for (size_t i = 1; i < split_str.size(); i++) // skip first
            {
                if (split_str.size() > 4)
                    throw std::string("Loaded face is not a triangle!");

                auto ordering = split_string(split_str[i], "/");
                // faces are ordered 1/1/1 (vertex, texture, normal)
                m_vertex_ordering.push_back( std::stol(ordering[0]) );

                if (!ordering[1].empty()) // may be empty
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
std::unique_ptr<Mesh> Parser::ConstructMesh()
{
	auto mesh_ptr = std::make_unique<Mesh>();
	
	for (size_t i = 0; i < m_vertex_ordering.size(); i += 3)
	{
        // WARNING: .obj is 1-indexed
        /*
        auto polygon =
            std::make_unique<Triangle>( 
                 m_vertex[m_vertex_ordering[i] - 1],
                 m_vertex[m_vertex_ordering[i + 1] - 1],
                 m_vertex[m_vertex_ordering[i + 2] - 1],
                 m_normals[m_normal_ordering[i] - 1],
                 m_normals[m_normal_ordering[i + 1] - 1],
                 m_normals[m_normal_ordering[i + 2] - 1],
                 true, nullptr); */

        auto polygon =
			std::make_unique<Triangle>(
				std::array<Point3f, 3> {
                    m_vertex.at(m_vertex_ordering.at(i)     - 1),
                    m_vertex.at(m_vertex_ordering.at(i + 1) - 1),
                    m_vertex.at(m_vertex_ordering.at(i + 2) - 1),
				},
				std::array<Normal3f, 3> {
                    m_normals.at(m_normal_ordering.at(i)     - 1),
                    m_normals.at(m_normal_ordering.at(i + 1) - 1),
                    m_normals.at(m_normal_ordering.at(i + 2) - 1),
				}, true, nullptr);
	    
        mesh_ptr->AddPolygon(std::move(polygon));       
	}

	return mesh_ptr;
}

void Parser::Reset()
{
    m_vertex.clear();
    m_texture_coord.clear();
    m_normals.clear();
    m_faces.clear();

    m_vertex_ordering.clear();
    m_texture_coord_ordering.clear();
    m_normal_ordering.clear();
}

std::unique_ptr<Mesh> Parser::Parse(const std::string& filename)
{
    try
    {
        LoadFile(filename);
    } 
    catch (std::string& msg)
    {
        std::cout << msg << '\n';
    }
	
	auto mesh_ptr = ConstructMesh();

    Reset(); // ready for next one

	return mesh_ptr;
}
