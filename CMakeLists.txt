cmake_minimum_required(VERSION 3.16)
project(PasswordManager VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Sql)

qt_add_executable(PasswordManager
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    passwordmodel.cpp
    passwordmodel.h
    databasemanager.cpp
    databasemanager.h
    passwordrepository.cpp
    passwordrepository.h
)

target_link_libraries(PasswordManager PRIVATE Qt6::Widgets Qt6::Sql)
