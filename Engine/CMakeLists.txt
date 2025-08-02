add_library(Engine STATIC
  Core/Types.hpp
  Core/Common.hpp
  Render/Mesh.cpp Render/Mesh.hpp
  Render/Shader.cpp Render/Shader.hpp
  Platform/Window.cpp Platform/Window.hpp
  Render/Camera.cpp Render/Camera.hpp
  # …add others as you move them
)
target_link_libraries(Engine PRIVATE glm)

target_include_directories(Engine PUBLIC .)
target_link_libraries(Engine
  PRIVATE glfw glad ${OPENGL_LIBRARIES}
)

target_include_directories(Engine PUBLIC
    ${CMAKE_SOURCE_DIR}/ThirdParty
    ${CMAKE_SOURCE_DIR}/Game
    ${CMAKE_SOURCE_DIR}/Engine
)