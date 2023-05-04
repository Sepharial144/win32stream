cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)

project("win32Stream" DESCRIPTION "Win32 screen capture library" LANGUAGES CXX)

find_package(OpenCV REQUIRED)
#find_package(Tesseract CONFIG REQUIRED)

file(GLOB HDR_FILES "${CMAKE_SOURCE_DIR}/src/*.hpp"
					"${CMAKE_SOURCE_DIR}/src/capture/*.hpp"
					"${CMAKE_SOURCE_DIR}/src/ProcessEngine/*.hpp"
					"${CMAKE_SOURCE_DIR}/src/exception/*.hpp"
)

file(GLOB SRC_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp"
					"${CMAKE_SOURCE_DIR}/src/capture/*.cpp"
					"${CMAKE_SOURCE_DIR}/src/ProcessEngine/*.cpp"
					"${CMAKE_SOURCE_DIR}/src/exception/*.cpp"
)

set(WIN32_STREAM_SOURCE ${HDR_FILES} ${SRC_FILES})

message("Headers: ${HDR_FILES}")
message("Source: ${SRC_FILES}")

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
