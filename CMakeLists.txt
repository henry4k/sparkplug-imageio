AddModule(${ModulePrefix}ImageIo)
TARGET_LINK_LIBRARIES(${ModulePrefix}ImageIo ${ModulePrefix}Common)

RequiredPackage(ZLIB)
RequiredPackage(PNG)
	ADD_DEFINITIONS(${PNG_DEFINITIONS})
	AddIncludeDirs(${ModulePrefix}ImageIo ${ZLIB_INCLUDE_DIRS} ${PNG_INCLUDE_DIR})
	TARGET_LINK_LIBRARIES(${ModulePrefix}ImageIo ${ZLIB_LIBRARIES} ${PNG_LIBRARIES})

InstallModule(${ModulePrefix}ImageIo)