# ================================================================== #
# ================================================== # Target

set(APP_TARGET client)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS include/*.hpp ${CMAKE_BINARY_DIR}/include/*.hpp)
file(GLOB_RECURSE PRIVATE_HEADERS CONFIGURE_DEPENDS src/*.hpp)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_executable(${APP_TARGET} ${HEADERS} ${PRIVATE_HEADERS} ${SOURCES})
set_target_properties(${APP_TARGET} PROPERTIES LINKER_LANGUAGE CXX)
target_compile_features(${APP_TARGET} PRIVATE cxx_std_20)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	# target_compile_options(${APP_TARGET} ... )
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	target_compile_options(${APP_TARGET} PRIVATE -W -Wall -Wextra -Werror -pedantic)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Intel")
	# target_compile_options(${APP_TARGET} ... )
elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	target_compile_options(${APP_TARGET} PRIVATE /W4 /WX)
endif()

target_include_directories(${APP_TARGET}
	PUBLIC
		$<INSTALL_INTERFACE:include>
		$<INSTALL_INTERFACE:include>
		$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(${APP_TARGET} light)

# ================================================================== #
# ================================================== # Dependencies

# find_package(...)
# target_link_libraries(${APP_TARGET} ...)
# target_include_directories(${APP_TARGET} ...)

# ================================================================== #
# ================================================== # Installation

INSTALL (
	DIRECTORY ${CMAKE_BINARY_DIR}/include
	DESTINATION include/${APP_TARGET}
	FILES_MATCHING PATTERN "*.hpp"
)
INSTALL (
	DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include
	DESTINATION include/${APP_TARGET}
	FILES_MATCHING PATTERN "*.hpp"
)
install(
	TARGETS ${APP_TARGET} EXPORT ${PROJECT_NAME}Targets
	LIBRARY       DESTINATION lib
	ARCHIVE       DESTINATION lib
	RUNTIME       DESTINATION bin
	INCLUDES      DESTINATION include/${APP_TARGET}
)

# ================================================================== #
# ================================================== # Cmake Packaging

include(CMakePackageConfigHelpers)

set(CONF_INSTALL_DIR  "${CMAKE_INSTALL_PREFIX}/lib/cmake/${PROJECT_NAME}")
set(CONF_INCLUDE_DIRS "${CMAKE_INSTALL_PREFIX}/include/${APP_TARGET}")

configure_package_config_file(${PROJECT_SOURCE_DIR}/cmake/PackageConfig.cmake.in
	${PROJECT_BINARY_DIR}/to_install/${PROJECT_NAME}Config.cmake
	INSTALL_DESTINATION ${CONF_INSTALL_DIR}
	PATH_VARS CONF_INCLUDE_DIRS CONF_INSTALL_DIR CMAKE_INSTALL_PREFIX
)

write_basic_package_version_file(${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
	VERSION ${${PROJECT_NAME}_VERSION}
	COMPATIBILITY AnyNewerVersion
)

install(FILES
	"${PROJECT_BINARY_DIR}/to_install/${PROJECT_NAME}Config.cmake"
	"${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
	DESTINATION ${CONF_INSTALL_DIR}
)

install(EXPORT
	${PROJECT_NAME}Targets
	NAMESPACE ${PROJECT_NAME}::
	DESTINATION ${CONF_INSTALL_DIR}
)