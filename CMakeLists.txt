cmake_minimum_required(VERSION 3.9)
project(OpenGL_STD)

set(CMAKE_CXX_STANDARD 14)

find_package(PkgConfig REQUIRED)
find_package(GLUT)
find_package(OpenGL)
#find_package(OpenCV REQUIRED)
#find_package(GLM)
#find_package(GLEW REQUIRED)
#find_package(glfw3)

#pkg_search_module(GLFW REQUIRED glfw3)


#include_directories(${GLFW_INCLUDE_DIRS})
#include_directories($(GLEW_INCLUDE_DIRS))
#link_libraries(${GLEW_LIBRARIES})

add_executable(main main.cpp)

target_link_libraries(main ${GLUT_LIBRARY})
target_link_libraries(main ${OPENGL_LIBRARY})
#target_link_libraries(main ${OpenCV_LIBS})