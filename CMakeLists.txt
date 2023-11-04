cmake_minimum_required(VERSION 3.26)
project(Mota_cpp)

set(CMAKE_CXX_STANDARD 23)

add_definitions(-DSFML_STATIC)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")
link_directories("${CMAKE_CURRENT_SOURCE_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

IF(WIN32)
    set(CMAKE_C_FLAGS"${CMAKE_C_FLAGS} -fexec-charset=GBK")
ENDIF()

add_executable(main "src/main.cpp"
        "src/GameSystem.cpp"
        "src/stdafx.cpp"
        "src/GameData.cpp"
        "ico/Icon.o")

target_link_options(main PRIVATE -flto)

target_link_libraries(main -static-libgcc -static-libstdc++ sfml-graphics-s sfml-audio-s sfml-window-s sfml-system-s opengl32 openal32 freetype flac vorbisenc vorbisfile vorbis ogg gdi32 winmm)