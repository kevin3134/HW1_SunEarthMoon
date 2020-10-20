###########################
# SunEarthMoon project CMakeList
# include GLAD, GLFW, GLM
###########################

cmake_minimum_required(VERSION 3.0)
project(SunEarthMoon)

aux_source_directory(. DIR_TOOT_SRCS)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
include_directories(${PROJECT_SOURCE_DIR}/include)

###########################
# GLAD
###########################
add_executable(SunEarthMoon 
    src/main.cpp 
    src/glad.c
    src/glad.c
    include/Shader.h
)

###########################
# GLFW
###########################
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

if(WIN32)
    message(STATUS "starting configure glfw win32")
    add_subdirectory(glfw)
    include_directories(glfw/include)
    target_link_libraries(SunEarthMoon glfw ${GLFW_LIBRARIES})
endif()

###########################
# GLM
###########################
if(WIN32)
    message(STATUS "starting configure glm win32")
    add_subdirectory(glm)
    include_directories(glm/include)
    target_link_libraries(SunEarthMoon glm ${GLLM_LIBRARIES})
endif()

##########################
# OPENGL
##########################
find_package(OpenGL REQUIRED)




