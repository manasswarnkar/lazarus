add_library(Engine STATIC
    Core/Logger.cpp
    Core/Timer.cpp
)

target_include_directories(Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_features(Engine PUBLIC cxx_std_23)