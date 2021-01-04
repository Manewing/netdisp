function(pyfile NAME)
  add_custom_command(
    OUTPUT
    ${PYTHON_OUT_DIR}/${NAME}.py
    DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/${NAME}.py
    COMMAND
    cp -a ${CMAKE_CURRENT_SOURCE_DIR}/${NAME}.py
          ${PYTHON_OUT_DIR}/${NAME}.py
  )
  list(APPEND PYFILES ${PYTHON_OUT_DIR}/${NAME}.py)
  set(PYFILES ${PYFILES} PARENT_SCOPE)
endfunction(pyfile)

add_subdirectory(netdisp)