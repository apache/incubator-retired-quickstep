# - Try to find the GNU sasl library (gsasl)
#
# Once done this will define
#
#  GSASL_FOUND - System has gnutls
#  GSASL_INCLUDE_DIR - The gnutls include directory
#  GSASL_LIBRARY - The libraries needed to use gnutls


IF (GSASL_INCLUDE_DIR AND GSASL_LIBRARY)
	# in cache already
  SET(GSASL_FIND_QUIETLY TRUE)
ENDIF (GSASL_INCLUDE_DIR AND GSASL_LIBRARY)

FIND_PATH(GSASL_INCLUDE_DIR gsasl.h)

FIND_LIBRARY(GSASL_LIBRARY gsasl)

INCLUDE(FindPackageHandleStandardArgs)

# handle the QUIETLY and REQUIRED arguments and set GSASL_FOUND to TRUE if
# all listed variables are TRUE
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GSASL DEFAULT_MSG GSASL_LIBRARY GSASL_INCLUDE_DIR)

MARK_AS_ADVANCED(GSASL_INCLUDE_DIR GSASL_LIBRARY)
