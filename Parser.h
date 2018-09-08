#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include "PolygonMesh.h"


class Parser
{


public:
    Parser();
    ~Parser();

    std::unique_ptr<PolygonMesh> parse( const std::string& filename );
	

private:
	void load_file(const std::string& filename);
	std::unique_ptr<PolygonMesh> construct_mesh();

    //std::ifstream m_file_in;
	std::vector<Vec3f> m_vertex;
	std::vector<Vec3f> m_texture_coord;
	std::vector<Vec3f> m_normals;
	std::vector<Vec3f> m_faces;
};

