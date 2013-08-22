#add_executable( test-client test-client.cpp )
#add_executable( test-decoder test-decoder.cpp )
add_executable( test-transport test-transport.cpp )

#target_link_libraries( test-client hulk-core hulk-fix )
#target_link_libraries( test-decoder hulk-fix )
target_link_libraries( test-transport hulk-fix )
