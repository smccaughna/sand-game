cmake_minimum_required (VERSION 3.8)

project (sand-game C)

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

set (CMAKE_C_STANDARD 11)
set (CMAKE_C_STANDARD_REQUIRED TRUE)

file (GLOB PROJ_SOURCES "src/*.c")
file (GLOB PROJ_HEADERS "src/*.h")

if (WIN32)
	add_executable (${PROJECT_NAME} WIN32 ${PROJ_SOURCES} ${PROJ_HEADERS})
else ()
	add_executable (${PROJECT_NAME} ${PROJ_SOURCES} ${PROJ_HEADERS})
endif ()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	target_compile_options (
		${PROJECT_NAME} PRIVATE
		$<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-g -Og -Wall>
		$<$<C_COMPILER_ID:MSVC>:/Zi /W4>
	)
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
	target_compile_options (
		${PROJECT_NAME} PRIVATE
		$<$<OR:$<C_COMPILER_ID:GNU>,$<C_COMPILER_ID:Clang>>:-O3>
		$<$<C_COMPILER_ID:MSVC>:/O2>
	)
endif ()

find_package (SDL2 REQUIRED)
find_package (SDL2_image REQUIRED)
find_package (SDL2_ttf REQUIRED)

target_link_libraries (
	${PROJECT_NAME} PRIVATE
	SDL2::Main
	SDL2::Image
	SDL2::TTF
)

if (NOT MSVC)
	target_link_libraries (${PROJECT_NAME} PRIVATE m)
endif ()

if (WIN32)
	add_custom_command (
		TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		${SDL2_PATH}/lib/x64/SDL2.dll
		$<TARGET_FILE_DIR:${PROJECT_NAME}>
	)
	add_custom_command (
		TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		${SDL2_IMAGE_PATH}/lib/x64/SDL2_image.dll
		$<TARGET_FILE_DIR:${PROJECT_NAME}>
	)
	add_custom_command (
		TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		${SDL2_TTF_PATH}/lib/x64/SDL2_ttf.dll
		$<TARGET_FILE_DIR:${PROJECT_NAME}>
	)
endif ()

add_custom_command (
	TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_CURRENT_SOURCE_DIR}/res
	$<TARGET_FILE_DIR:${PROJECT_NAME}>/res
)