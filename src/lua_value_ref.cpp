#include "lua_value_ref.hpp"
#include "detail/lua_error.hpp"
#include <chrono>
#include <random>
#include <utility>

namespace luabz
{
const int lua_value_ref::top = -1;
const char lua_value_ref::lua_table_field_delimeter = '.';
std::vector<std::function<int(lua_State*)>> lua_value_ref::registered_functions;
std::unordered_map<lua_State*, std::vector<int>>
    lua_value_ref::file_registered_functions;
lua_value_ref::lua_var_loader::lua_var_loader(lua_State* st,
                                              const std::string& variable_name,
                                              int& us)
  : state{st}, used_space{us}
{
    if (variable_name.find(lua_table_field_delimeter) != std::string::npos) {
        auto delimeter_position =
            variable_name.find_first_of(lua_table_field_delimeter);
        std::string table_name = variable_name.substr(0, delimeter_position);
        std::string field_name = variable_name.substr(delimeter_position + 1);
        lua_getglobal(state, table_name.c_str());
        ++used_space;
        while (true) {
            delimeter_position =
                field_name.find_first_of(lua_table_field_delimeter);
            bool field_reached = (delimeter_position == std::string::npos);
            if (field_reached) {
                lua_getfield(state, top, field_name.c_str());
                ++used_space;
                return;
            }
            std::string parent_field_name =
                field_name.substr(0, delimeter_position);
            field_name = field_name.substr(delimeter_position + 1);
            lua_getfield(state, top, parent_field_name.c_str());
            ++used_space;
            if (!lua_istable(state, top)) {
                std::string error_message = parent_field_name;
                error_message += " is not a table, and cannot contain the "
                                 "following  fields ";
                error_message += field_name;
                error_message += std::to_string(used_space);
                error_message += "\nOriginal string: ";
                error_message += variable_name;
                error_message += "\n";
                detail::lua_error(error_message);
            }
        }
    }
    lua_getfield(state, LUA_GLOBALSINDEX, variable_name.c_str());
    ++used_space;
}
lua_value_ref::lua_var_loader::~lua_var_loader()
{
    lua_pop(state, used_space);
    used_space = 0;
}
lua_value_ref::lua_value_ref(lua_State* state, std::string name)
  : m_state{state}, m_name{std::move(name)}, used_stack_spaces{0}
{
}

lua_value_ref::lua_value_ref(const lua_value_ref& rhs)
  : m_state{rhs.m_state}, m_name{rhs.m_name}, used_stack_spaces{0}
{
}
/**
 * We have two cases when confronting two lua_value_ref variables \n
 * 1) Both variable are on the same script side, in this case we just simple
 * perform load_lua_var on both variable and call lua_equal with the
 * corresponding indices 2) The variables are on two different script file, in
 * this case we first load the two variable then we perform a xmove which
 * permits us to move data from one lua stack to another lua stack, data is
 * moved from rhs into this.stack. Once we have performed the xmove we as always
 * call lua_equal
 */
bool lua_value_ref::operator==(const lua_value_ref& rhs) const
{
    return call_lua_operator(rhs, lua_equal);
}
/**
 * We have two cases when confronting two lua_value_ref variables \n
 * 1) Both variable are on the same script side, in this case we just simple
 * perform load_lua_var on both variable and call lua_lessthan with the
 * corresponding indices 2) The variables are on two different script file, in
 * this case we first load the two variable then we perform a xmove which
 * permits us to move data from one lua stack to another lua stack, data is
 * moved from rhs into this.stack. Once we have performed the xmove we as always
 * call lua_lessthan
 */
bool lua_value_ref::operator<(const lua_value_ref& rhs) const
{
    return call_lua_operator(rhs, lua_lessthan);
}
/**
 * Helper function for pushing the varaible associated by the this object and
 * the variable associated with the rhs object and performing a call to a
 * specific lua_operator on the previously loaded values.
 * \note All C++ operator should forward to this function when they want to
 * perform a lua operator operation \param rhs The right hand side object we are
 * comparing againts \param lua_operator The lua operator that must be called
 */
bool lua_value_ref::call_lua_operator(const lua_value_ref& rhs,
                                      int (*lua_operator)(lua_State* l,
                                                          int index1,
                                                          int index2)) const
{
    int lhs_index = -2, rhs_index = -1;
    lua_var_loader lhs_loader{m_state, m_name, used_stack_spaces};
    lua_var_loader rhs_loader{rhs.m_state, rhs.m_name, rhs.used_stack_spaces};
    if (m_state != rhs.m_state) {
        lua_xmove(rhs.m_state, m_state, 1);
        ++used_stack_spaces;
    }
    auto result =
        static_cast<bool>(lua_operator(m_state, lhs_index, rhs_index));
    return result;
}
bool lua_value_ref::is_nil() const
{
    lua_var_loader lhs_loader{m_state, m_name, used_stack_spaces};
    auto result = static_cast<bool>(lua_isnil(m_state, top));
    return result;
}
/**
 * \details Pops the top element from the state and sets the value of the
 * variable identified by m_name to the popped value.
 * \sa operator=(const T& new_value)
 * \note The object_table_index represent the index of any user defined table
 * for which we want to set a field to a particular value. See operator=(const
 * T& new_value) for why the value is -3
 */
void lua_value_ref::set_lua_var()
{
    static const int object_table_index = -3;
    int lua_table_index =
        (is_table_field()) ? object_table_index : LUA_GLOBALSINDEX;
    std::string lua_variable_name =
        (is_table_field()) ? get_field_name() : m_name;
    lua_setfield(m_state, lua_table_index, lua_variable_name.c_str());
}
lua_value_ref lua_value_ref::operator[](const std::string& field_name) const
{
    return this->operator[](field_name.c_str());
}
lua_value_ref lua_value_ref::operator[](const char* field_name) const
{
    lua_var_loader lhs_loader{m_state, m_name, used_stack_spaces};
    if (!lua_istable(m_state, top)) {
        luabz::detail::lua_error("What you are trying to access is not a lua "
                                 "table so it cannot be accessed as a table");
    }
    std::string full_name = m_name + lua_table_field_delimeter + field_name;
    lua_value_ref result(m_state, full_name);
    return result;
}
bool lua_value_ref::is_table_field() const
{
    return m_name.find(lua_table_field_delimeter) != std::string::npos;
}
std::string lua_value_ref::get_field_name() const
{
    auto delimeter_position = m_name.find_last_of(lua_table_field_delimeter);
    return m_name.substr(delimeter_position + 1);
}
std::string lua_value_ref::generate_return_value_name() const
{
    std::uniform_int_distribution<int> distribution;
    auto seed = std::chrono::steady_clock::now().time_since_epoch().count();
    std::default_random_engine dre(seed);
    std::string generated_name = m_name + "_";
    generated_name += std::to_string(distribution(dre));
    return generated_name;
}
/**
 * \note The index at which the function is saved inside registredFunctions
 * becomes an upvalue of the lua_CFunction
 */
void lua_value_ref::register_function(std::size_t function_position_index)
{
    lua_var_loader lhs_loader{m_state, m_name, used_stack_spaces};
    detail::lua_value<decltype(function_position_index)>::insert(
        m_state, function_position_index);
    lua_CFunction new_value = [](lua_State* functionState) -> int {
        int functionIndex = lua_tointeger(
            functionState, lua_upvalueindex(1));  // retrieve  upvalue
        auto& function = registered_functions.at(functionIndex);
        return function(functionState);
    };
    detail::lua_value<lua_CFunction>::insert(m_state, new_value, 1);
    set_lua_var();
}
}  // namespace luabz