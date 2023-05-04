set(TARGET Capture_screen)

add_executable(${TARGET} main.cpp ${WIN32_STREAM_SOURCE})

target_include_directories(${TARGET} PUBLIC 
	${OpenCV_INCLUDE_DIRS}
	#${Tesseract_INCLUDE_DIRS}
	"${CMAKE_SOURCE_DIR}/src"
	${WIN32_STREAM_SOURCE}
)


target_link_libraries(${TARGET} PRIVATE 
	${OpenCV_LIBS}
	#${Tesseract_LIBRARIES}
)