cmake_minimum_required(VERSION 3.14)
project(logic_gate_simulator)

# raylib
find_package(raylib QUIET)
if (NOT raylib_FOUND)

    message("Fetching raylib...")
    include(FetchContent)
    FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 5.0
    )
    FetchContent_MakeAvailable(raylib)
endif()

# raylib-cpp
find_package(raylib_cpp QUIET)
if (NOT raylib_cpp_FOUND)
    message("fetching raylib-cpp...")
    include(FetchContent)
    FetchContent_Declare(
        raylib_cpp
        GIT_REPOSITORY https://github.com/RobLoach/raylib-cpp.git
        GIT_TAG v5.0.0
    )
    FetchContent_MakeAvailable(raylib_cpp)
endif()

# earcut.hpp
#find_package(earcut.hpp QUIET)
#if (NOT earcut.hpp_FOUND)
#    message("fetching earcut.hpp...")
#    #include(FetchContent)
#    FetchContent_Declare(
#        earcut.hpp
#        GIT_REPOSITORY https://github.com/mapbox/earcut.hpp.git
#        GIT_TAG v2.2.4
#    )
#    FetchContent_MakeAvailable(earcut.hpp)
#endif()

# This is the main part:
set(SOURCES src/main.cpp)

# add resources.


#configure_file(resources/shaders resources/shaders COPYONLY)
#configure_file(resources/shaders/lighting.glslv resources/shaders/lighting.glslv COPYONLY)
#configure_file(resources/shaders/lighting_instancing.glslv resources/shaders/lighting_instancing.glslv COPYONLY)

# add the executable.
add_executable(${PROJECT_NAME} ${SOURCES} "src/rendering/objects/objects.cpp" "src/rendering/renderer.cpp" "src/rendering/renderer.hpp" "src/rendering/objects/volumes/arrow.cpp" "src/rendering/objects/mesh_data.cpp" "src/math/matrix_a.cpp" "src/rendering/objects/volumes/rect.cpp" "src/resource_manager/resource_manager.cpp" "src/logic/logic_gate.cpp" "src/logic/logic_gate_io.cpp" "resources/shaders/rlights.h" "src/logic/logic_pipe.cpp" "src/math/rand.cpp" "src/rendering/objects/instanced_mesh_factory.cpp" "src/simulation/ticker.cpp" "src/storage/object_storage.cpp" "src/logic/logic_gates/input_toggle_gate.cpp" "src/rendering/objects/volumes/text.cpp" "src/rendering/objects/meshes/mesh_custom.cpp")

# Thanks to:
# https://stackoverflow.com/a/58633313

message("Binary Dir: ${CMAKE_BINARY_DIR}")
message("Source Dir: ${CMAKE_SOURCE_DIR}")

# First check if /resources/shaders exists.
if(EXISTS ${CMAKE_SOURCE_DIR}/resources/shaders)
    add_custom_target(copy-files ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources/shaders
        ${CMAKE_BINARY_DIR}/resources/shaders
    )
endif()

add_dependencies(${PROJECT_NAME} copy-files)

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
target_link_libraries(${PROJECT_NAME} PUBLIC raylib raylib_cpp)

# Web Configurations
if (${PLATFORM} STREQUAL "Web")
    # Tell Emscripten to build an example.html file.
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")

    # Required linker flags for using Raylib with Emscripten
    target_link_options(${PROJECT_NAME} PRIVATE -sEXPORTED_FUNCTIONS=['_main','_malloc'] -sEXPORTED_RUNTIME_METHODS=ccall -sUSE_GLFW=3)
endif()

# That's it! You should have an example executable that you can run. Have fun!
message("cmake process complete.")