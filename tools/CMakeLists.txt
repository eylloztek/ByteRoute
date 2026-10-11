add_executable(
    byteroute_tap_probe
    tap_probe.c
)

target_link_libraries(
    byteroute_tap_probe
    PRIVATE
        ByteRoute::Core
)

byteroute_set_project_warnings(byteroute_tap_probe)