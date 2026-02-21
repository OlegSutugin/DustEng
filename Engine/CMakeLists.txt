#general information 
cmake_minimum_required(VERSION 3.31)

#cpp options
set(CMAKE_CXX_STANDARD 20)

#Common options
project(DustEngine VERSION 1.0.0) #solution name
add_executable(${PROJECT_NAME} Source/main.cpp)

#version
configure_file(EngineConfig.h.template ${CMAKE_CURRENT_SOURCE_DIR}/Source/EngineConfig.h)

set(ENGINE_SOURCES
    Source/Core/Engine.cpp
    Source/Core/Engine.h
    Source/Core/Utility.h
    .clang-format
    Source/pch.h
    Source/pch.cpp
    Source/EngineConfig.h
    Source/Log/Log.h
    Source/Log/Log.cpp
)

add_library(${PROJECT_NAME}Lib ${ENGINE_SOURCES})
target_include_directories(${PROJECT_NAME}Lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Source)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}Lib)

#add libraries
find_package(spdlog REQUIRED)
target_link_libraries(${PROJECT_NAME}Lib PUBLIC spdlog::spdlog)

#folders
set_property(GLOBAL PROPERTY USE_FOLDERS On)   #making all cmake projects in one folder in IDE if OFF (==FALSE)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER Engine)
set_target_properties(${PROJECT_NAME}Lib PROPERTIES FOLDER Engine)

include(${CMAKE_SOURCE_DIR}/Automation/CMAKE/CMakeHelpers.cmake)
create_ide_folders(ENGINE_SOURCES)
setup_precompiled_headers(${PROJECT_NAME}Lib Source/pch.cpp Source/pch.h "${ENGINE_SOURCES}")