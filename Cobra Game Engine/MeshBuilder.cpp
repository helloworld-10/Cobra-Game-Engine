//#include "MeshBuilder.h"
#pragma once
#include "MeshBuilder.h"

MeshComponent MeshBuilder::createMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, const std::string* texname) {
	MeshComponent mesh;
	mesh.verts = vertices;
	mesh.indices = indices;

	Texture tex;

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping/filtering options (on the currently bound texture object)

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load((*texname).c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		if (nrChannels == 4) {
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else {
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
	}
	
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	glActiveTexture(0);

	tex.id = texture;
	mesh.texture = tex;
	//Generate vertex buffer object
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	//Generate buffer for indices
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	//Generate Vertex array object
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	mesh.EBO = EBO;
	mesh.VBO = VBO;
	mesh.VAO = VAO;
	// ..:: Initialization code (done once (unless your object frequently changes)) :: ..
	// 1. bind Vertex Array Object
	glBindVertexArray(VAO);
	// 2. copy our vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	// 3. then set our vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	


	glBindVertexArray(0);
	return mesh;
}


MeshComponent MeshBuilder::loadFromObj(const std::string* filename, const std::string* texname) {
	std::ifstream in;
	in.open((*filename));
	if (!in.is_open()) {
		std::cout << "error in obj loading"<<std::endl;
	}
	std::string line;
	std::vector<double> pos;
	std::vector<double> tex;
	std::vector<double> nor;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	while (in.peek() != EOF && getline(in, line)) {
		int head = line.find(" ");
		std::string type = line.substr(0, head);
		//head++;
		if (type == "v") {
			for (int i = 0; i < 3; i++)
			{
				int nhead = line.find(" ", head+1);

				float coord = std::stof(line.substr(head, nhead-head));
				pos.push_back(coord);
				//nhead++;
				head = nhead;
			}
		}
		else if (type == "vt") {
			for (int i = 0; i < 2; i++)
			{
				int nhead = line.find(" ", head+1);
				tex.push_back(std::stof(line.substr(head, nhead-head)));
				//nhead++;
				head = nhead;
			}
		}
		else if (type == "vn") {
			for (int i = 0; i < 3; i++)
			{
				int nhead = line.find(" ", head+1);
				nor.push_back(std::stof(line.substr(head, nhead-head)));
				//nhead++;
				head = nhead;
			}
		}
		else if (type == "f") {
			if (vertices.empty()) {
				vertices.resize(pos.size() / 3);
			}
			for (int i = 0; i < 3; i++)
			{
				int nhead = line.find(" ", head+1);
				std::string face = line.substr(head+1, nhead-head);

				Vertex v;
				unsigned int inList;
				int _head = 0;
				for (int j = 0; j < 3; j++)
				{
					
					int _nhead = face.find("/", _head);
					if (_nhead < 0) {
						_nhead = face.length();
					}

					

					unsigned int vertVal = std::stoi(face.substr(_head, _nhead-_head));

					vertVal--;
					switch (j) {
					case 0:
						inList = vertVal;
						vertVal *= 3;
						v.position = { pos[vertVal],pos[vertVal + 1],pos[vertVal + 2] };
						break;
					case 1:
						vertVal *= 2;
						v.texCoord = { tex[vertVal],tex[vertVal+1] };
						break;
					case 2:
						vertVal *= 3;
						v.normal = { nor[vertVal],nor[vertVal + 1],nor[vertVal + 2] };
						break;
					}
					_head = _nhead+1;
				}
				vertices[inList] = v;
				indices.push_back(inList);
				//nhead++;
				head = nhead;
			}
		}
	}
	in.close();
	return MeshBuilder::createMesh(vertices,indices,texname);
}



