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


set(ENGINE_SOURCES
    Core/Logger.cpp
    Core/Timer.cpp
    Core/LinearAllocator.cpp
    Platform/Window.cpp
)

set(ENGINE_HEADERS
    Core/Logger.h
    Core/Timer.h
    Core/Assert.h
    Core/LinearAllocator.h
    Platform/Window.h
)

add_library(Engine STATIC
    ${ENGINE_SOURCES}
    ${ENGINE_HEADERS}
)

target_include_directories(Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(Engine PUBLIC glfw glad_gl_core)

target_compile_features(Engine PUBLIC cxx_std_23)