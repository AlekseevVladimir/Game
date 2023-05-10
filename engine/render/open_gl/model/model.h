#pragma once

#include <memory>
#include <concepts>
#include <variant>
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

	void draw();

	std::vector<Mesh::Texture> getTextures() { return m_textures; };

	unsigned int VAO, VBO;
	std::vector<Vertex> m_vertices;
	std::vector<Texture> m_textures;
protected:
};

class ElementsMesh : public Mesh 
{
public:
	const std::vector<unsigned int>& getIndices() {
		return m_indices;
	}

	// Mesh-������

	ElementsMesh(std::vector<Vertex> vertices, std::vector<Texture> textures, std::vector<unsigned int> indices) :
		Mesh(vertices, textures), m_indices(indices) {
		setupMesh();
	}

	std::vector<Mesh::Texture> getTextures() { return m_textures; };

	void draw();

	unsigned int EBO;
	std::vector<unsigned int> m_indices;
	void setupMesh();
private:
	// ������ ��� ����������
};



class Model
{
public:
	/*
	void setupModel() {
		for (Mesh& mesh : m_meshes) {
			//mesh.setupMesh();
		}
	}
	*/
	//virtual void setModelDataAndDraw(
	//	std::shared_ptr<Shader> shaderBase, GameObject* goPtr, GameObject* viewPointPtr) override;

	std::vector<std::variant<Mesh, ElementsMesh>> m_meshes;
private:
};

template<typename T>
concept MeshConcept = requires(T mesh)
{
	{mesh.getTextures()} -> std::same_as<std::vector<Mesh::Texture>>;

	//{mesh.draw()};
};

