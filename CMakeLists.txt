cmake_minimum_required(VERSION 4.3.0)

project(physics-sandbox
  VERSION 1.0.0
  SPDX_LICENSE "MIT"
  LANGUAGES "CXX")

# Bundle in SDL3 as a static library
set(SDL_SHARED false)
set(SDL_STATIC true)
add_subdirectory("vendored/SDL3-3.4.18" EXCLUDE_FROM_ALL)

add_executable(physics-sandbox "src/main.cpp")

target_link_libraries(physics-sandbox SDL3::SDL3)
target_compile_features(physics-sandbox PUBLIC cxx_std_26)
target_include_directories(physics-sandbox PRIVATE "include/")
