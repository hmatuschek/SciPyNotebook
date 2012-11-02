SET(PYTHON_CONFIG_EXEC_NAMES "python-config")
SET(PYTHON_EXEC_NAMES "python")
FIND_PROGRAM(PYTHON_CONFIG_EXEC NAMES ${PYTHON_CONFIG_EXEC_NAMES})
FIND_PROGRAM(PYTHON_EXEC NAMES ${PYTHON_EXEC_NAMES})

IF(PYTHON_CONFIG_EXEC AND PYTHON_EXEC)
  SET(PYTHON_CONFIG_FOUND TRUE)
  EXEC_PROGRAM(${PYTHON_EXEC} ARGS "-V"
      OUTPUT_VARIABLE PYTHON_VERSION)
ENDIF(PYTHON_CONFIG_EXEC AND PYTHON_EXEC)


IF(PYTHON_CONFIG_FOUND)
  # get linker flags
  EXEC_PROGRAM(${PYTHON_CONFIG_EXEC} ARGS "--ldflags"
    OUTPUT_VARIABLE PYTHON_LD_FLAGS)
    
  # get libraries:
  EXEC_PROGRAM(${PYTHON_CONFIG_EXEC} ARGS "--libs"
    OUTPUT_VARIABLE PYTHON_LIBRARY)

  EXEC_PROGRAM(${PYTHON_CONFIG_EXEC} ARGS "--cflags"
    OUTPUT_VARIABLE PYTHON_C_FLAGS)

  EXEC_PROGRAM(${PYTHON_CONFIG_EXEC} ARGS "--cflags"
    OUTPUT_VARIABLE PYTHON_CXX_FLAGS)
ENDIF(PYTHON_CONFIG_FOUND)



INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(PYTHON DEFAULT_MSG
                                  PYTHON_VERSION PYTHON_INCLUDE_DIR PYTHON_CXX_FLAGS PYTHON_LIBRARY PYTHON_LD_FLAGS)

SET(PYTHON_INCLUDE_DIRS ${PYTHON_INCLUDE_DIR})
SET(PYTHON_LIBRARIES ${PYTHON_LIBRARY} m dl)

mark_as_advanced(PYTHON_VERSION PYTHON_LIBRARY PYTHON_INCLUDE_DIR PYTHON_CXX_FLAGS PYTHON_LD_FLAGS)