project(core)
cmake_minimum_required(VERSION 3.10)
SET(SOURCES
	"component.h"
	"system.h"
	"systems_holder.cpp"
	"systems_holder.h"
	"window_ctrl.cpp"
	"window_ctrl.h"
	"game_object.cpp"
	"game_object.h"
)
add_library(${PROJECT_NAME} ${SOURCES})                      
set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "engine")
message(${CMAKE_BINARY_DIR})
