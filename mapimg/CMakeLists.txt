PROJECT( mapIMG )
cmake_minimum_required(VERSION 2.4.0)
FIND_PACKAGE(QT4 REQUIRED)

INCLUDE_DIRECTORIES(Source)

# tell cmake to process CMakeLists.txt in that subdirectory
add_subdirectory(Source)
