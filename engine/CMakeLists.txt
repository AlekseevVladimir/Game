CMAKE_MINIMUM_REQUIRED(VERSION 3.20.2)
SET(SUBFOLDERS 
	"core"
	"engine_math"
	"render"
	"transform"
	"utils"
	"player_controls"
	"relations"
)

FOREACH(subfolder ${SUBFOLDERS})
	ADD_SUBDIRECTORY(${subfolder})
ENDFOREACH()

