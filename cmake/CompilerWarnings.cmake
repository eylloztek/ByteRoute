function(byteroute_set_project_warnings target)
    if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(
            ${target}
            PRIVATE
                -Wall
                -Wextra
                -Wpedantic
                -Werror
                -Wshadow
                -Wformat=2
                -Wundef
                -Wstrict-prototypes
        )
    endif()
endfunction()