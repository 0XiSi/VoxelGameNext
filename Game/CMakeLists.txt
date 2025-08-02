add_library(Game STATIC
  Components.hpp

  Voxel/BlockTypes.hpp
  Voxel/Chunk.hpp Voxel/Chunk.cpp
  Voxel/ChunkManager.hpp Voxel/ChunkManager.cpp

  Systems/WorldSystem.hpp Systems/WorldSystem.cpp
  Systems/WorldSerializer.hpp Systems/WorldSerializer.cpp


  Factories/PlayerFactory.hpp Factories/PlayerFactory.cpp
)
target_link_libraries(Game PRIVATE glm)
target_include_directories(Game PUBLIC .)
target_link_libraries(Game PUBLIC Engine)

target_include_directories(Game PRIVATE
    ${CMAKE_SOURCE_DIR}/ThirdParty
    ${CMAKE_SOURCE_DIR}/Game
    ${CMAKE_SOURCE_DIR}/Engine
)