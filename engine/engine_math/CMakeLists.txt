project(engine_math)
cmake_minimum_required(VERSION 3.10)
SET(SOURCES
	"vector2.h"
	"vector3.h"
)
add_library(${PROJECT_NAME} ${SOURCES})                      
set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "engine")
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)