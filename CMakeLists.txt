cmake_minimum_required(VERSION 3.18.0)
#PROJECT:
project(opengl_ex)
# Find dependencies
#find_package(glad REQUIRED)
#find_package(imgui REQUIRED)

#Source Files:
add_subdirectory(src)

#GLFW:
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glfw)
target_link_libraries(${PROJECT_NAME} PUBLIC glfw)
#OPENGL:
find_package( OpenGL REQUIRED )
target_link_libraries(${PROJECT_NAME} PUBLIC OpenGL::GL)
#GLM:
#add_subdirectory(external/glm)
#target_link_libraries(${PROJECT_NAME} PUBLIC glm)
#Add Include Dir
add_subdirectory(include)
target_include_directories(${PROJECT_NAME} PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/include"
                          #"${PROJECT_SOURCE_DIR}/external/glm"
                          )
