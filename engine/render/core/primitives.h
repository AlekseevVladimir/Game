#pragma once
#include <string>
#include <Math/Vector2.h>
#include <Math/Vector3.h>


struct Vertex {
	Vector3<float> Position;
	Vector3<float> Normal;
	Vector2<float> TexCoords;
};

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};