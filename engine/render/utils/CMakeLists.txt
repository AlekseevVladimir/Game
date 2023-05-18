cmake_minimum_required(VERSION 3.10)
project(render_utils)
SET(SOURCES
	"render_utils.h"
	"render_utils.cpp"
)

ADD_LIBRARY(render_utils ${SOURCES}) 
SET_PROPERTY(TARGET ${PROJECT_NAME} PROPERTY FOLDER "engine/render")

