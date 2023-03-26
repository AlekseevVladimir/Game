#include "Model.h"
#include <engine/render/core/textures_ctrl.h>
#include <engine/render/open_gl/shaders/open_gl_shader.h>
#include <engine/core/game_object.h>
#include <engine/render/core/view_point_component.h>
#include <engine/render/utils/render_utils.h>
#include <engine/transform/position_component.h>
#include <engine/transform/transform_utils.h>


void Model::setModelData(std::shared_ptr<Shader> shaderBase) {
	OpenGLShader* shader = dynamic_cast<OpenGLShader*>(shaderBase.get());
	TexturesCtrl& texCtrl = TexturesCtrl::getInstance();
	GameObject* viewPointPtr = GameObjectHolder::getInstance().getObjectsWithComponent<ViewPointComponent>()[0];
	GameObject* cube = GameObjectHolder::getInstance().getObject("cube2");
	shader->setFloat3("viewPos", &viewPointPtr->getComponent<PositionComponent>()->getPos()[0]);
	shader->setMatrix4Float("view", GL_FALSE, glm::value_ptr(getView(viewPointPtr)));
	shader->setMatrix4Float("projection", GL_FALSE, glm::value_ptr(getProjection()));
	shader->setMatrix4Float(
		"model", GL_FALSE,
		glm::value_ptr(getTransform(cube->getComponent<PositionComponent>(), cube->getComponent<RotationComponent>())));

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
		glDrawArrays(GL_TRIANGLES, 0, mesh.m_vertices.size());
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
	}
}