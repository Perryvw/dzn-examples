cmake_minimum_required(VERSION 3.5.0)

project(example-project VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

find_package(GTest CONFIG REQUIRED)

# Library with all generated dzn files

# Note: You want some kind of automation for this
# for example by including a generated cmake file containing a list of these files
add_custom_command(OUTPUT dzn-gen/Airlock.hh dzn-gen/Airlock.cc COMMAND dzn code -I dzn-src -o dzn-gen dzn-src/Airlock.dzn DEPENDS dzn-src/Airlock.dzn)
add_custom_command(OUTPUT dzn-gen/AirlockArmor.hh dzn-gen/AirlockArmor.cc COMMAND dzn code -I dzn-src -o dzn-gen dzn-src/armor/AirlockArmor.dzn DEPENDS dzn-src/armor/AirlockArmor.dzn)
add_custom_command(OUTPUT dzn-gen/AirlockSystem.hh dzn-gen/AirlockSystem.cc COMMAND dzn code -I dzn-src -o dzn-gen dzn-src/AirlockSystem.dzn DEPENDS dzn-src/AirlockSystem.dzn)
add_custom_command(OUTPUT dzn-gen/IAirlock.hh dzn-gen/IAirlock.cc COMMAND dzn code -I dzn-src -o dzn-gen dzn-src/IAirlock.dzn DEPENDS dzn-src/IAirlock.dzn)
add_custom_command(OUTPUT dzn-gen/IAirlockExternal.hh dzn-gen/IAirlockExternal.cc COMMAND dzn code -I dzn-src -o dzn-gen dzn-src/external/IAirlockExternal.dzn DEPENDS dzn-src/external/IAirlockExternal.dzn)
add_custom_command(OUTPUT dzn-gen/IDoor.hh dzn-gen/IDoor.cc COMMAND dzn code -I dzn-src -o dzn-gen dzn-src/components/IDoor.dzn DEPENDS dzn-src/components/IDoor.dzn)
add_custom_command(OUTPUT dzn-gen/IVacuum.hh dzn-gen/IVacuum.cc COMMAND dzn code -I dzn-src -o dzn-gen dzn-src/components/IVacuum.dzn DEPENDS dzn-src/components/IVacuum.dzn)
add_custom_command(OUTPUT dzn-gen/promise.hh dzn-gen/promise.cc COMMAND dzn code -I dzn-src -o dzn-gen dzn-src/utils/promise.dzn DEPENDS dzn-src/utils/promise.dzn)
add_custom_command(OUTPUT dzn-gen/result.hh dzn-gen/result.cc COMMAND dzn code -I dzn-src -o dzn-gen dzn-src/utils/result.dzn DEPENDS dzn-src/utils/result.dzn)

# Create lib and add sources
add_library(dzn-gen-lib STATIC
    dzn-gen/Airlock.cc
    dzn-gen/AirlockArmor.cc
    dzn-gen/AirlockSystem.cc
    dzn-gen/IAirlock.cc
    dzn-gen/IAirlockExternal.cc
    dzn-gen/IDoor.cc
    dzn-gen/IVacuum.cc
    dzn-gen/promise.cc
    dzn-gen/result.cc
)
target_include_directories(dzn-gen-lib PRIVATE "." "../include" "dzn-gen" "glue/foreign-components")

# Main library using the Dezyne lib

add_library(example-project

    glue/GlueDoor.cpp
    glue/GlueVacuum.cpp

    airlock.cpp

    # dzn runtime files
    runtime/coroutine.cc
    runtime/pump.cc
    runtime/runtime.cc
    runtime/thread-pool.cc
)

target_include_directories(example-project PRIVATE "." "../include" "glue/foreign-components")

target_link_libraries(example-project dzn-gen-lib)

# Test library using library

enable_testing()

include(GoogleTest)

add_executable(testexe test/testairlock.cpp)
target_include_directories(testexe PRIVATE ".")
target_link_libraries(testexe example-project GTest::gtest_main gmock)
