cmake_minimum_required( VERSION 2.8 )
project( hulk_fix )

include_directories( include )
include_directories( $ENV{HULK_INSTALL_DIR}/hulk-core/include )
link_directories( $ENV{HULK_INSTALL_DIR}/hulk-core/lib )
add_subdirectory( src )
add_subdirectory( test )

install( FILES include/hulk/fix/decoder.h DESTINATION include/hulk/fix )
install( FILES include/hulk/fix/message.h DESTINATION include/hulk/fix )
install( FILES include/hulk/fix/session.h DESTINATION include/hulk/fix )
install( FILES include/hulk/fix/tcp.h DESTINATION include/hulk/fix )
install( FILES include/hulk/fix/transport.h DESTINATION include/hulk/fix )
