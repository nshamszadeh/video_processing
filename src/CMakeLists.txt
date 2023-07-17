include_directories( ${OpenCV_INCLUDE_DIRS} )
include_directories( ../include )
add_executable( test main.cpp )
target_link_libraries( test ${OpenCV_LIBS} )
