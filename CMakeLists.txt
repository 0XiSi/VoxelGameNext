cmake_minimum_required(VERSION 3.16)
project(VoxelGame VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)

# Build options
option(BUILD_CLIENT "Build OpenGL client" ON)
option(BUILD_SERVER "Build headless server" OFF)

# 3rd-party

# GLM (header-only)
add_library(glm INTERFACE)
target_include_directories(glm INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/glm)

# entt (header-only)
#add_library(entt INTERFACE)
#target_include_directories(entt INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/entt)


add_subdirectory(ThirdParty/glfw)
add_library(glad STATIC ThirdParty/glad/src/glad.c)
target_include_directories(glad PUBLIC ThirdParty/glad/include)


# Engine library (no main())
add_subdirectory(Engine)

# Game library (pure logic, no GL)
add_subdirectory(Game)

# Executables
if(BUILD_CLIENT)
  add_subdirectory(Client)
endif()

if(BUILD_SERVER)
  add_subdirectory(Server)
endif()

# Unit tests
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_subdirectory(Tests)
endif()