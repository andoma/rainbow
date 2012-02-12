set(LUA_INCLUDE_DIR ${LOCAL_LIBRARY}/Lua)

# These definitions are copied from Lua's Makefile
set(LUA_CORE_SRC lapi.c lcode.c lctype.c ldebug.c ldo.c ldump.c lfunc.c lgc.c llex.c
                 lmem.c lobject.c lopcodes.c lparser.c lstate.c lstring.c ltable.c
                 ltm.c lundump.c lvm.c lzio.c)
set(LUA_LIB_SRC  lauxlib.c lbaselib.c lbitlib.c lcorolib.c ldblib.c liolib.c
                 lmathlib.c loslib.c lstrlib.c ltablib.c loadlib.c linit.c)

# Loop through the source list and prefix the project source directory
foreach(file ${LUA_CORE_SRC} ${LUA_LIB_SRC})
	set(LUA_SRC ${LUA_SRC} ${PROJECT_SOURCE_DIR}/${LUA_INCLUDE_DIR}/${file})
endforeach()

add_definitions(-DLUA_COMPAT_ALL)
add_library(lua STATIC ${LUA_SRC})