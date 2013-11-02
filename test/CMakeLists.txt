
set( CMAKE_CXX_FLAGS  "-std=c++11" )

#add_executable( test-client test-client.cpp )
#add_executable( test-decoder test-decoder.cpp )
#add_executable( test_transport test_transport.cpp )
#add_executable( test_server test_server.cpp )
#add_executable( test3 test3.cpp )
#add_executable( xxx xxx.cpp )

#target_link_libraries( test-client hulk-core hulk-fix )
#target_link_libraries( test-decoder hulk-fix )
#target_link_libraries( test_transport hulk_fix )
#target_link_libraries( test_server hulk_fix )
#target_link_libraries( test3 hulk_fix )
#target_link_libraries( xxx hulk_fix )

add_executable( test_session test_session.cpp )
target_link_libraries( test_session hulk_fix )
