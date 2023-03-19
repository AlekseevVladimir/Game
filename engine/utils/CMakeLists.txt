project(utils)
cmake_minimum_required(VERSION 3.10)
SET(SOURCES
	"events.h"
	"engine_constants.h"
	"game_object_factory.h"
	"game_object_factory.cpp"
)

add_library(${PROJECT_NAME} ${SOURCES})                      

LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/engine)
LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/engine/render)
target_link_libraries(utils PUBLIC transform)
target_link_libraries(utils PUBLIC player_controls)
target_link_libraries(utils PUBLIC render_core)
set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "engine")
