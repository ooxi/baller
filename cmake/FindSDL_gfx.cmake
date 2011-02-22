
if(SDLGFX_INCLUDE_DIR)
  # Already in cache, be silent
  set(SDLGFX_FIND_QUIETLY TRUE)
endif(SDLGFX_INCLUDE_DIR)

find_path(SDLGFX_INCLUDE_DIR SDL_gfxPrimitives.h PATH_SUFFIXES SDL)

find_library(SDLGFX_LIBRARY NAMES SDL_gfx)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SDLGFX DEFAULT_MSG
                                  SDLGFX_LIBRARY SDLGFX_INCLUDE_DIR)

mark_as_advanced(SDLGFX_LIBRARY SDLGFX_INCLUDE_DIR)
