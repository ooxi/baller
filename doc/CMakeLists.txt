
INSTALL(FILES de/anleitung.html en/manual.html  DESTINATION ${DOCDIR})

add_custom_target(manpages ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/ballerburg.6.gz)
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/ballerburg.6.gz
	COMMAND gzip -c -9 ${CMAKE_CURRENT_SOURCE_DIR}/ballerburg.6 > ${CMAKE_CURRENT_BINARY_DIR}/ballerburg.6.gz
	DEPENDS ballerburg.6)
INSTALL(FILES  ${CMAKE_CURRENT_BINARY_DIR}/ballerburg.6.gz DESTINATION ${MANDIR})
