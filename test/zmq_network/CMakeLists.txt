# configure executables for network tests with zmq

# daemon tests that exit immediately
add_test(NAME daemon_arg_help COMMAND ${DAEMON_EXECUTABLE} --help)
set_tests_properties(daemon_arg_help PROPERTIES
                     PASS_REGULAR_EXPRESSION "Show help message")

add_test(NAME daemon_arg_version COMMAND ${DAEMON_EXECUTABLE} --version)
set_tests_properties(daemon_arg_version PROPERTIES
                     PASS_REGULAR_EXPRESSION "${DAEMON_EXECUTABLE} v")

add_test(NAME daemon_arg_license COMMAND ${DAEMON_EXECUTABLE} --license)
set_tests_properties(daemon_arg_license PROPERTIES
                     PASS_REGULAR_EXPRESSION "Copyright.*Neroshop")

add_test(NAME daemon_badarg COMMAND ${DAEMON_EXECUTABLE} asdf)
set_tests_properties(daemon_badarg PROPERTIES
                     PASS_REGULAR_EXPRESSION "too many positional options")

# cli tests that don't require daemon
file(TO_NATIVE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cli.status.not_connected" _in)
add_test(NAME cli_status_not_connected
         COMMAND sh -c "$<TARGET_FILE:${CLI_EXECUTABLE}> < ${_in}")
set_tests_properties(cli_status_not_connected PROPERTIES
                     PASS_REGULAR_EXPRESSION "Not connected")

file(TO_NATIVE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cli.version" _in)
add_test(NAME cli_version
         COMMAND sh -c "$<TARGET_FILE:${CLI_EXECUTABLE}> < ${_in}")
set_tests_properties(cli_version PROPERTIES
                     PASS_REGULAR_EXPRESSION "${CLI_EXECUTABLE} v")

file(TO_NATIVE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cli.license" _in)
add_test(NAME cli_license
         COMMAND sh -c "$<TARGET_FILE:${CLI_EXECUTABLE}> < ${_in}")
set_tests_properties(cli_license PROPERTIES
                     PASS_REGULAR_EXPRESSION "Copyright.*Neroshop")

add_test(NAME cli_badarg COMMAND ${CLI_EXECUTABLE} asdf)
set_tests_properties(cli_badarg PROPERTIES
                     PASS_REGULAR_EXPRESSION "No command line arguments")

# configure helper executable to kill daemon
add_executable(kill_daemon kill_daemon.cpp)

# start daemon in background, run cli tests, kill daemon
add_test(NAME daemon_detach COMMAND ${DAEMON_EXECUTABLE} --detach --port 55091)
set_tests_properties(daemon_detach PROPERTIES
                     PASS_REGULAR_EXPRESSION "Forked PID")

file(TO_NATIVE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cli.help" _in)
add_test(NAME cli_help
         COMMAND sh -c "$<TARGET_FILE:${CLI_EXECUTABLE}> < ${_in}")
set_tests_properties(cli_help PROPERTIES
                     PASS_REGULAR_EXPRESSION "Commands available")

file(TO_NATIVE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cli.connect.55091" _in)
add_test(NAME cli_connect
         COMMAND sh -c "$<TARGET_FILE:${CLI_EXECUTABLE}> < ${_in}")
set_tests_properties(cli_connect PROPERTIES
                     PASS_REGULAR_EXPRESSION "Connected")

file(TO_NATIVE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cli.status.connected" _in)
add_test(NAME cli_status_connected
         COMMAND sh -c "$<TARGET_FILE:${CLI_EXECUTABLE}> < ${_in}")
set_tests_properties(cli_status_connected PROPERTIES
                     PASS_REGULAR_EXPRESSION "Connected")

add_test(NAME kill_daemon_connect COMMAND kill_daemon)
set_tests_properties(kill_daemon_connect PROPERTIES
                     PASS_REGULAR_EXPRESSION "Killing PID")

set_property(TEST daemon_detach PROPERTY FIXTURES_SETUP daemon_connect)
set_tests_properties(cli_help PROPERTIES FIXTURES_REQUIRED daemon_connect)
set_tests_properties(cli_connect PROPERTIES FIXTURES_REQUIRED daemon_connect)
set_tests_properties(cli_status_connected
                     PROPERTIES FIXTURES_REQUIRED daemon_connect)
set_property(TEST kill_daemon_connect PROPERTY FIXTURES_CLEANUP daemon_connect)
