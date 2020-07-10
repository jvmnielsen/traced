#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include "../geometry/mesh.hpp"

class Parser
{
public:
	//auto GetMeshFromFile(const std::string& filename) -> std::unique_ptr<Mesh>;

	auto construct_mesh_from_file(const std::string& filename, std::shared_ptr<Material> material, const Transform& transform) -> std::unique_ptr<Mesh>;

private:
	auto parse_file(const std::string& filename) -> void;
	auto construct_mesh(std::shared_ptr<Material> material, const Transform& transform) -> std::unique_ptr<Mesh>;
    auto reset() -> void;

	std::vector<Point3f> m_vertex;
	std::vector<Point2f> m_texture_coord;
	std::vector<Vec3f> m_normals;
	std::vector<Vec3i> m_faces;

    std::vector<int> m_vertex_ordering;
    std::vector<int> m_texture_coord_ordering;
    std::vector<int> m_normal_ordering;
};

