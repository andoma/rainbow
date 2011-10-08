#ifndef LUA_INPUT_H_
#define LUA_INPUT_H_

#include "lua.h"
#include "Input/Input.h"

class lua_Input
{
public:
	static void accelerate(lua_State *L);

	static void key_down(lua_State *L, const Key &key);
	static void key_up(lua_State *L, const Key &key);

	static void touch_began(lua_State *L, const Touch *const touches, const unsigned int count);
	static void touch_canceled(lua_State *L);
	static void touch_ended(lua_State *L, const Touch *const touches, const unsigned int count);
	static void touch_moved(lua_State *L, const Touch *const touches, const unsigned int count);

private:
	static void getfield(lua_State *L, const char *const field);
	static void init(lua_State *L);
	static void key_event(lua_State *L, const char *const type, const Key &key);
	static void touch_event(lua_State *L, const char *const type, const Touch *const touches = 0, const unsigned int count = 0);

	lua_Input();
	lua_Input(const lua_Input &);
	lua_Input& operator=(const lua_Input &);

	friend class LuaMachine;
};

#endif
