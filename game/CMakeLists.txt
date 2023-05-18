project(game)
cmake_minimum_required(VERSION 3.10)
SET(SOURCES
	"GameCtrl.h"
	"GameCtrl.cpp"
)
add_library(${PROJECT_NAME} ${SOURCES})  

LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/engine)
LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/engine/render/core)
target_link_libraries(game PUBLIC core) 
target_link_libraries(game PUBLIC utils) 
target_link_libraries(game PUBLIC player_controls) 
target_link_libraries(game PUBLIC render_core) 
set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "game")
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

