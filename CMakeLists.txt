#general information 
cmake_minimum_required(VERSION 3.31)

#Common options
project(Dust VERSION 1.0.0 LANGUAGES CXX) #solution name

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) #executable file in that directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) #libraries to that directory too

set_property(GLOBAL PROPERTY USE_FOLDERS On)   #making all cmake projects in one folder in IDE if OFF (==FALSE)
set(CMAKE_SUPPRESS_REGENERATION  TRUE)  #deleting ZERO_CHECK project in IDE

#project
add_subdirectory(Engine) #inner cmakelists
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT DustEngine) #making startup project in IDE delete all artifactws before that

include(${CMAKE_SOURCE_DIR}/Automation/CMAKE/CMakeHelpers.cmake)
system_info()