#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include "PolygonMesh.h"


class Parser
{


public:
    //Parser();
    //~Parser() = default;

    std::unique_ptr<PolygonMesh> parse( const std::string& filename );
	

private:
	void load_file(const std::string& filename);
	std::unique_ptr<PolygonMesh> construct_mesh();

    //std::ifstream m_file_in;
	std::vector<Vec3f> m_vertex;
	std::vector<Vec3f> m_texture_coord;
	std::vector<Vec3f> m_normals;
	std::vector<Vec3i> m_faces;

    std::vector<int> m_vertex_ordering;
    std::vector<int> m_texture_coord_ordering;
    std::vector<int> m_normal_ordering;
   
};

