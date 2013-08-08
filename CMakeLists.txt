cmake_minimum_required( VERSION 2.8 )
project( hulk-fix )

include_directories( include )
add_subdirectory( src )

install( FILES include/hulk/fix/decoder.h DESTINATION include/hulk/fix )
install( FILES include/hulk/fix/message.h DESTINATION include/hulk/fix )
install( FILES include/hulk/fix/session.h DESTINATION include/hulk/fix )
