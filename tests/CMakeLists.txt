add_test(
    NAME byteroute.smoke
    COMMAND byteroute
)

add_executable(
    byteroute_test_byteorder
    test_byteorder.c
)

target_link_libraries(
    byteroute_test_byteorder
    PRIVATE
        ByteRoute::Core
)

byteroute_set_project_warnings(byteroute_test_byteorder)

add_test(
    NAME byteroute.byteorder
    COMMAND byteroute_test_byteorder
)

add_executable(
    byteroute_test_checksum
    test_checksum.c
)

target_link_libraries(
    byteroute_test_checksum
    PRIVATE
        ByteRoute::Core
)

byteroute_set_project_warnings(byteroute_test_checksum)

add_test(
    NAME byteroute.checksum
    COMMAND byteroute_test_checksum
)

add_executable(
    byteroute_test_packet
    test_packet.c
)

target_link_libraries(
    byteroute_test_packet
    PRIVATE
        ByteRoute::Core
)

byteroute_set_project_warnings(byteroute_test_packet)

add_test(
    NAME byteroute.packet
    COMMAND byteroute_test_packet
)