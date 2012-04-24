
INSTALL(FILES de/anleitung.html  DESTINATION ${DOCDIR})

add_custom_target(manpages ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/ballerburg.1.gz)
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/ballerburg.1.gz
	COMMAND gzip -c -9 ${CMAKE_CURRENT_SOURCE_DIR}/ballerburg.1 > ${CMAKE_CURRENT_BINARY_DIR}/ballerburg.1.gz
	DEPENDS ballerburg.1)
INSTALL(FILES  ${CMAKE_CURRENT_BINARY_DIR}/ballerburg.1.gz DESTINATION ${MANDIR})
