include(${CMAKE_SOURCE_DIR}/CMake/build.cmake)


project(OpenALAudioSystem)

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
