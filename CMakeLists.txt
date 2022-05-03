cmake_minimum_required(VERSION 3.10)

# project info
project(food-disp VERSION 1.0.0)

# c++ standard definition
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCES
    src/food_disp.cxx
    src/mqtt/fd_mosquitto.cxx)

if(!${CONFIG_FILE})
    message(FATAL_ERROR "Config file not set")
endif()

add_compile_definitions(CONFIG_FILE=${CONFIG_FILE})

add_executable(${PROJECT_NAME} ${SOURCES})

# Specify object linkage
target_link_libraries(${PROJECT_NAME} PRIVATE
    mosquittopp
    encrypt
)

# Install executables on packages output
install(TARGETS ${PROJECT_NAME}
    DESTINATION ${CMAKE_INSTALL_BINDIR})
