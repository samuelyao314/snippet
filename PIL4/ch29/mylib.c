#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

#include "lua.h"
#include "lauxlib.h"

#include "util.h"


// Exercise 29.1
static int l_summation(lua_State *L) {
    double s;

    int i;
    for (i = 1; i <= lua_gettop(L); i++) {
        double d = luaL_checknumber(L, i);
        s = d + s;
    }

    lua_pushnumber(L, s);
    return 1;
}

// Exercise 29.2
static int l_pack(lua_State *L) {
    int n = lua_gettop(L);

    lua_newtable(L);
    int i;
    for (i = 1; i <= n; i++) {
        lua_pushvalue(L, i);
        lua_seti(L, -2, i);
    }

    lua_pushinteger(L, n);
    lua_setfield(L, -2, "n");

    return 1;
}

static int l_sin(lua_State *L) {
    double d = luaL_checknumber(L, 1);
    lua_pushnumber(L, sin(d));
    return 1;
}

static int l_dir(lua_State *L) {
    DIR *dir;
    struct dirent *entry;
    int i;
    const char *path = luaL_checkstring(L, 1);

    dir = opendir(path);
    if (dir == NULL) {
        lua_pushnil(L);
        lua_pushstring(L, strerror(errno));
        return 2;
    }
    lua_newtable(L);
    i = 1;
    while ((entry = readdir(dir)) != NULL) {
        lua_pushinteger(L, i++);
        lua_pushstring(L, entry->d_name);
        lua_settable(L, -3);
    }

    closedir(dir);
    return 1;
}

static int l_reverse(lua_State *L) {
    int n = lua_gettop(L);

    int i;
    for (i = 1; i <= n/2; i++) {
        int j = (n+1-i);
        // swap
        lua_pushvalue(L, i);
        lua_pushvalue(L, j);
        lua_replace(L, i);
        lua_replace(L, j);
    }

    return n;
}

static int l_foreach(lua_State *L) {
    luaL_checktype(L, 1, LUA_TTABLE);
    luaL_checktype(L, 2, LUA_TFUNCTION);

    lua_pushnil(L);
    while (lua_next(L, 1) != 0) {
        lua_pushvalue(L, 2);
        lua_pushvalue(L, 3);
        lua_pushvalue(L, 4);
        int error  = lua_pcall(L, 2, 0, 0);
        if (error) {
            lua_error(L);
        }
        lua_pop(L, 1);
    }

    return 0;
}

static int l_map(lua_State *L) {
    int i, n;

    luaL_checktype(L, 1, LUA_TTABLE);
    luaL_checktype(L, 2, LUA_TFUNCTION);

    n = luaL_len(L, 1);
    for (i = 1; i <= n; i++) {
        lua_pushvalue(L, 2);
        lua_geti(L, 1, i);
        lua_call(L, 1, 1);
        lua_seti(L, 1, i);
    }
    return 0;
}

static int l_split(lua_State *L) {
    const char *s = luaL_checkstring(L, 1);   /* subject */
    const char *sep = luaL_checkstring(L, 2); /* separator */

    lua_newtable(L);
    char *e;
    int i = 1;
    while ((e = strchr(s, *sep)) != NULL) {
        lua_pushlstring(L, s, e - s);
        lua_rawseti(L, -2, i++);
        s = e + 1;
    }
    lua_pushstring(L, s);
    lua_rawseti(L, -2, i);
    return 1;
}

static int l_upper(lua_State *L) {
    size_t l;
    size_t i;
    luaL_Buffer b;
    const char *s = luaL_checklstring(L, 1, &l);
    char *p = luaL_buffinitsize(L, &b, l);
    for (i = 0; i < l; i++) {
         p[i] = toupper(s[i]);
    }
    luaL_pushresultsize(&b, l);
    return 1;
}

static int l_concat(lua_State *L) {
    int i, n;
    luaL_checktype(L, 1, LUA_TTABLE);
    n = luaL_len(L, 1);
    luaL_Buffer b;
    luaL_buffinit(L, &b);
    for (i = 1; i <= n; i++) {
        lua_geti(L, 1, i);
        luaL_addvalue(&b);
    }
    luaL_pushresult(&b);
    return 1;
}

static int l_counter(lua_State *L) {
    int val = lua_tointeger(L, lua_upvalueindex(1));
    lua_pushinteger(L, ++val);
    lua_copy(L, -1, lua_upvalueindex(1));
    return 1;
}

static int newCounter(lua_State *L) {
    lua_pushinteger(L, 0);
    lua_pushcclosure(L, &l_counter, 1);
    return 1;
}


static const struct luaL_Reg mylib[] = {
    {"dir", l_dir},
    {"sin", l_sin},
    {"summation", l_summation},
    {"pack", l_pack},
    {"reverse", l_reverse},
    {"foreach", l_foreach},
    {"map", l_map},
    {"split", l_split},
    {"upper", l_upper},
    {"concat", l_concat},
    {"newCounter", newCounter},
    {NULL, NULL},
};


int luaopen_mylib (lua_State *L) {
    luaL_newlib(L, mylib);
    return 1;
}
