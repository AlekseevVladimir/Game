CMAKE_MINIMUM_REQUIRED(VERSION 3.20.2)
ADD_EXECUTABLE(TheGame main.cpp glad.c)
target_link_libraries(TheGame ${PROJECT_SOURCE_DIR}/3d_party/libs/glfw3.lib)
target_link_libraries(TheGame ${PROJECT_SOURCE_DIR}/3d_party/libs/assimp-vc142-mtd.lib)
SET_PROPERTY(TARGET TheGame PROPERTY FOLDER "executable")
include_directories( ${PROJECT_SOURCE_DIR}/engine/core )
include_directories( ${PROJECT_SOURCE_DIR}/game )
target_link_libraries(TheGame core)
target_link_libraries(TheGame game)

message(${PROJECT_SOURCE_DIR})
