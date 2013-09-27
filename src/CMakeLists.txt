
add_library( hulk_fix decoder.cpp session.cpp )
target_link_libraries( hulk_fix hulk_core )
install( TARGETS hulk_fix DESTINATION lib )

