project(relations)
cmake_minimum_required(VERSION 3.10)
SET(SOURCES
	"hierarchy_component.h"
	"hierarchy_component.cpp"
)
add_library(${PROJECT_NAME} ${SOURCES})                      
set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "engine")
