#general information 
cmake_minimum_required(VERSION 3.31)

#setup conan profiles
set(PROFILE_PATH_Windows_Release "${CMAKE_CURRENT_SOURCE_DIR}/Automation/ConanProfiles/WindowsRelease")
set(PROFILE_PATH_Windows_Debug "${CMAKE_CURRENT_SOURCE_DIR}/Automation/ConanProfiles/WindowsDebug")
set(PROFILE_PATH_Linux_Release "${CMAKE_CURRENT_SOURCE_DIR}/Automation/ConanProfiles/LinuxRelease")
set(PROFILE_PATH_Linux_Debug "${CMAKE_CURRENT_SOURCE_DIR}/Automation/ConanProfiles/LinuxDebug")

set(CURRENT_PROFILE "PROFILE_PATH_${CMAKE_HOST_SYSTEM_NAME}_${CMAKE_BUILD_TYPE}")
message(STATUS "set conan profile: ${CURRENT_PROFILE}")

if(DEFINED ${CURRENT_PROFILE})
    set(CONAN_PROFILE "${${CURRENT_PROFILE}}")
    message(STATUS "using conan profile: ${CONAN_PROFILE}")
else()
    message(FATAL_ERROR "Unsupported configuration for: ${CURRENT_PROFILE}")
endif()

include(${CMAKE_CURRENT_SOURCE_DIR}/Automation/CMAKE/CMakeHelpers.cmake)
setup_conan(${CONAN_PROFILE} ${CONAN_PROFILE})

#Common options
project(Dust VERSION 1.0.0 LANGUAGES CXX) #solution name

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) #executable file in that directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) #libraries to that directory too

set_property(GLOBAL PROPERTY USE_FOLDERS On)   #making all cmake projects in one folder in IDE if OFF (==FALSE)
set(CMAKE_SUPPRESS_REGENERATION  TRUE)  #deleting ZERO_CHECK project in IDE

#project
add_subdirectory(Engine) #inner cmakelists
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT DustEngine) #making startup project in IDE delete all artifactws before that

system_info()