cmake_minimum_required(VERSION 3.20)

set(CMAKE_TOOLCHAIN_FILE "vcpkg/scripts/buildsystems/vcpkg.cmake")
set(VCPKG_TARGET_TRIPLET "x64-windows-static")

cmake_policy(SET CMP0091 NEW)

project(Projet_OpenCV)

add_executable(Projet_OpenCV src/Main.cpp)

set_property(TARGET Projet_OpenCV PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

target_compile_options(Projet_OpenCV PRIVATE /Zc:__cplusplus)
target_compile_features(Projet_OpenCV PRIVATE cxx_std_20)

find_package(OpenCV CONFIG REQUIRED)

target_link_libraries(Projet_OpenCV PRIVATE
		${OpenCV_LIBS})

add_custom_command(TARGET Projet_OpenCV PRE_BUILD
		COMMAND robocopy ${PROJECT_SOURCE_DIR}/resources/ ${PROJECT_BINARY_DIR}/resources /E /NS /NC /NFL /NDL /NP /NJH /NJS  || cmd /c "exit /b 0")
