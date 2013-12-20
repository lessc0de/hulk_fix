
add_executable( test_decoder test_decoder.cpp )
add_executable( test_session test_session.cpp )
add_executable( test_tcp_acceptor test_tcp_acceptor.cpp )
add_executable( test_tcp_drop_copy test_tcp_drop_copy.cpp )

target_link_libraries( test_decoder hulk_fix )
target_link_libraries( test_session hulk_fix )
target_link_libraries( test_tcp_acceptor hulk_fix )
target_link_libraries( test_tcp_drop_copy hulk_fix )
