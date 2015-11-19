include(${CMAKE_SOURCE_DIR}/CMake/build.cmake)


project(OpenALAudioSystem)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/debug)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/debug)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/release)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/release)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/release)

add_subdirectory("dependencies/OpenAL")
add_subdirectory("dependencies/alut")

set(OPENAL_AUDIO_PRIVATE_FILES
	"src/OpenALAudioSystem.cpp;"
	)
	
set(OPENAL_AUDIO_PUBLIC_FILES
	"include/OpenALAudioSystem.h;"
	"include/OpenALSoundCue.h;"
	"include/COpenALSoundSource.h;"
	"include/OpenALSoundSystemConfig.h;"
	)

buildmodule(OpenALAudioSystem "${OPENAL_AUDIO_PRIVATE_FILES}${OPENAL_AUDIO_PUBLIC_FILES}")  

add_dependencies(OpenALAudioSystem ManaForgeEngine alut)
target_link_libraries(OpenALAudioSystem ManaForgeEngine alut)

if(WIN32)
	add_dependencies(OpenALAudioSystem OpenAL32)
	target_link_libraries(OpenALAudioSystem OpenAL32)
else()
	add_dependencies(OpenALAudioSystem openal)
	target_link_libraries(OpenALAudioSystem openal)
endif()
