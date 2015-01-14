set( CMAKE_CXX_FLAGS "-Wall -O2" )

file( GLOB SRCS *.cpp *.h )

add_executable( hlsyn ${SRCS} )
