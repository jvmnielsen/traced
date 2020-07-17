#pragma once

#include "../geometry/mesh.hpp"

#include <graphics-math.hpp>

#include <fstream>
#include <iostream>
#include <vector>


namespace tr {

class Parser
{
public:
	//auto GetMeshFromFile(const std::string& filename) -> std::unique_ptr<Mesh>;

	auto construct_mesh_from_file(const std::string& filename, std::shared_ptr<Material> material, const gm::Transform& transform) -> std::unique_ptr<Mesh>;

private:
	auto parse_file(const std::string& filename) -> void;
	auto construct_mesh(std::shared_ptr<Material> material, const gm::Transform& transform) -> std::unique_ptr<Mesh>;
    auto reset() -> void;

	std::vector<gm::Point3f> m_vertex;
	std::vector<gm::Point2f> m_texture_coord;
	std::vector<gm::Vec3f> m_normals;
	std::vector<gm::Vec3i> m_faces;

    std::vector<int> m_vertex_ordering;
    std::vector<int> m_texture_coord_ordering;
    std::vector<int> m_normal_ordering;
};

}