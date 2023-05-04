cmake_minimum_required(VERSION 3.10)
project(open_gl)

SET(SHADER_SOURCES
	"shaders/open_gl_shader.h"
	"shaders/open_gl_shader.cpp"
	"shaders/shader_utils.h"
	"shaders/shader_utils.cpp"
	"shaders/open_gl_shadow_map_shader.h" 
	"shaders/open_gl_shadow_map_shader.cpp" 
	"shaders/open_gl_solid_object_shader.h"
	"shaders/open_gl_solid_object_shader.cpp")
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
LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/engine/render/core)
ADD_LIBRARY(open_gl ${SHADER_SOURCES} ${MODEL_SOURCES})
target_link_libraries(open_gl PUBLIC render_utils) 
target_link_libraries(open_gl PUBLIC render_core) 

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)                 
SET_PROPERTY(TARGET ${PROJECT_NAME} PROPERTY FOLDER "engine/render")
