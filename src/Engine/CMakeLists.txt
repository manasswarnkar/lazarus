include(FetchContent)

# --- GLFW ---
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.4
)
FetchContent_MakeAvailable(glfw)


# --- GLAD (OpenGL function loader) ---
FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG        v2.0.6
    SOURCE_SUBDIR  cmake
)
FetchContent_MakeAvailable(glad)

glad_add_library(glad_gl_core STATIC API gl:core=4.5)


# --- glm ---
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        1.0.1
)
FetchContent_MakeAvailable(glm)

set(ENGINE_SOURCES
    Core/Logger.cpp
    Core/Timer.cpp
    Core/LinearAllocator.cpp
    Core/UUID.cpp
    Core/ViewportMath.cpp
    Platform/Window.cpp
    Input/Input.cpp
    Renderer/Buffer.cpp
    Renderer/OrthographicCamera.cpp
    Renderer/OrthographicCameraController.cpp
    Renderer/RenderCommand.cpp
    Renderer/Renderer.cpp
    Renderer/Shader.cpp
    Renderer/VertexArray.cpp
)

set(ENGINE_HEADERS
    Core/Logger.h
    Core/Timer.h
    Core/Assert.h
    Core/LinearAllocator.h
    Core/UUID.h
    Core/ViewportMath.h
    Platform/Window.h
    Events/Event.h
    Events/ApplicationEvent.h
    Events/KeyEvent.h
    Events/MouseEvent.h
    Input/KeyCodes.h
    Input/MouseCodes.h
    Input/Input.h
    Renderer/Buffer.h
    Renderer/OrthographicCamera.h
    Renderer/OrthographicCameraController.h
    Renderer/RenderCommand.h
    Renderer/Renderer.h
    Renderer/Shader.h
    Renderer/VertexArray.h
)

add_library(Engine STATIC
    ${ENGINE_SOURCES}
    ${ENGINE_HEADERS}
)

target_include_directories(Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(Engine PUBLIC glfw glad_gl_core glm::glm)

target_compile_features(Engine PUBLIC cxx_std_23)