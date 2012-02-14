# #####################################
# CMake build file for the translations
# #####################################

if (GETTEXT_FOUND)

	# Generate template file
	add_custom_target(po-template ALL
		DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}.pot)
	add_custom_command(
		OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}.pot
		COMMAND xgettext -k_ -kN_  --package-version=${VERSION}
				--package-name=${CMAKE_PROJECT_NAME}
				-o ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}.pot
				${CMAKE_CURRENT_SOURCE_DIR}/../src/*.c
		COMMAND sed -i 	s/charset=CHARSET/charset=ISO-8859-1/ 
				${CMAKE_PROJECT_NAME}.pot
		DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/../src/*.c
		)

	file(GLOB POFILES *.po)

	foreach(POFILE ${POFILES})
		string(REGEX REPLACE "(.*)/(.*)\\.po" "\\2" LANG ${POFILE})
		string(REPLACE ".po" ".mo" MOFILE ${POFILE})
		add_custom_command(
			OUTPUT ${MOFILE}
			COMMAND ${GETTEXT_MSGFMT_EXECUTABLE} -o ${MOFILE} ${POFILE}
			DEPENDS ${POFILE}
			)
		add_custom_target(mo-file-${LANG} ALL DEPENDS ${MOFILE})
		install(FILES ${MOFILE} RENAME ${CMAKE_PROJECT_NAME}.mo
			DESTINATION ${LOCALEDIR}/${LANG}/LC_MESSAGES)
	endforeach()

endif (GETTEXT_FOUND)
