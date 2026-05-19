cmake_minimum_required(VERSION 3.25)
project(car_wash_wasm LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------------------------------------------------------------------------
# Target 1: Intermediate Static Library — native C++ business logic
# ---------------------------------------------------------------------------
add_library(car_wash_core STATIC cpp_app/car_wash.cc)
target_include_directories(car_wash_core PUBLIC cpp_app)

# ---------------------------------------------------------------------------
# Target 2: Binding Execution Bundle — Embind WebAssembly executable
# ---------------------------------------------------------------------------
add_executable(car_wash bindings.cc)

target_link_libraries(car_wash PRIVATE car_wash_core)

# Required compile flags
target_compile_options(car_wash PRIVATE
  -fexceptions
)

# Required link flags (all in one call to avoid duplicate property issues)
# MODULARIZE=1 + EXPORT_ES6=1 = proper ES module with default export for Vite
set_target_properties(car_wash PROPERTIES
  LINK_FLAGS
    "-fexceptions --bind -gsource-map -s MODULARIZE=1 -s EXPORT_ES6=1 -s ALLOW_MEMORY_GROWTH=1 --emit-tsd=${CMAKE_SOURCE_DIR}/dist/car_wash.d.ts"
)

# Output directly to dist/ so vite can serve it
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/dist)

# Enforce the final bundle name (Emscripten uses .js extension)
set_target_properties(car_wash PROPERTIES
  OUTPUT_NAME car_wash
)