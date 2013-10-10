INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_FECAPI fecapi)

FIND_PATH(
    FECAPI_INCLUDE_DIRS
    NAMES fec/fec_api.h
    HINTS $ENV{FECAPI_DIR}/include
        ${PC_FECAPI_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREEFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    FECAPI_LIBRARIES
    NAMES fec
    HINTS $ENV{FECAPI_DIR}/lib
        ${PC_FECAPI_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(FECAPI DEFAULT_MSG FECAPI_LIBRARIES FECAPI_INCLUDE_DIRS)
MARK_AS_ADVANCED(FECAPI_LIBRARIES FECAPI_INCLUDE_DIRS)

