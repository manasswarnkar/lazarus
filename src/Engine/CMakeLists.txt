set(ENGINE_SOURCES
    Core/Logger.cpp
    Core/Timer.cpp
)

set(ENGINE_HEADERS
    Core/Logger.h
    Core/Timer.h
    Core/Assert.h
)

add_library(Engine STATIC
    ${ENGINE_SOURCES}
    ${ENGINE_HEADERS}
)

target_include_directories(Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_features(Engine PUBLIC cxx_std_23)