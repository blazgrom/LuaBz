#ifndef LUABZ_LUA_STATE_FACTORY_HPP
#define LUABZ_LUA_STATE_FACTORY_HPP

#include <lua.hpp>
#include <string>
#include <unordered_map>

namespace luabz
{
namespace detail
{
/**
 * \brief A factory used to generate/retrieve new lua_State*
 *
 * The generation of new lua states uses the following logic.
 * We have one master state which is unique for whole application, whenever the
 * user wants to open a new lua file, we create a new thread (lua_State) on the
 * master state and insert it in the master state's registry in order to
 * guarantee that our state want be collected by Lua's garbage collector. Before
 * returning the new thread (lua_State) do the caller we map the state to the
 * name of the lua file. This is done for the following reasons:\n
 * 1 - We can have two differnt lua_scripts that operate on the same lua
 * file,sharing data through lua.\n 2 - We don't have to lookup the state in the
 * master state's registry whenever we can two consecutive calls for the same
 * file\n If a lua file has already been open, the old lua state associated with
 * the file is returned \todo Decide if we want to delete some of the thread
 * inside the master state's registry after a while, and decide the deletion
 * should be performed\n
 */
class lua_state_factory
{
  public:
    /// Generates new lua_state to be used by lua_script
    /**
     *
     * \note Every new lua_thread is pushed onto the registry in order \n
     * to not allow GC from Lua
     * \param file_name The name of the lua file on which the new lua state
     * \param load_std Whether or not to load lua's std
     * operates
     * \note The first type this function is called with file_name that has not
     * yet been used, the file is loaded. All subsequent calls use the already
     * loaded filestatic
     */
    static lua_State* get_lua_state(const std::string& file_name,
                                    bool load_std = false);
    /**
     * \brief Open all standard library on a lua file
     * \param file_name The name of the lua file
     * \pre There must be an already opened lua state asssociated with the
     * file_name
     */
    static void open_standard_library(const std::string& file_name);

  private:
    /**
     * \brief Sets the global table of the new state
     * \param state The state for which we want to change the globaltable
     * \note Has the following logic \n
     * 1 ) Insert the new global table and the metatable of the former \n
     * 2 ) Replace the current global table with the new global table
     */
    static void set_state_globaltable(lua_State* state);
    /**
     * \brief Retrieves the lua script associated with file_name from
     * master_state's registry
     */
    static lua_State* get_loaded_lua_state(const std::string& file_name);
    /**
     * \brief Creates new lua_State which is associated with the script file
     * file_name and inserts the lua_Sate in active_lua_states
     */
    static lua_State* create_new_lua_state(const std::string& file_name,
                                           bool load_std);
};
}  // namespace detail
}  // namespace luabz
#endif