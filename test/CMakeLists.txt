find_package(Catch2 3 QUIET)
if(NOT Catch2_FOUND)
    # If Catch2 is not found, fetch it from GitHub
    include(FetchContent)
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.8.1
    )
    FetchContent_MakeAvailable(Catch2)
endif()

add_executable(offsetAllocatorTests offsetAllocatorTests.cpp)
set_property(TARGET offsetAllocatorTests PROPERTY CXX_STANDARD 20)

target_link_libraries(offsetAllocatorTests 
    PRIVATE
    offsetAllocator
    Catch2::Catch2WithMain
)

if(MSVC)
    target_compile_options(offsetAllocatorTests PRIVATE /W4)
else()
    target_compile_options(offsetAllocatorTests PRIVATE -Wall -Wextra -pedantic)
endif()

include(Catch)
catch_discover_tests(offsetAllocatorTests)
