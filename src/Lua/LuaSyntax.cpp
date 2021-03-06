// Copyright (c) 2010-present Bifrost Entertainment AS and Tommy Nguyen
// Distributed under the MIT License.
// (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)

#ifndef NDEBUG

#include "Lua/LuaSyntax.h"

#include <lua.hpp>

#include "Lua/LuaHelper.h"

namespace rainbow
{
    class IDrawable;

    namespace audio
    {
        struct Channel;
        struct Sound;
    }

    namespace lua
    {
        class Animation;
        class Font;
        class Label;
        class RenderQueue;
        class Shader;
        class Sprite;
        class SpriteBatch;
        class Texture;

        namespace shaders
        {
            class Diffuse;
        }
    }
}  // rainbow

namespace b2 { namespace lua
{
    class Body;
    class CircleShape;
    class Contact;
    class EdgeShape;
    class Fixture;
    class PolygonShape;
    class World;
}}  // namespace b2::lua

namespace spine { namespace lua
{
    class Skeleton;
}}

using rainbow::czstring;

namespace
{
    bool iswrappeduserdata(lua_State* L, int n)
    {
        return rainbow::lua::isuserdata(L, n) || rainbow::lua::istable(L, n);
    }

    int type_error(lua_State* L, int n, czstring type)
    {
        czstring msg = lua_pushfstring(
            L, "%s expected, got %s", type, luaL_typename(L, n));
        return luaL_argerror(L, n, msg);
    }

    template <typename F>
    void optional(lua_State* L, int n, F&& is_type, czstring type)
    {
        if (!lua_isnoneornil(L, n) && !is_type(L, n))
            type_error(L, n, type);
    }

    template <typename F>
    void require(lua_State* L, int n, F&& is_type, czstring type)
    {
        if (!is_type(L, n))
            type_error(L, n, type);
    }
}

namespace rainbow { namespace lua { namespace detail
{
    /* bool */

    template <>
    void checktype<nil_or<bool>>(lua_State* L, int n)
    {
        optional(L, n, isboolean, "nil or boolean");
    }

    template <>
    void checktype<bool>(lua_State* L, int n)
    {
        require(L, n, isboolean, "boolean");
    }

    /* char* */

    template <>
    void checktype<nil_or<char*>>(lua_State* L, int n)
    {
        optional(L, n, lua_isstring, "nil or string");
    }

    template <>
    void checktype<char*>(lua_State* L, int n)
    {
        require(L, n, lua_isstring, "string");
    }

    /* void */

    template <>
    void checktype<nil_or<void*>>(lua_State* L, int n)
    {
        optional(L, n, istable, "table");
    }

    template <>
    void checktype<void*>(lua_State* L, int n)
    {
        require(L, n, istable, "table");
    }

    /* IDrawable */

    template <>
    void checktype<IDrawable>(lua_State* L, int n)
    {
        require(L, n, iswrappeduserdata, "drawable");
    }

    /* rainbow::audio */

    template <>
    void checktype<audio::Channel>(lua_State* L, int n)
    {
        require(L, n, istable, "channel");
    }

    template <>
    void checktype<audio::Sound>(lua_State* L, int n)
    {
        require(L, n, istable, "sound");
    }

    /* lua_Number */

    template <>
    void checktype<nil_or<lua_Number>>(lua_State* L, int n)
    {
        optional(L, n, isnumber, "nil or number");
    }

    template <>
    void checktype<lua_Number>(lua_State* L, int n)
    {
        require(L, n, isnumber, "number");
    }

    /* rainbow::lua::Animation */

    template <>
    void checktype<Animation>(lua_State* L, int n)
    {
        require(L, n, iswrappeduserdata, "animation");
    }

    /* rainbow::lua::Font */

    template <>
    void checktype<Font>(lua_State* L, int n)
    {
        require(L, n, iswrappeduserdata, "font");
    }

    /* rainbow::lua::Label */

    template <>
    void checktype<Label>(lua_State* L, int n)
    {
        require(L, n, iswrappeduserdata, "label");
    }

    /* rainbow::lua::RenderQueue */

    template <>
    void checktype<RenderQueue>(lua_State* L, int n)
    {
        require(L, n, iswrappeduserdata, "renderqueue");
    }

    /* rainbow::lua::Shader */

    template <>
    void checktype<Shader>(lua_State* L, int n)
    {
        if (!((isnumber(L, n) && lua_tointeger(L, n) == 0) || isuserdata(L, n)))
        {
            luaL_argerror(L, n, "shader");
        }
    }

    /* rainbow::lua::Sprite */

    template <>
    void checktype<nil_or<Sprite>>(lua_State* L, int n)
    {
        optional(L, n, iswrappeduserdata, "nil or sprite");
    }

    template <>
    void checktype<Sprite>(lua_State* L, int n)
    {
        require(L, n, iswrappeduserdata, "sprite");
    }

    /* rainbow::lua::SpriteBatch */

    template <>
    void checktype<SpriteBatch>(lua_State* L, int n)
    {
        require(L, n, iswrappeduserdata, "sprite batch");
    }

    /* rainbow::lua::Texture */

    template <>
    void checktype<Texture>(lua_State* L, int n)
    {
        require(L, n, iswrappeduserdata, "texture");
    }

    /* rainbow::lua::shaders::Diffuse */

    template <>
    void checktype<shaders::Diffuse>(lua_State* L, int n)
    {
        require(L, n, iswrappeduserdata, "shaders.diffuse");
    }

    /* Box2D */

    template <>
    void checktype<b2::lua::Body>(lua_State* L, int n)
    {
        luaL_checkudata(L, n, "b2Body");
    }

    template <>
    void checktype<b2::lua::CircleShape>(lua_State* L, int n)
    {
        luaL_checkudata(L, n, "CircleShape");
    }

    template <>
    void checktype<b2::lua::Contact>(lua_State* L, int n)
    {
        luaL_checkudata(L, n, "b2Contact");
    }

    template <>
    void checktype<b2::lua::EdgeShape>(lua_State* L, int n)
    {
        luaL_checkudata(L, n, "EdgeShape");
    }

    template <>
    void checktype<b2::lua::Fixture>(lua_State* L, int n)
    {
        luaL_checkudata(L, n, "b2Fixture");
    }

    template <>
    void checktype<b2::lua::PolygonShape>(lua_State* L, int n)
    {
        luaL_checkudata(L, n, "PolygonShape");
    }

    template <>
    void checktype<b2::lua::World>(lua_State* L, int n)
    {
        luaL_checkudata(L, n, "World");
    }

    /* Spine */

    template <>
    void checktype<spine::lua::Skeleton>(lua_State* L, int n)
    {
        luaL_checkudata(L, n, "skeleton");
    }
}}}  // namespace rainbow::lua::detail

#endif  // NDEBUG
