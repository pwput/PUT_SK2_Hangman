cmake_minimum_required(VERSION 3.12)

project(qt_hangman)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(either_qt6_or_qt5 NAMES Qt6 Qt5 COMPONENTS Widgets Network REQUIRED)
set(QtX Qt${either_qt6_or_qt5_VERSION_MAJOR})

find_package(${QtX} COMPONENTS Widgets Network REQUIRED)

set(SRCS
    main.cpp
    mywidget.ui
    mywidget.cpp
    mywidget.h
)

add_executable(hangman ${SRCS})

target_link_libraries(hangman ${QtX}::Widgets ${QtX}::Network)
