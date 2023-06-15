#include "open_gl_shadow_map_shader.h"
#include "glad/glad.h"

void ShadowMapShader::setMatrices(GameObject* viewPointPtr)
{
	LightDirectionComponent* lightDirComponent = 
		viewPointPtr->getComponent<LightDirectionComponent>();
	glm::mat lightView = glm::lookAt(
		lightDirComponent->getFrom().getGlm(),
		lightDirComponent->getTo().getGlm(),
		glm::vec3(0.f, 1.f, 0.f)  // vector up
	);
	glm::mat4 lightSpaceMatrix = lightDirComponent->_projectionMatrix * lightView;
	setMatrix4Float("lightSpaceMatrix", GL_FALSE, glm::value_ptr(lightSpaceMatrix));
};
