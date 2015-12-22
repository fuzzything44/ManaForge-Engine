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

set(OpenALAudioSystem_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/include;${CMAKE_CURRENT_SOURCE_DIR}/dependencies/alut/include;${CMAKE_CURRENT_SOURCE_DIR}/dependencies/OpenAL/include;" CACHE INTERNAL "")

buildmodule(OpenALAudioSystem "${OPENAL_AUDIO_PRIVATE_FILES}${OPENAL_AUDIO_PUBLIC_FILES}" "ManaForgeEngine")



if(WIN32)
	add_dependencies(OpenALAudioSystem OpenAL32 alut_static)
	target_link_libraries(OpenALAudioSystem OpenAL32 alut_static)
else()
#	add_dependencies(OpenALAudioSystem openal alut_static)
	target_link_libraries(OpenALAudioSystem openal alut_static)
endif()
	