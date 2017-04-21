cmake_minimum_required(VERSION 3.7)
project(boatshit)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -g -O0")

#FIXME: #define GLM_FORCE_RADIANS 1

find_package(CURL REQUIRED)
#FIXME: find_package(glm REQUIRED)
find_package(GLEW REQUIRED)
find_package(SDL2 REQUIRED)
find_package(DevIL REQUIRED)
find_package(ASSIMP REQUIRED)
find_package(OpenAL REQUIRED)

add_executable(main main.cpp) #water.cpp sky.cpp)

target_include_directories(main PUBLIC
  ${GLM_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS} ${CURL_INCLUDE_DIR} ${GLEW_INCLUDE_DIR} ${ASSIMP_INCLUDE_DIRS} ${OPENAL_INCLUDE_DIR}
)
 
target_link_libraries(main
  ${SDL2_LIBRARIES} ${CURL_LIBRARY} ${GLEW_LIBRARY} -lGL -lGLU ${IL_LIBRARIES} ${ILU_LIBRARIES} ${ASSIMP_LIBRARIES} ${OPENAL_LIBRARY} -lalut
)

add_subdirectory(data)
