cmake_minimum_required(VERSION 3.16)
project(SellGatorERP LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(erp STATIC
    src/models.cpp
    src/services.cpp
    src/app.cpp
)

target_include_directories(erp PUBLIC include)

add_executable(sellgator_erp src/main.cpp)

target_link_libraries(sellgator_erp PRIVATE erp)
