#include "parser.hpp"
#include <fstream>
#include <string>
#include <iterator>
#include <sstream>
#include <memory>
#include <vector>
#include "parser.hpp"

auto
split_string(const std::string& subject) -> std::vector<std::string>
{
    std::istringstream stream{ subject };

    std::vector<std::string> container{ std::istream_iterator<std::string>{stream}, std::istream_iterator<std::string>{} };

    return container;
}

auto
split_string(std::string& subject, const std::string& delimiter) -> std::vector<std::string>
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

void Parser::ParseFile(const std::string& filename)
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
	    else if (split_str[0] == "vt") // texture coordinate
		{
			m_texture_coord.emplace_back(Point2f{std::stof(split_str[1]), std::stof(split_str[2])});
		}
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
                    continue;

                auto ordering = split_string(split_str[i], "/");
                // faces are ordered 1/1/1 (vertex, texture, normal)
                m_vertex_ordering.push_back(std::stoi(ordering[0]));

                if (!ordering[1].empty()) // may be empty
                    m_texture_coord_ordering.push_back(std::stoi(ordering[1]));


                if (!ordering[2].empty()) // may be empty
                    m_normal_ordering.push_back( std::stoi(ordering[2]));
            }
		}
	}
	infile.close();
}

std::unique_ptr<Mesh> Parser::ConstructMesh()
{
	//auto mesh_ptr = std::make_unique<Mesh>();

	std::vector<Triangle> triangles;

	triangles.reserve(m_vertex_ordering.size() / 3);

	for (size_t i = 0; i < m_vertex_ordering.size(); i += 3)
	{
		triangles.emplace_back(
				std::array<Point3f, 3> {
						m_vertex[m_vertex_ordering[i    ] - 1],
						m_vertex[m_vertex_ordering[i + 1] - 1],
						m_vertex[m_vertex_ordering[i + 2] - 1],
				},
				std::array<Normal3f, 3> {
						m_normals[m_normal_ordering[i    ] - 1],
						m_normals[m_normal_ordering[i + 1] - 1],
						m_normals[m_normal_ordering[i + 2] - 1],
				});
	}

	return std::make_unique<Mesh>(std::move(triangles));
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

std::unique_ptr<Mesh> Parser::GetMeshFromFile(const std::string& filename)
{
    ParseFile(filename);

    auto mesh_ptr = ConstructMesh();

    Reset(); // ready for next one

    return mesh_ptr;
}