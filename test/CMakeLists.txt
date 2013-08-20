add_executable( test-client test-client.cpp )
add_executable( test-decoder test-decoder.cpp )

target_link_libraries( test-client hulk-core hulk-fix )
target_link_libraries( test-decoder hulk-core hulk-fix )
