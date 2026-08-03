include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.15.2
)

# Prevent GTest from overriding your project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

add_executable(EngineTests
    TimerTest.cpp
    AssertTest.cpp
    LinearAllocatorTest.cpp
    EventTest.cpp
    InputTest.cpp
    BufferLayoutTest.cpp
)

target_link_libraries(EngineTests
    PRIVATE
        Engine
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(EngineTests)