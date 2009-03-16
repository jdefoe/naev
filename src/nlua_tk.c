/*
 * See Licensing and Copyright notice in naev.h
 */

/**
 * @file nlua_tk.c
 *
 * @brief NAEV toolkit Lua module.
 */

#include "nlua_tk.h"

#include <stdlib.h>

#include "lauxlib.h"

#include "nlua.h"
#include "nluadef.h"
#include "log.h"
#include "naev.h"
#include "dialogue.h"


/* toolkit */
static int tk_msg( lua_State *L );
static int tk_yesno( lua_State *L );
static int tk_input( lua_State *L );
static const luaL_reg tk_methods[] = {
   { "msg", tk_msg },
   { "yesno", tk_yesno },
   { "input", tk_input },
   {0,0}
}; /**< Toolkit Lua methods. */



/**
 * @brief Loads the Toolkit Lua library.
 *
 *    @param L Lua state.
 *    @return 0 on success.
 */
int lua_loadTk( lua_State *L )
{
   luaL_register(L, "tk", tk_methods);
   return 0;
}


/**
 * @brief Bindings for interacting with the Toolkit.
 *
 * These toolkit bindings are all blocking, meaning that your Lua code won't
 *  continue executing until the user closes the dialogue that popped up.
 *
 *  A typical example  would be:
 *  @code
 *  tk.msg( "Title", "This is a message." )
 *  if tk.yesno( "YesNo popup box", "Click yes to do something." ) then
 *    -- Player clicked yes, do something
 *  else
 *    -- Player clicked no
 *  end
 *  @endcode
 *
 *  @luamod tk
 */
/**
 * @brief Creates a window with an ok button.
 *
 * @usage tk.msg( "Title", "This is a message." )
 *
 *    @luaparam title Title of the window.
 *    @luaparam message Message to display in the window.
 * @luafunc msg( title, message )
 */
static int tk_msg( lua_State *L )
{  
   char *title, *str;
   NLUA_MIN_ARGS(2);
   
   if (lua_isstring(L,1)) title = (char*) lua_tostring(L,1);
   else NLUA_INVALID_PARAMETER();
   if (lua_isstring(L,2)) str = (char*) lua_tostring(L,2);
   else NLUA_INVALID_PARAMETER();
   
   dialogue_msgRaw( title, str );
   return 0;
}
/**
 * @brief Displays a window with Yes and No buttons.
 *
 * @usage if tk.yesno( "YesNo popup box", "Click yes to do something." ) then -- Clicked yes
 *
 *    @luaparam title Title of the window.
 *    @luaparam message Message to display in the window.
 *    @luareturn true if yes was clicked, false if no was clicked.
 * @luafunc yesno( title, message )
 */
static int tk_yesno( lua_State *L )
{  
   int ret;
   char *title, *str;
   NLUA_MIN_ARGS(2);
   
   if (lua_isstring(L,1)) title = (char*) lua_tostring(L,1);
   else NLUA_INVALID_PARAMETER();
   if (lua_isstring(L,2)) str = (char*) lua_tostring(L,2);
   else NLUA_INVALID_PARAMETER();
   
   ret = dialogue_YesNoRaw( title, str );
   lua_pushboolean(L,ret);
   return 1;
}
/**
 * @brief Creates a window that allows player to write text input.
 *
 * @usage name = tk.input( "Name", 3, 20, "Enter your name:" )
 *
 *    @luaparam title Title of the window.
 *    @luaparam min Minimum characters to accept (must be greater then 0).
 *    @luaparam max Maximum characters to accept.
 *    @luaparam str Text to display in the window.
 *    @luareturn nil if input was canceled or a string with the text written.
 * @luafunc input( title, min, max, str )
 */
static int tk_input( lua_State *L )
{  
   char *title, *str, *ret;
   int min, max;
   NLUA_MIN_ARGS(4);

   if (lua_isstring(L,1)) title = (char*) lua_tostring(L,1);
   else NLUA_INVALID_PARAMETER();
   if (lua_isnumber(L,2)) min = (int) lua_tonumber(L,2);
   else NLUA_INVALID_PARAMETER();
   if (lua_isnumber(L,3)) max = (int) lua_tonumber(L,3);
   else NLUA_INVALID_PARAMETER();
   if (lua_isstring(L,4)) str = (char*) lua_tostring(L,4);
   else NLUA_INVALID_PARAMETER();
   
   ret = dialogue_inputRaw( title, min, max, str );
   if (ret != NULL) {
      lua_pushstring(L, ret);
      free(ret);
   }
   else
      lua_pushnil(L);
   return 1;
}
