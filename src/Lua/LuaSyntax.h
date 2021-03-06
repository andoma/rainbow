// Copyright (c) 2010-present Bifrost Entertainment AS and Tommy Nguyen
// Distributed under the MIT License.
// (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)

#ifndef LUA_LUASYNTAX_H_
#define LUA_LUASYNTAX_H_

#include <utility>

#include "Common/String.h"
#include "Lua/LuaMacros.h"
#include "Platform/Macros.h"

// A 'verify' macro is defined in 'AssertMacros.h' in the macOS SDK.
#ifdef verify
#   undef verify
#endif

#ifndef NDEBUG
#   define LUA_ASSERT(L, expr, ...) rainbow::lua::verify(L, (expr), __VA_ARGS__)
#else
#   define LUA_ASSERT(L, expr, ...) static_cast<void>(0)
#endif

extern "C" {
    struct lua_State;
    extern int (luaL_error)(lua_State* L, const char* fmt, ...);
}

NS_RAINBOW_LUA_BEGIN
{
    namespace detail
    {
        template <typename T>
        void checktype(lua_State* L, int n);

        template <typename T, typename... Args>
        void checkargs(lua_State* L, int n)
        {
            checktype<T>(L, n);
            checkargs<Args...>(L, n + 1);
        }

        template <>
        inline void checkargs<std::nullptr_t>(lua_State*, int)
        {
        }
    }

    template <typename T>
    struct nil_or
    {
    };

    template <typename... Args>
    void checkargs(lua_State* L)
    {
#ifdef NDEBUG
        NOT_USED(L);
#else
        detail::checkargs<Args..., std::nullptr_t>(L, 1);
#endif  // NDEBUG
    }

    template <typename... Args>
    void verify(lua_State* L, bool success, czstring error, Args&&... args)
    {
        if (!success)
            luaL_error(L, error, std::forward<Args>(args)...);
    }
} NS_RAINBOW_LUA_END

#endif  // LUA_LUASYNTAX_H_
