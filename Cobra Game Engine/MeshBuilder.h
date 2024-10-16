#pragma once
#include <vector>
#include <glad/glad.h>
#include "vendor/stb_image.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vec3.hpp>
#include "Component.h"

class MeshBuilder {
public:
	static MeshComponent createMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, const std::string* texname);
	static MeshComponent loadFromObj(const std::string* filename, const std::string* texname);
};

