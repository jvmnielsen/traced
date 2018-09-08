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

	return container;
}

void Parser::load_file(const std::string& filename)
{
	std::ifstream infile(filename);

	std::string line;

	while (getline(infile, line))
	{
		auto split_str = split_string(line);

		if (split_str[0] == "v") // vertex
		{
			m_vertex.emplace_back(Vec3f(std::stof(split_str[1]),
				std::stof(split_str[2]),
				std::stof(split_str[3])));
		}
		else if (split_str[0] == "vt") // texture coordinate
		{
			m_texture_coord.emplace_back(Vec2f(std::stof(split_str[1]),
				std::stof(split_str[2])));

		}
		else if (split_str[0] == "vn") // normal
		{
			m_normals.emplace_back(Vec3f(std::stof(split_str[1]),
				std::stof(split_str[2]),
				std::stof(split_str[3])));
		}
		else if (split_str[0] == "f") // face declaration, we assume only triangles are used
		{
			//std::vector<std::vector<int>> face;
			std::vector<std::string> temp;
			for (size_t i = 1; i < split_str.size(); ++i) // skip first
			{
				temp = split_string(split_str[i], "/");

				m_faces.emplace_back(Vec3f(std::stol(temp[0]),
					std::stol(temp[1]),
					std::stol(temp[2])));

			}
		}
	}
}
std::unique_ptr<PolygonMesh> Parser::construct_mesh()
{
	auto mesh_ptr = std::make_unique<PolygonMesh>();
	for (size_t i = 0; i < m_faces.size(); i += 3)
	{
		mesh_ptr->add_polygon(Polygon(
			m_vertex[m_faces[i][0]],
			m_vertex[m_faces[i + 1][0]],
			m_vertex[m_faces[i + 2][0]], true));
	}

	return mesh_ptr;
}

std::unique_ptr<PolygonMesh> Parser::parse( const std::string& filename )
{
	load_file(filename);
	auto mesh_ptr = construct_mesh();

	return mesh_ptr;
}
