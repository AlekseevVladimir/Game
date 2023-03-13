project(game)
cmake_minimum_required(VERSION 3.10)
SET(SOURCES
	"GameCtrl.h"
)
add_library(${PROJECT_NAME} ${SOURCES})                      
set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "game")
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
