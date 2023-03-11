#include "Model.h"
#include "TexturesCtrl.h"


std::vector<Mesh::Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) {
	std::vector<Mesh::Texture> textures;
	TexturesCtrl& texCtrl = TexturesCtrl::getInstance();
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		Mesh::Texture texture;
		texture.id = texCtrl.loadImage(str.C_Str(), directory);
		texture.type = typeName;
		texture.path = str.C_Str();
		textures.push_back(texture);
		
	}
	return textures;
}