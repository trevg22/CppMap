cmake_minimum_required(VERSION 3.14)

#Cmake settings
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_AUTOUIC ON)
#set(CMAKE_AUTORCC ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(map LANGUAGES C CXX)
add_executable(map)
#set_target_properties(map PROPERTIES AUTOMOC TRUE)

add_subdirectory(src)
add_subdirectory(third-party/voronoi)
add_subdirectory(third-party/SQLite3)
add_subdirectory(third-party/lua)

find_package(Marble REQUIRED)
find_package(Qt5 COMPONENTS Network REQUIRED)
find_package(Threads REQUIRED)

if (NOT MSVC)
    target_link_libraries(map PRIVATE pthread)
else()
    target_link_libraries(map PRIVATE Threads:Threads)
endif()
target_link_libraries(map PRIVATE ${CMAKE_DL_LIBS})

target_link_libraries(map PRIVATE lua)
target_link_libraries(map PRIVATE voronoi)
target_link_libraries(map PRIVATE Marble)
target_link_libraries(map PRIVATE sqlite3)

