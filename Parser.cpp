#include "Parser.h"
#include <fstream>
#include <string>
#include <iterator>
#include <sstream>
#include "Vec2.h"

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
		auto split_str = split_string(line, " ");

		if (split_str[0] == "v") // vertex
		{
			m_vertex.push_back(Vec3f(std::stof(split_str[1]),
				std::stof(split_str[2]),
				std::stof(split_str[3])));
		}
		else if (split_str[0] == "vt") // texture coordinate
		{
			//m_texture_coord.emplace_back(Vec2f(std::stof(split_str[1]),
			//	std::stof(split_str[2])));

		}
		else if (split_str[0] == "vn") // normal
		{
			m_normals.push_back(Vec3f(std::stof(split_str[1]),
				std::stof(split_str[2]),
				std::stof(split_str[3])));
		}
		else if (split_str[0] == "f") // face declaration, we assume only triangles are used
		{
			//std::vector<Vec3i> face;
			//std::vector<int> face;
			
			m_faces.push_back( // assuming triangles
				Vec3i(std::stol(split_str[1]),
					  std::stol(split_str[2]),
					  std::stol(split_str[3])));

			
			//m_faces.emplace_back(face);
		}
	}
}
std::unique_ptr<PolygonMesh> Parser::construct_mesh()
{
	auto mesh_ptr = std::make_unique<PolygonMesh>();
	
	for (auto& face : m_faces)
	{
		mesh_ptr->add_polygon(std::make_shared<Polygon>( // first element of each Vec3i
			m_vertex[face[0]-1], // WARNING: .obj is 1-indexed
			m_vertex[face[1]-1],
			m_vertex[face[2]-1], true)); 
	}

	return mesh_ptr;
}

std::unique_ptr<PolygonMesh> Parser::parse( const std::string& filename )
{
	load_file(filename);
	auto mesh_ptr = construct_mesh();

	return mesh_ptr;
}
