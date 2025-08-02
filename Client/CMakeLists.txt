add_executable(Client Main.cpp
  Render/RenderSystem.cpp
  Input/InputSystem.cpp
)
target_include_directories(Client PRIVATE ${CMAKE_SOURCE_DIR}/ThirdParty)
target_link_libraries(Client PRIVATE glm)
target_link_libraries(Client
  PRIVATE glfw glad ${OPENGL_LIBRARIES}
)
target_link_libraries(Client PRIVATE Engine Game)

target_include_directories(Client PRIVATE
    ${CMAKE_SOURCE_DIR}/ThirdParty
    ${CMAKE_SOURCE_DIR}/Game
    ${CMAKE_SOURCE_DIR}/Engine
)