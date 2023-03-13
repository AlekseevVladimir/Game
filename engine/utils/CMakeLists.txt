project(utils)
cmake_minimum_required(VERSION 3.10)
SET(SOURCES
	"events.h"
	"engine_constants.h"
	"game_object_factory.h"
	"game_object_factory.cpp"
)
add_library(${PROJECT_NAME} ${SOURCES})                      
set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "engine")
