project(core)
cmake_minimum_required(VERSION 3.10)
SET(SOURCES
	"component.h"
	"system.h"
	"systems_holder.cpp"
	"systems_holder.h"
)
add_library(${PROJECT_NAME} ${SOURCES})                      
set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "engine")
