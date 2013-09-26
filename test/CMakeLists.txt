#add_executable( test-client test-client.cpp )
#add_executable( test-decoder test-decoder.cpp )
add_executable( test_transport test_transport.cpp )
add_executable( test_server test_server.cpp )

#target_link_libraries( test-client hulk-core hulk-fix )
#target_link_libraries( test-decoder hulk-fix )
target_link_libraries( test_transport hulk_fix )
target_link_libraries( test_server hulk_fix )
