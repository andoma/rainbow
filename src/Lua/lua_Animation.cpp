#include "Graphics/Animation.h"
#include "Lua/lua_Sprite.h"
#include "Lua/lua_Animation.h"

const char lua_Animation::class_name[] = "animation";
const LuaMachine::Method<lua_Animation> lua_Animation::methods[] = {
	{ "is_stopped",  &lua_Animation::is_stopped },
	{ "set_delay",   &lua_Animation::set_delay },
	{ "set_fps",     &lua_Animation::set_fps },
	{ "set_frames",  &lua_Animation::set_frames },
	{ "set_sprite",  &lua_Animation::set_sprite },
	{ "play",        &lua_Animation::play },
	{ "stop",        &lua_Animation::stop },
	{ 0, 0 }
};

lua_Animation::lua_Animation(lua_State *L)
{
	assert(lua_gettop(L) == 3 || lua_gettop(L) == 4
	       || !"Rainbow::Lua::Animation: rainbow.animation(sprite, frames{}, fps, loop delay)");

	Sprite *sprite = lua_isnil(L, 1) ? nullptr : LuaMachine::wrapper<lua_Sprite>(L, 1)->raw_ptr();

	// Count number of frames
	unsigned int count = 0;
	lua_pushnil(L);
	while (lua_next(L, 2))
	{
		++count;
		lua_pop(L, 1);
	}

	// Allocate frames array
	unsigned int *const frames = new unsigned int[count + 1];
	unsigned int *frame = frames;

	// Populate frames array
	lua_pushnil(L);
	while (lua_next(L, 2))
	{
		*frame = lua_tointeger(L, -1);
		++frame;
		lua_pop(L, 1);
	}
	frames[count] = 0;

	const unsigned int fps = lua_tointeger(L, 3);
	const int delay = (lua_gettop(L) == 4) ? lua_tointeger(L, 4) : 0;
	this->animation = new Animation(sprite, frames, fps, delay);
}

lua_Animation::~lua_Animation()
{
	delete this->animation;
}

int lua_Animation::is_stopped(lua_State *L)
{
	lua_pushboolean(L, this->animation->is_stopped());
	return 1;
}

int lua_Animation::set_delay(lua_State *L)
{
	assert(lua_gettop(L) == 1 || !"Rainbow::Lua syntax: <animation>:set_loop(delay in milliseconds)");

	this->animation->set_delay(lua_tointeger(L, 1));
	return 0;
}

int lua_Animation::set_fps(lua_State *L)
{
	assert(lua_gettop(L) == 1 || !"Rainbow::Lua syntax: <animation>:set_fps(fps)");

	this->animation->set_timeout(1000.0f / lua_tointeger(L, 1));
	return 0;
}

int lua_Animation::set_frames(lua_State *L)
{
	assert(lua_gettop(L) > 1
	       || !"Rainbow::Lua syntax: <animation>:set_frames(table { count > 1 })");

	const unsigned int framec = lua_gettop(L);
	unsigned int *const frames = new unsigned int[framec + 1];
	for (unsigned int i = 0; i < framec; ++i)
		frames[i] = lua_tointeger(L, i);
	frames[framec] = 0;
	this->animation->set_frames(frames);
	return 0;
}

int lua_Animation::set_sprite(lua_State *L)
{
	assert(lua_gettop(L) == 1 || !"Rainbow::Lua syntax: <animation>:set_sprite(sprite)");

	Sprite *sprite = LuaMachine::wrapper<lua_Sprite>(L, 1)->raw_ptr();
	this->animation->set_sprite(sprite);
	return 0;
}

int lua_Animation::play(lua_State *)
{
	this->animation->start();
	return 0;
}

int lua_Animation::stop(lua_State *)
{
	this->animation->stop();
	this->animation->reset();
	return 0;
}