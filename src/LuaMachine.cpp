// Copyright 2011-12 Bifrost Entertainment. All rights reserved.

#include "Common/Data.h"
#include "Lua/lua_Animation.h"
#include "Lua/lua_Audio.h"
#include "Lua/lua_Font.h"
#include "Lua/lua_Input.h"
#include "Lua/lua_Label.h"
#include "Lua/lua_Platform.h"
#include "Lua/lua_Random.h"
#include "Lua/lua_SceneGraph.h"
#include "Lua/lua_Sprite.h"
#include "Lua/lua_SpriteBatch.h"
#include "Lua/lua_Texture.h"

#ifdef USE_PHYSICS
#	include "Lua/lua_Physics.h"
#endif

void LuaMachine::dump_stack(lua_State *L)
{
	printf("Lua stack size: %i\n", lua_gettop(L));
	for (int l = 1; l <= lua_gettop(L); ++l)
	{
		printf("#%i: ", l);
		switch (lua_type(L, l))
		{
			case LUA_TNIL:
				puts("(nil)");
				break;
			case LUA_TNUMBER:
				printf(LUA_NUMBER_FMT "\n", lua_tonumber(L, l));
				break;
			case LUA_TBOOLEAN:
				printf("%s\n", lua_toboolean(L, l) ? "true" : "false");
				break;
			case LUA_TSTRING:
				printf("%s\n", lua_tolstring(L, l, nullptr));
				break;
			case LUA_TTABLE:
				puts("(table)");
				break;
			case LUA_TFUNCTION:
				printf("%p\n", lua_tocfunction(L, l));
				break;
			case LUA_TUSERDATA:
				printf("%p\n", lua_touserdata(L, l));
				break;
			case LUA_TTHREAD:
				puts("(thread)");
				break;
			case LUA_TLIGHTUSERDATA:
				printf("%p\n", lua_topointer(L, l));
				break;
			default:
				puts("(unknown)");
				break;
		}
	}
}

lua_Debug* LuaMachine::getinfo(lua_State *L)
{
	lua_Debug *d = new lua_Debug();
	lua_getstack(L, 1, d);
	lua_getinfo(L, "nSl", d);
	return d;
}

LuaMachine::~LuaMachine()
{
	delete this->scenegraph;
	lua_close(this->L);
}

LuaMachine::LuaMachine() : scenegraph(nullptr), L(luaL_newstate())
{
	luaL_openlibs(this->L);

	// Initialize "rainbow" namespace
	lua_createtable(this->L, 0, 16);
	lua_pushvalue(this->L, -1);
	lua_setglobal(this->L, "rainbow");

	lua_Platform::init(this->L);  // Initialize "rainbow.platform" namespace
	lua_Random::init(this->L);    // Initialize "rainbow.random" function
	lua_Input::init(this->L);     // Initialize "rainbow.input" namespace
	lua_Audio::init(this->L);     // Initialize "rainbow.audio" namespace

#ifdef USE_PHYSICS
	lua_Physics::init(this->L);   // Initialize "rainbow.physics" namespace
#endif

	// Initialize "rainbow.scenegraph"
	this->scenegraph = new lua_SceneGraph(this->L);

	// Bind C++ objects
	wrap<lua_Animation>(this->L);
	wrap<lua_Font>(this->L);
	wrap<lua_Label>(this->L);
	wrap<lua_Sprite>(this->L);
	wrap<lua_SpriteBatch>(this->L);
	wrap<lua_Texture>(this->L);

	// Clean up the stack
	lua_pop(this->L, 1);

	// We need to set LUA_PATH
	const char *const bundle = Data::get_path();

	lua_getglobal(this->L, "package");
	lua_getfield(this->L, -1, "path");

	size_t path_len = 0;
	const char *const pkg_path = lua_tolstring(this->L, -1, &path_len);
	char *const lua_path = new char[strlen(bundle) + 8 + path_len];
	strcpy(lua_path, bundle);
	strcat(lua_path, "/?.lua;");
	strcat(lua_path, pkg_path);

	lua_pushstring(this->L, lua_path);
	Data::free(lua_path);
	lua_setfield(this->L, -3, "path");
	lua_pop(this->L, 2);

	R_ASSERT(lua_gettop(L) == 0, "Stack not empty");
}

int LuaMachine::call(const char *const k, int nargs, int nresults)
{
	R_ASSERT(lua_gettop(L) == nargs, "call: Number of arguments != stack size");

	lua_getglobal(this->L, k);
	lua_insert(this->L, 1);
	const int lua_e = lua_pcall(this->L, nargs, nresults, 0);
	if (lua_e)
		this->err(lua_e);
	return lua_e;
}

void LuaMachine::err(const int lua_e)
{
	R_ASSERT(lua_e != LUA_OK, "err: No error to report");

	const char *err_general = "general";
	const char *err_runtime = "runtime";
	const char *err_syntax  = "syntax";
	const char *err_memory  = "memory allocation";
	const char *err_errfunc = "error handling";

	const char *desc = err_general;
	const char *const m = lua_tolstring(this->L, -1, nullptr);
	lua_pop(this->L, 1);
	switch (lua_e)
	{
		case LUA_ERRRUN:
			desc = err_runtime;
			dump_stack(this->L);
			break;
		case LUA_ERRSYNTAX:
			desc = err_syntax;
			dump_stack(this->L);
			break;
		case LUA_ERRMEM:
			desc = err_memory;
			dump_stack(this->L);
			break;
		case LUA_ERRERR:
			desc = err_errfunc;
			break;
		default:
			break;
	}
	fprintf(stderr, "Lua %s error: %s\n", desc, m);
}

int LuaMachine::load(SceneGraph::Node *root, const char *const lua)
{
	// Load Lua script
	int lua_e = luaL_loadfile(this->L, lua);
	if (lua_e || (lua_e = lua_pcall(this->L, 0, LUA_MULTRET, 0)))
		this->err(lua_e);
	else
		this->scenegraph->set_root(root);
	return lua_e;
}

int LuaMachine::update(const unsigned long t)
{
	lua_pushinteger(this->L, t);
	return this->call("update", 1);
}
