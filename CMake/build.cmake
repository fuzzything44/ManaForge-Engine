macro(__handledependencies TARGET_NAME DEPENDENCY_TO_CONFIGURE)
		
		foreach(MANAGER ${${DEPENDENCY_TO_CONFIGURE}_DEPENDENCIES})
			__handledependencies(${TARGET_NAME} ${MANAGER})
		endforeach()

		if(DEFINED ${DEPENDENCY_TO_CONFIGURE}_COMPILE_DEFINITIONS)
			target_compile_definitions(${TARGET_NAME} ${${DEPENDENCY_TO_CONFIGURE}_COMPILE_DEFINITIONS})
		endif()
		
		if(DEFINED ${DEPENDENCY_TO_CONFIGURE}_COMPILE_FEATURES)
			target_compile_features(${TARGET_NAME} PRIVATE ${${DEPENDENCY_TO_CONFIGURE}_COMPILE_FEATURES})
		endif()
		
		if(DEFINED ${DEPENDENCY_TO_CONFIGURE}_INCLUDE_DIRECTORIES)
			target_include_directories(${TARGET_NAME} PRIVATE ${${DEPENDENCY_TO_CONFIGURE}_INCLUDE_DIRECTORIES})
		endif()
		
		if(DEFINED ${DEPENDENCY_TO_CONFIGURE}_LINK_LIBRARIES)
			target_link_libraries(${TARGET_NAME} ${${DEPENDENCY_TO_CONFIGURE}_LINK_LIBRARIES} ${BASE_MANAGERS})
		endif()
		
		add_dependencies(${TARGET_NAME} ${DEPENDENCY_TO_CONFIGURE})
endmacro()


function(buildmodule MODULE_NAME MODULE_FILES BASE_MANAGERS)

	set(${MODULE_NAME}_DEPENDENCIES ${BASE_MANAGERS} CACHE INTERNAL "DEPENDENCIES. DO NOT EDIT")

	project(${MODULE_NAME})
	
	add_library(${MODULE_NAME} SHARED ${MODULE_FILES})
	
	foreach(MANAGER "${BASE_MANAGERS};${MODULE_NAME}" ${MODULE_NAME})
		__handledependencies(${MODULE_NAME} ${MANAGER})
	endforeach()

	
	set_target_properties(${MODULE_NAME}
		PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
		LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
		RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
	)
	
	target_link_libraries(${MODULE_NAME} Qt5::Widgets Qt5::Gui) 

	
	
	
endfunction(buildmodule)


