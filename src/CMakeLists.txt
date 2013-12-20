
add_library( hulk_fix acceptor.cpp decoder.cpp encoder.cpp session.cpp transport.cpp tcp.cpp )
target_link_libraries( hulk_fix hulk_core )
install( TARGETS hulk_fix DESTINATION lib )
