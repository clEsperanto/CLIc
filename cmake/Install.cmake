include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Set configuration file names
set(PROJECT_CONFIG_FILE "${PROJECT_NAME}Config.cmake")
set(VERSION_CONFIG_FILE "${PROJECT_NAME}ConfigVersion.cmake")

# Install library targets
install(
    TARGETS ${LIBRARY_NAME} clFFT
    EXPORT ${TARGETS_EXPORT_NAME}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Development
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT RuntimeLibraries
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT RuntimeLibraries
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
)

# Export for the build tree
write_basic_package_version_file(
    "${PROJECT_BINARY_DIR}/${VERSION_CONFIG_FILE}"
    VERSION "${${PROJECT_NAME}_VERSION}"
    COMPATIBILITY SameMajorVersion
)
export(
    EXPORT ${TARGETS_EXPORT_NAME}
    NAMESPACE ${PROJECT_NAME}::
    FILE "${CMAKE_CURRENT_BINARY_DIR}/${TARGETS_EXPORT_NAME}.cmake"
)
configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
    "${PROJECT_BINARY_DIR}/${PROJECT_CONFIG_FILE}"
    INSTALL_DESTINATION "${PROJECT_BINARY_DIR}"
)

# Export for the install tree
set(CONFIG_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")
install(
    EXPORT ${TARGETS_EXPORT_NAME}
    NAMESPACE ${PROJECT_NAME}::
    FILE ${TARGETS_EXPORT_NAME}.cmake
    DESTINATION ${CONFIG_INSTALL_DIR}
)
install(
    FILES "${PROJECT_BINARY_DIR}/${PROJECT_CONFIG_FILE}"
    "${PROJECT_BINARY_DIR}/${VERSION_CONFIG_FILE}"
    DESTINATION ${CONFIG_INSTALL_DIR}
)
