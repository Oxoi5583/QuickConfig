cmake_minimum_required(VERSION 3.30)
# For Disable MSVC Visual C Dll
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MT")
set(CMAKE_EXE_LINKER_FLAGS "
    /NODEFAULTLIB:msvcrt.lib msvcrt.lib
    /NODEFAULTLIB:msvcrt.lib msvcrtd.lib
    /NODEFAULTLIB:msvcrt.lib libcmt.lib
")
project(QuickConfig)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_COMPILER_LAUNCHER ccache)

set(EXE_PATH "${CMAKE_SOURCE_DIR}/bin")
set(LIB_PATH "${CMAKE_SOURCE_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${LIB_PATH})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${LIB_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${EXE_PATH})

include_directories(include)

file(GLOB LIB_SRC_FILES "src/*.c*")
add_library(${PROJECT_NAME} ${LIB_SRC_FILES})

file(GLOB EXE_SRC_FILES "test/*.c*")
add_executable(test ${EXE_SRC_FILES})
target_link_directories(test PRIVATE ${LIB_PATH})
target_link_libraries(test ${PROJECT_NAME})