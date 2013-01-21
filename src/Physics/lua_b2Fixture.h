namespace b2
{
	namespace Lua
	{
		int Filter(lua_State *L)
		{
			lua_createtable(L, 0, 3);

			lua_rawsetfield(L, lua_pushinteger, 0x0001, "categoryBits");
			lua_rawsetfield(L, lua_pushinteger, 0xFFFF, "maskBits");
			lua_rawsetfield(L, lua_pushinteger, 0, "groupIndex");

			return 1;
		}

		void parse_Filter(lua_State *L, b2Filter &filter)
		{
			const char type[] = "Filter";
			static_cast<void>(type);

			lua_rawgetfield(L, "categoryBits", type);
			filter.categoryBits = lua_tointeger(L, -1);
			lua_pop(L, 1);

			lua_rawgetfield(L, "maskBits", type);
			filter.maskBits = lua_tointeger(L, -1);
			lua_pop(L, 1);

			lua_rawgetfield(L, "groupIndex", type);
			filter.groupIndex = lua_tointeger(L, -1);
			lua_pop(L, 1);
		}

		int FixtureDef(lua_State *L)
		{
			lua_createtable(L, 0, 6);

			lua_rawsetnilfield(L, "shape");
			lua_rawsetfield(L, lua_pushnumber, 0.2f, "friction");
			lua_rawsetfield(L, lua_pushnumber, 0.0f, "restitution");
			lua_rawsetfield(L, lua_pushnumber, 0.0f, "density");
			lua_rawsetfield(L, lua_pushboolean, false, "isSensor");

			lua_pushliteral(L, "filter");
			Filter(L);
			lua_rawset(L, -3);

			return 1;
		}

		void parse_FixtureDef(lua_State *L, b2FixtureDef &def)
		{
			const char type[] = "FixtureDef";
			static_cast<void>(type);

			lua_rawgetfield(L, "shape", type);
			def.shape = parse_Shape(L);
			lua_pop(L, 1);

			lua_rawgetfield(L, "friction", type);
			def.friction = lua_tonumber(L, -1);
			lua_pop(L, 1);

			lua_rawgetfield(L, "restitution", type);
			def.restitution = lua_tonumber(L, -1);
			lua_pop(L, 1);

			lua_rawgetfield(L, "density", type);
			def.density = lua_tonumber(L, -1);
			lua_pop(L, 1);

			lua_rawgetfield(L, "isSensor", type);
			def.isSensor = lua_toboolean(L, -1);
			lua_pop(L, 1);

			lua_rawgetfield(L, "filter", type);
			parse_Filter(L, def.filter);
			lua_pop(L, 1);
		}

		class Fixture
		{
			friend class Contact;

		public:
			static const char class_name[];
			static const Rainbow::Lua::Method<Fixture> methods[];

			Fixture(lua_State *);

			int get_body(lua_State *);

		private:
			b2Fixture *fixture;
		};

		const char Fixture::class_name[] = "Fixture";
		const Rainbow::Lua::Method<Fixture> Fixture::methods[] = {
			{ "GetBody",  &Fixture::get_body },
			{ nullptr,    nullptr }
		};

		Fixture::Fixture(lua_State *) : fixture(nullptr) { }

		int Fixture::get_body(lua_State *L)
		{
			lua_rawgeti(L, LUA_REGISTRYINDEX, g_body_list);
			lua_pushinteger(L, reinterpret_cast<lua_Integer>(this->fixture->GetBody()));
			lua_gettable(L, -2);
			lua_remove(L, -2);
			R_ASSERT(lua_istable(L, 1), "Body wasn't properly registered");
			return 1;
		}
	}
}