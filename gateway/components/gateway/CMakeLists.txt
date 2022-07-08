set(srcs "src/gateway_common.c")
set(requires "")
set(include_dirs "include")
set(priv_includes "priv_inc")
set(priv_srcs "")

if (CONFIG_GATEWAY_EXTERNAL_NETIF_STATION OR CONFIG_GATEWAY_DATA_FORWARDING_NETIF_SOFTAP)
    list(APPEND srcs "src/gateway_wifi.c")
endif()

list(APPEND requires "json")

idf_component_register(SRCS "${srcs}" "${priv_srcs}"
                       INCLUDE_DIRS "${include_dirs}"
                       PRIV_INCLUDE_DIRS "${priv_includes}"
                       REQUIRES "${requires}")

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/priv_src")
    execute_process(COMMAND git rev-parse --short HEAD
                OUTPUT_VARIABLE PRIV_SRC_COMMIT_ID
                OUTPUT_STRIP_TRAILING_WHITESPACE
                WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/priv_src")
endif()

execute_process(COMMAND git rev-parse --short HEAD
            OUTPUT_VARIABLE SRC_COMMIT_ID
            OUTPUT_STRIP_TRAILING_WHITESPACE)

set_source_files_properties(SOURCE ${srcs}
                            PROPERTIES COMPILE_DEFINITIONS
                            "COMMIT_ID=\"${SRC_COMMIT_ID}\"")

set_source_files_properties(SOURCE ${priv_srcs}
                            PROPERTIES COMPILE_DEFINITIONS
                            "COMMIT_ID=\"${PRIV_SRC_COMMIT_ID}\"")
