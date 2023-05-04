cmake_minimum_required (VERSION 3.20)

set(CMAKE_CXX_STANDARD 17)

project("win32Stream" DESCRIPTION "Win32 screen capture library" LANGUAGES CXX)

find_package(OpenCV REQUIRED)
#find_package(Tesseract CONFIG REQUIRED)

file(GLOB_RECURSE WIN32_STREAM_SOURCE "${CMAKE_SOURCE_DIR}/src/*.hpp" "${CMAKE_SOURCE_DIR}/src/*.cpp")

add_library(${PROJECT_NAME} ${WIN32_STREAM_SOURCE})

add_subdirectory(examples)

target_include_directories(${PROJECT_NAME} PUBLIC 
	${OpenCV_INCLUDE_DIRS}
	#${Tesseract_INCLUDE_DIRS}
	"${CMAKE_SOURCE_DIR}/src"
)


target_link_libraries(${PROJECT_NAME} PRIVATE 
	${OpenCV_LIBS}
	#${Tesseract_LIBRARIES}
)
