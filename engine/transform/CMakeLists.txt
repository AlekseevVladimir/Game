project(transform)
cmake_minimum_required(VERSION 3.10)
SET(SOURCES
	"movement_request_component.h"
	"position_component.h"
	"position_component.cpp"
	"rotation_component.h"
	"rotation_request_component.h"
	"scale_component.h"
	"view_point_component.h"
	"transform_utils.h" 
	"movement_system.h" 
	"movement_system.cpp" 
	"rotation_system.h" 
	"rotation_system.cpp"
)
add_library(${PROJECT_NAME} ${SOURCES})                      
set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "engine")

