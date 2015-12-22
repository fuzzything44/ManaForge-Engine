
add_definitions(-DHAVE_CONFIG_H=1)

# Without __NO_CTYPE tolower and friends are macros which introduce a GLIBC 2.3
# dependency. By defining this identifier we are currently backwards compatible
# to GLIBC 2.1.3, which is a good thing. In addition, the macros lead to code
# which triggers warnings with -Wunreachable-code.
if(HAVE_GLIBC)
	add_definitions(-D__NO_CTYPE=1)
endif()


if(HAVE_GCC_VISIBILITY)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fvisibility=hidden")
	add_definitions(-DHAVE_GCC_VISIBILITY)
endif()

# So our static library can be linked into shared libraries (for instance,
# gcc on amd64 needs -fPIC)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CMAKE_SHARED_LIBRARY_C_FLAGS}")

set(ALUT_SOURCES
	alutBufferData.c
	alutCodec.c
	alutError.c
	alutInit.c
	alutInputStream.c
	alutLoader.c
	alutOutputStream.c
	alutUtil.c
	alutVersion.c
	alutWaveform.c)

set(ALUT_INTERNAL_HEADERS
	alutInternal.h)
source_group(InternalHeader FILES ${ALUT_INTERNAL_HEADERS})

include_directories(../../OpenAL/include)

set(ALUT_HEADERS
	../include/AL/alut.h)
source_group(APIHeader FILES ${ALUT_HEADERS})


if(BUILD_STATIC)
	# we can't create a static library with the same name
	# as the shared one, so we copy it over after creation
	add_library(alut_static STATIC ${ALUT_SOURCES} ${ALUT_INTERNAL_HEADERS} ${ALUT_HEADERS})
	set_target_properties(alut_static
		PROPERTIES

		ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
		LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
		RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin

	)
	add_dependencies(alut_static openal ${ADD_LIBS})
	if(UNIX)
		target_link_libraries(alut_static m)
	endif()	
	if(NOT WIN32)
		# TODO this is an inelegant hack...
		add_custom_command(TARGET
			alut_static
			POST_BUILD
			COMMAND
			${CMAKE_COMMAND}
			ARGS
			-E
			copy
			${CMAKE_BINARY_DIR}/bin/${CMAKE_STATIC_LIBRARY_PREFIX}alut_static${CMAKE_STATIC_LIBRARY_SUFFIX}
			${CMAKE_BINARY_DIR}/bin/${CMAKE_STATIC_LIBRARY_PREFIX}alut${CMAKE_STATIC_LIBRARY_SUFFIX})
		install_files(/lib${LIB_SUFFIX}
			FILES
			${CMAKE_STATIC_LIBRARY_PREFIX}alut${CMAKE_STATIC_LIBRARY_SUFFIX})
	endif()
endif()


add_library(alut SHARED ${ALUT_SOURCES} ${ALUT_INTERNAL_HEADERS} ${ALUT_HEADERS})
set_property(TARGET
	alut
	PROPERTY
	PUBLIC_HEADER
	${ALUT_HEADERS})
set_target_properties(alut
	PROPERTIES
	VERSION
	${MAJOR_VERSION}
	SOVERSION
	${MAJOR_VERSION})
set_target_properties(alut
	PROPERTIES

	ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
	LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin

)
if(WIN32)
	add_dependencies(alut OpenAL32)
	target_link_libraries(alut "OpenAL32")
else()
	add_dependencies(alut openal)
	target_link_libraries(alut "openal")
endif()
if(UNIX)
	target_link_libraries(alut m)
endif()	

install(TARGETS alut
	LIBRARY DESTINATION lib${LIB_SUFFIX}
	ARCHIVE DESTINATION lib${LIB_SUFFIX}
	RUNTIME DESTINATION bin
	PUBLIC_HEADER DESTINATION include/AL)
