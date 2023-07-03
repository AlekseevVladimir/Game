#include "model_loader.h"
#include <filesystem>
#include <engine/render/core/textures_ctrl.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

std::vector<Mesh::Texture> loadMaterialTextures(
	aiMaterial* mat, aiTextureType type, std::string typeName) 
{
	std::vector<Mesh::Texture> textures;
	TexturesCtrl& texCtrl = TexturesCtrl::getInstance();
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		Mesh::Texture texture;
		//TODO fix paths ?
		texture.id = texCtrl.loadImage(str.C_Str(), "../../resources/models/troll/");
		texture.type = typeName;
		texture.path = str.C_Str();
		textures.push_back(texture);
	}
	return textures;
}


ElementsMesh processMesh(aiMesh* mesh, const aiScene* scene) 
{

	std::vector<Mesh::Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Mesh::Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Mesh::Vertex vertex;
		// Обрабатываем координаты вершин, нормали и текстурные координаты
		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;

		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;

		vector.x = mesh->mTangents[i].x;
		vector.y = mesh->mTangents[i].y;
		vector.z = mesh->mTangents[i].z;
		vertex.Tangent = vector;
		
		vector.x = mesh->mBitangents[i].x;
		vector.y = mesh->mBitangents[i].y;
		vector.z = mesh->mBitangents[i].z;
		vertex.BiTangent = vector;
		if (mesh->mTextureCoords[0]) // если меш содержит текстурные координаты
		{
			glm::vec2 vec;

			// Вершина может содержать до 8 различных текстурных координат. Мы предполагаем, что мы не будем использовать модели,
			// в которых вершина может содержать несколько текстурных координат, поэтому мы всегда берем первый набор (0)
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
		{
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		}
		vertices.push_back(vertex);

	}
	// Обрабатываем индексы
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}
	// Обрабатываем материал
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<Mesh::Texture> diffuseMaps = loadMaterialTextures(material,
			aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<Mesh::Texture> specularMaps = loadMaterialTextures(material,
			aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

		
		std::vector<Mesh::Texture> normalMaps = loadMaterialTextures(material,
			aiTextureType_HEIGHT, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

		std::vector<Mesh::Texture> heightMaps = loadMaterialTextures(material,
			aiTextureType_DISPLACEMENT, "texture_height");
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
		
	}

	return ElementsMesh(vertices, textures, indices);
}


void processNode(aiNode* node, const aiScene* scene, Model& model) 
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++) 
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		model.m_meshes.push_back(processMesh(mesh, scene));
	}
	for (unsigned int i = 0; i < node->mNumChildren; i++) 
	{
		processNode(node->mChildren[i], scene, model);
	}
}

Model ModelLoader::loadModel(std::string name) 
{
	Assimp::Importer importer;
	auto test = std::filesystem::path("../../resources/models/troll/troll.obj");
	auto test1 = std::filesystem::current_path();
	std::cout << test1;
	const aiScene* scene = importer.ReadFile(
		"../../resources/models/troll/troll.obj", 
		aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
	{
		std::cout << importer.GetErrorString();
		throw(std::exception("ASSIMP ERROR"));
	}
	Model model;
	processNode(scene->mRootNode, scene, model);
	return model;
}

