#include <string>
#include "gtest/gtest.h"
#include "lua_script.hpp"
#include "lua_test_helpers.hpp"

class lua_value_Get : public ::testing::Test
{
  public:
    luabz::lua_script script;
    void SetUp() override
    {
        std::string lua_script_file =
            construct_script_path("luascript_test.lua");
        script.open(lua_script_file);
    }
};
TEST_F(lua_value_Get, LuaBoolAsCppBool)
{
    bool t = script["boolt_var"];
    bool f = script["boolf_var"];
    ASSERT_TRUE(t);
    ASSERT_FALSE(f);
}
TEST_F(lua_value_Get, LuaStringAsCppString)
{
    std::string expected = "This is some string";
    std::string actual = script["string_var"];
    ASSERT_TRUE(expected == actual);
}
TEST_F(lua_value_Get, LuaStringAsCppChar)
{
    char expected = 'T';
    char actual = script["string_var"];
    ASSERT_TRUE(expected == actual);
}
TEST_F(lua_value_Get, LuaIntegerAsCppUnsignedLongLong)
{
    unsigned long long integer_var = script["integer_var"];
    unsigned long long expected = 100;
    ASSERT_EQ(expected, integer_var);
}
TEST_F(lua_value_Get, LuaIntegerAsCppLongLong)
{
    long long integer_var = script["integer_var"];
    long long expected = 100;
    ASSERT_EQ(expected, integer_var);
}
TEST_F(lua_value_Get, LuaIntegerAsCppUnsignedLong)
{
    unsigned long integer_var = script["integer_var"];
    unsigned long expected = 100;
    ASSERT_EQ(expected, integer_var);
}
TEST_F(lua_value_Get, LuaIntegerAsCppLong)
{
    long integer_var = script["integer_var"];
    long expected = 100;
    ASSERT_EQ(expected, integer_var);
}
TEST_F(lua_value_Get, LuaIntegerAsCppInt)
{
    int integer_var = script["integer_var"];
    ASSERT_EQ(100, integer_var);
}
TEST_F(lua_value_Get, LuaIntegerAsCppUnsignedInt)
{
    unsigned int integer_var = script["integer_var"];
    unsigned int expected = 100;
    ASSERT_EQ(expected, integer_var);
}
TEST_F(lua_value_Get, LuaIntegerAsCppUnsignedShort)
{
    unsigned short integer_var = script["integer_var"];
    unsigned short expected = 100;
    ASSERT_EQ(expected, integer_var);
}
TEST_F(lua_value_Get, LuaIntegerAsCppShort)
{
    short integer_var = script["integer_var"];
    short expected = 100;
    ASSERT_EQ(expected, integer_var);
}
TEST_F(lua_value_Get, LuaNumberAsCppDouble)
{
    double double_var = script["double_var"];
    ASSERT_DOUBLE_EQ(102.0351, double_var);
}
TEST_F(lua_value_Get, LuaNumberAsCppFloat)
{
    float double_var = script["double_var"];
    ASSERT_FLOAT_EQ(102.0351f, double_var);
}