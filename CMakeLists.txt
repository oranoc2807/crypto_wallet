cmake_minimum_required(VERSION 3.16)

# Include the ESP-IDF project-specific build configuration
include($ENV{IDF_PATH}/tools/cmake/project.cmake)

# Ensure that the falcon component is included
include_directories($ENV{IDF_PATH}/components/falcon)

# Set the project name
project(main)

