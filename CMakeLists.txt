cmake_minimum_required(VERSION 3.10)

# Name of your project
project(AlgorithmVisualizer)

# SFML 3 strictly requires C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Tell CMake to look in the M2 Homebrew directory
set(SFML_DIR "/opt/homebrew/lib/cmake/SFML")

# Find the SFML 3 package
find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)

# Add your source files here
add_executable(AlgorithmVisualizer main.cpp Sorter.cpp Grid.cpp)

# Link SFML 3 to your executable (Notice the SFML:: prefix)
target_link_libraries(AlgorithmVisualizer PRIVATE SFML::Graphics SFML::Window SFML::System)

# Change this line:
