file(GLOB SKYNET_TEST_SOURCES "${PROJECT_SOURCE_DIR}/test/*.cpp")

######################################################################################################################
# DEPENDENCIES
######################################################################################################################

######################################################################################################################
# MAKE TARGETS
######################################################################################################################



##########################################
# "make check-tests"
##########################################
add_custom_target(build-tests COMMAND ${CMAKE_CTEST_COMMAND} --show-only)
add_custom_target(check-tests COMMAND ${CMAKE_CTEST_COMMAND} --verbose)

##########################################
# "make server client ..."
##########################################

foreach(skynet_test_source ${SKYNET_TEST_SOURCES})
		# Create a human readable name
		get_filename_component(skynet_test_filename ${skynet_test_source} NAME)
		string(REPLACE ".cpp" "" skynet_test_name ${skynet_test_filename})

		# Add the test target separately and as part of "make check-tests"
		add_executable(${skynet_test_name} EXCLUDE_FROM_ALL ${skynet_test_source})
		add_dependencies(build-tests ${skynet_test_name})
		add_dependencies(check-tests ${skynet_test_name})

		target_link_libraries(${skynet_test_name} skynet_shared)

		# Set test target properties and dependencies
		set_target_properties(${skynet_test_name}
				PROPERTIES	
				RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
				COMMAND ${skynet_test_name}
		)


endforeach(skynet_test_source ${SKYNET_TEST_SOURCES})
