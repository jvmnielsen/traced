#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include "Mesh.h"

class Parser
{
public:
	std::unique_ptr<Mesh> GetMeshFromFile(const std::string& filename);

private:
	void ParseFile(const std::string& filename);
	std::unique_ptr<Mesh> ConstructMesh();
    void Reset();

	std::vector<Point3f> m_vertex;
	std::vector<Point2f> m_texture_coord;
	std::vector<Vec3f> m_normals;
	std::vector<Vec3i> m_faces;

    std::vector<int> m_vertex_ordering;
    std::vector<int> m_texture_coord_ordering;
    std::vector<int> m_normal_ordering;
};

