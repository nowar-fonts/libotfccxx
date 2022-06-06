cmake_minimum_required(VERSION 3.12)
project(libotfcc++)
include(CheckLibraryExists)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 23)

include_directories(
	"${PROJECT_SOURCE_DIR}/include"
)

add_executable(main
	main.cpp
)

check_library_exists(m sin "" require_lib_m)
if (require_lib_m)
	target_link_libraries(main m)
endif (require_lib_m)
