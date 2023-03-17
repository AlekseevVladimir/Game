CMAKE_MINIMUM_REQUIRED(VERSION 3.20.2)
ADD_EXECUTABLE(TheGame main.cpp glad.c)
target_link_libraries(TheGame ${PROJECT_SOURCE_DIR}/3d_party/libs/glfw3.lib)
SET_PROPERTY(TARGET TheGame PROPERTY FOLDER "executable")

message(${PROJECT_SOURCE_DIR})