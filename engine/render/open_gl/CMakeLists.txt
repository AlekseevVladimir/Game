cmake_minimum_required(VERSION 3.10)
project(open_gl)

SET(SHADER_SOURCES
	"shaders/open_gl_shader.h"
	"shaders/shader_component.h"
	"shaders/utils.h"
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
ADD_LIBRARY(open_gl ${SHADER_SOURCES} ${MODEL_SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)                 
SET_PROPERTY(TARGET ${PROJECT_NAME} PROPERTY FOLDER "engine/render")
