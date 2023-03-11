cmake_minimum_required(VERSION 3.10)
project(open_gl)

SET(SHADER_SOURCES
	"shaders/shader_base.h"
	"shaders/shader_component.h"
	"shaders/solid_object_shader.h"
	"shaders/utils.h"
)
SET(MODEL_SOURCES
	"model/vao_component.h"
)
SOURCE_GROUP("shaders" FILES ${SHADER_SOURCES})
SOURCE_GROUP("model" FILES ${MODEL_SOURCES})
ADD_LIBRARY(open_gl ${SHADER_SOURCE} ${MODEL_SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)                 
SET_PROPERTY(TARGET ${PROJECT_NAME} PROPERTY FOLDER "engine/render")
