project(player_controls)
cmake_minimum_required(VERSION 3.10)
SET(SOURCES
	"player_controls_component.h"
	"input_control_system.h"
	"input_control_system.cpp"
)
add_library(${PROJECT_NAME} ${SOURCES})                      
set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "engine")

