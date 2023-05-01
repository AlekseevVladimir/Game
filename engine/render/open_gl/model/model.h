#pragma once

#include <memory>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <engine/render/core/stb_image.h>


class Shader;
class GameObject;


class Mesh {
public:
	struct Vertex {
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
	};

	struct Texture {
		unsigned int id;
		std::string type;
		std::string path;
	};
	
	Mesh(std::vector<Vertex> vertices, std::vector<Texture> textures) :
		m_vertices(vertices)
		, m_textures(textures) {
		setupMesh();
	}

	void setupMesh();

	void drawMesh();

	unsigned int VAO, VBO;
	std::vector<Vertex> m_vertices;
	std::vector<Texture> m_textures;
protected:
};

class ElementsMesh : public Mesh {
public:
	const std::vector<unsigned int>& getIndices() {
		return m_indices;
	}

	// Mesh-данные

	ElementsMesh(std::vector<Vertex> vertices, std::vector<Texture> textures, std::vector<unsigned int> indices) :
		Mesh(vertices, textures), m_indices(indices) {
		setupMesh();
	}

	void drawMesh();

	unsigned int EBO;
	std::vector<unsigned int> m_indices;
	void setupMesh();
private:
	// Данные для рендеринга
};

class ModelBase
{
public:
	virtual ~ModelBase() = default;

	virtual void setModelDataAndDraw(std::shared_ptr<Shader> shaderBase, GameObject* goPtr) = 0;

};

template<typename TMesh>
class Model : public ModelBase {
public:
	/*
	void setupModel() {
		for (Mesh& mesh : m_meshes) {
			//mesh.setupMesh();
		}
	}
	*/
	virtual ~Model() = default;
	virtual void setModelDataAndDraw(std::shared_ptr<Shader> shaderBase, GameObject* goPtr) override;

	std::vector<TMesh> m_meshes;
private:
};
