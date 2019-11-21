cmake_minimum_required (VERSION 2.8)
project(jcu-random CXX)

# <CONAN BUILD>

option(WITH_EXAMPLE "Build example." OFF)

include(CheckSymbolExists)
check_symbol_exists(time time.h HAVE_TIME_H)
check_symbol_exists(GetTickCount windows.h HAVE_GETTICKCOUNT)
check_symbol_exists(gettimeofday sys/time.h HAVE_GETTIMEOFDAY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/config-intl.h.in ${CMAKE_CURRENT_BINARY_DIR}/config-intl.h)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(SRC_FILES
		${CMAKE_CURRENT_SOURCE_DIR}/include/jcu-random/random.h
		${CMAKE_CURRENT_SOURCE_DIR}/include/jcu-random/random-provider.h
		${CMAKE_CURRENT_SOURCE_DIR}/include/jcu-random/well512-random.h
		${CMAKE_CURRENT_SOURCE_DIR}/include/jcu-random/secure-random.h
		${CMAKE_CURRENT_SOURCE_DIR}/include/jcu-random/secure-random-factory.h
		${CMAKE_CURRENT_SOURCE_DIR}/src/random-intl.h
		${CMAKE_CURRENT_SOURCE_DIR}/src/random-intl.cc
		${CMAKE_CURRENT_SOURCE_DIR}/src/random.cc
		${CMAKE_CURRENT_SOURCE_DIR}/src/secure-random-factory.cc
		${CMAKE_CURRENT_SOURCE_DIR}/src/well512-random.cc
		${CMAKE_CURRENT_SOURCE_DIR}/src/digest/digest.h
		${CMAKE_CURRENT_SOURCE_DIR}/src/digest/sha1_digest.h
		${CMAKE_CURRENT_SOURCE_DIR}/src/digest/sha1_digest.cc
		${CMAKE_CURRENT_BINARY_DIR}/config-intl.h
		)

if(WIN32)
	set(SRC_PLATFORM_FILES
			${CMAKE_CURRENT_SOURCE_DIR}/src/windows/windows-random-provider.cc
			)
endif()
if(UNIX)
	set(SRC_PLATFORM_FILES
			${CMAKE_CURRENT_SOURCE_DIR}/src/linux/urandom-random-provider.cc
			)
endif()

add_library(${PROJECT_NAME} ${SRC_FILES} ${SRC_PLATFORM_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>)

if(WIN32)
	target_link_libraries(${PROJECT_NAME} Advapi32.lib)
endif()

add_subdirectory(sample-app)

if(WITH_EXAMPLE)
	add_subdirectory(example)
endif()
