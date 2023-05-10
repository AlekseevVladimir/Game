#include "glad/glad.h"
#include "model.h"
#include <engine/render/core/textures_ctrl.h>
#include <engine/render/open_gl/shaders/open_gl_shader.h>
#include <engine/core/game_object.h>
#include <engine/render/core/view_point_component.h>
#include <engine/render/utils/render_utils.h>
#include <engine/render/core/shaders_manager.h>
#include "engine/render/core/textures_ctrl.h"
#include <engine/transform/position_component.h>
//#include <engine/transform/transform_utils.h>
	
void ElementsMesh::setupMesh() {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int),
			&m_indices[0], GL_STATIC_DRAW);

		// Координаты вершин
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

		// Нормали вершин
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

		// Текстурные координаты вершин
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

		glBindVertexArray(0);
	}

void Mesh::setupMesh() {
		float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
		};
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// Координаты вершин
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

		// Нормали вершин
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

		// Текстурные координаты вершин
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

		glBindVertexArray(0);
	}

//void Model::setModelDataAndDraw(std::shared_ptr<Shader> shaderBase, GameObject* goPtr) {}
/*
//TODO: move this to shader as it is shader specific
template<typename TMesh>
void Model<TMesh>::setModelDataAndDraw(
	std::shared_ptr<Shader> shaderBase, GameObject* goPtr, GameObject* viewPointPtr) 
{
	OpenGLShader* shader = dynamic_cast<OpenGLShader*>(shaderBase.get());
	TexturesCtrl& texCtrl = TexturesCtrl::getInstance();
//	GameObject* cube = GameObjectHolder::getInstance().getObject("cube2");
	shader->setFloat3("viewPos", &viewPointPtr->getComponent<PositionComponent>()->getPos()[0]);
	shader->setMatrix4Float("view", GL_FALSE, glm::value_ptr(getView(viewPointPtr)));
	shader->setMatrix4Float("projection", GL_FALSE, glm::value_ptr(getProjection()));
	shader->setMatrix4Float(
		"model", GL_FALSE,
		glm::value_ptr(getTransform(goPtr->getComponent<PositionComponent>(), 
			goPtr->getComponent<RotationComponent>(), 
			goPtr->getComponent<ScaleComponent>())));

	int cnt = 0;
	for (auto mesh : m_meshes) {
		for (const Mesh::Texture& texture : mesh.m_textures)
		{
			// Получаем номер текстуры (значение N в diffuse_textureN)
			std::string name = texture.type;
			unsigned idx = texCtrl.bindTexture(texture.id);
			shader->setInt1(("material." + name).c_str(), idx);
		}
		shader->setFloat1("material.shininess", 32.0f);
		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(mesh.VAO);
		//glDrawElements(GL_TRIANGLES, mesh.getIndices().size(), GL_UNSIGNED_INT, 0);
	//.	glDrawArrays(GL_TRIANGLES, 0, mesh.m_vertices.size());
		mesh.drawMesh();
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
	}
}
*/
void Mesh::draw()
{
	glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
}

void ElementsMesh::draw()
{
	glDrawElements(GL_TRIANGLES, getIndices().size(), GL_UNSIGNED_INT, 0);
}

//template class Model<Mesh>;
//template class Model<ElementsMesh>;