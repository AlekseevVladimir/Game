cmake_minimum_required(VERSION 3.10)
project(open_gl)

SET(SHADER_SOURCES
	"shaders/open_gl_shader.h"
	"shaders/shader_utils.h"
	"shaders/shader_utils.cpp"
)
SET(MODEL_SOURCES
	"model/vao_component.h"
	"model/model_loader.h" 
	"model/model_loader.cpp" 
	"model/model_component.h" 
	"model/model.h" 
	"model/model.cpp")
SOURCE_GROUP("shaders" FILES ${SHADER_SOURCES})
SOURCE_GROUP("model" FILES ${MODEL_SOURCES})
LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/engine/render/utils)
ADD_LIBRARY(open_gl ${SHADER_SOURCES} ${MODEL_SOURCES})
target_link_libraries(open_gl PUBLIC render_utils) 

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)                 
SET_PROPERTY(TARGET ${PROJECT_NAME} PROPERTY FOLDER "engine/render")
