cmake_minimum_required(VERSION 3.10)
project(render_core)
SET(CORE_SOURCES
	"constants.h"
	"graphics_ctrl.h"
	"primitives.h"
	"stb_image.h"
	"stb_image.cpp"
	"textures_ctrl.h"
	"vertices_array.h"
	"glad.c"
)

SET(LIGHT_SOURCES
	"light/light_direction_component.h"
	"light/light_emitter_component.h"
	"light/spot_light_component.h"
)
SET(MODEL_SOURCES
	"model/material_component.h"
    "model/model_component.h"
)
SOURCE_GROUP("light" FILES ${LIGHT_SOURCES})
SOURCE_GROUP("model" FILES ${MODEL_SOURCES})
SOURCE_GROUP("" FILES ${CORE_SOURCES})
ADD_LIBRARY(render_core ${CORE_SOURCES} ${LIGHT_SOURCES} ${MODEL_SOURCES})
SOURCE_GROUP("simple" FILES "subfolder_item.cpp" "subfolder_item.h")                      
SET_PROPERTY(TARGET ${PROJECT_NAME} PROPERTY FOLDER "engine/render")
