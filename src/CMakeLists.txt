
set( CMAKE_CXX_FLAGS  "-std=c++11" )
add_library( hulk_fix decoder.cpp session.cpp transport.cpp tcp.cpp )
target_link_libraries( hulk_fix hulk_core )
install( TARGETS hulk_fix DESTINATION lib )
