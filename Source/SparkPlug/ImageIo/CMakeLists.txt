FILE(GLOB Sources "*.h" "*.cpp")
ADD_LIBRARY(sparkplug-imageio ${Sources})

TARGET_LINK_LIBRARIES(sparkplug-imageio sparkplug-core)

FIND_PACKAGE(ZLIB REQUIRED)

FIND_PACKAGE(PNG REQUIRED)
	ADD_DEFINITIONS(${PNG_DEFINITIONS})
	INCLUDE_DIRECTORIES(${ZLIB_INCLUDE_DIRS} ${PNG_INCLUDE_DIR})
	TARGET_LINK_LIBRARIES(sparkplug-imageio ${ZLIB_LIBRARIES} ${PNG_LIBRARIES})


FILE(GLOB PublicHeaders RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.h")
SET_TARGET_PROPERTIES(sparkplug-imageio PROPERTIES PUBLIC_HEADER "${PublicHeaders}")
SET_TARGET_PROPERTIES(sparkplug-imageio PROPERTIES VERSION "${VERSION}")

INSTALL(TARGETS sparkplug-imageio
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    PUBLIC_HEADER DESTINATION "include/SparkPlug/ImageIo"
)

if(UNIX)
    SET(PKG_DEPS "sparkplug-core >= ${VERSION}, libpng >= 1.2")
    SET(PKG_LIBS "")
    SET(LIB_NAME "sparkplug-imageio.${VERSION}")

    CONFIGURE_FILE(
        ${CMAKE_SOURCE_DIR}/sparkplug-imageio.pc.in
        ${CMAKE_BINARY_DIR}/sparkplug-imageio.pc @ONLY
    )

    INSTALL(
        FILES ${CMAKE_BINARY_DIR}/sparkplug-imageio.pc
        DESTINATION lib${LIB_SUFFIX}/pkgconfig
    )
endif()
