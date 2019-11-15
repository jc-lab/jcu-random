cmake_minimum_required (VERSION 2.8)
project(jcu-random CXX)

# <CONAN BUILD>

include(CheckSymbolExists)
check_symbol_exists(time time.h HAVE_TIME_H)
check_symbol_exists(GetTickCount windows.h HAVE_GETTICKCOUNT)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/config-intl.h.in ${CMAKE_CURRENT_BINARY_DIR}/config-intl.h)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(SRC_FILES
		${CMAKE_CURRENT_SOURCE_DIR}/include/jcu-random/random.h
		${CMAKE_CURRENT_SOURCE_DIR}/include/jcu-random/well512-random.h
		${CMAKE_CURRENT_SOURCE_DIR}/include/jcu-random/secure-random.h
		${CMAKE_CURRENT_SOURCE_DIR}/include/jcu-random/secure-random-factory.h
		${CMAKE_CURRENT_SOURCE_DIR}/src/random-intl.h
		${CMAKE_CURRENT_SOURCE_DIR}/src/random-intl.cc
		${CMAKE_CURRENT_SOURCE_DIR}/src/random.cc
		${CMAKE_CURRENT_SOURCE_DIR}/src/well512-random.cc
		${CMAKE_CURRENT_BINARY_DIR}/config-intl.h
		)

if(WIN32)
	set(SRC_PLATFORM_FILES
			${CMAKE_CURRENT_SOURCE_DIR}/src/windows/win-secure-random-factory.cc
			)
endif()
if(UNIX)
	set(SRC_PLATFORM_FILES
			${CMAKE_CURRENT_SOURCE_DIR}/src/linux/urandom-secure-random-factory.cc
			)
endif()

add_library(${PROJECT_NAME} ${SRC_FILES} ${SRC_PLATFORM_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>)

if(WIN32)
	target_link_libraries(${PROJECT_NAME} Advapi32.lib)
endif()
