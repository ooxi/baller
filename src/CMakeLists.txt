

include_directories(${CMAKE_BINARY_DIR} ${SDL_INCLUDE_DIR})


add_executable (ballerburg baller1.c baller2.c ballergui.c cannoneer.c screen.c
			dlgAlert.c sdlgui.c)

target_link_libraries(ballerburg ${SDL_LIBRARY} SDL_gfx)

if(SDLMAIN_LIBRARY)
	target_link_libraries(ballerburg ${SDLMAIN_LIBRARY})
endif(SDLMAIN_LIBRARY)
