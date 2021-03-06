#include "lua_test_helpers.hpp"
#include "luabz.hpp"
#include <gtest/gtest.h>
#include <string>

class value_TableTest : public ::testing::Test
{
  public:
    luabz::script script{construct_script_path("luascript_test.lua")};
    void TearDown() override { script.close(); }
};
TEST_F(value_TableTest, GettingOneLevelDepthTableField)
{
    bool value = script["TableLevelOne"]["x"];
    ASSERT_FALSE(value);
}
TEST_F(value_TableTest, GettingTwoLevelDepthTableField)
{
    bool value = script["TableLevelOne"]["TableLevelTwo"]["x"];
    ASSERT_TRUE(value);
}
TEST_F(value_TableTest, GettingThreeLevelDepthTableField)
{
    bool value = script["TableLevelOne"]["TableLevelTwo"]["TableLevelThree"]["x"];
    ASSERT_FALSE(value);
}

TEST_F(value_TableTest, GettingFourLevelDepthTableField)
{
    bool value = script["TableLevelOne"]["TableLevelTwo"]["TableLevelThree"]["TableLevelFour"]["x"];
    ASSERT_TRUE(value);
}
TEST_F(value_TableTest, GettingFiveLevelDepthTableField)
{
    bool value = script["TableLevelOne"]["TableLevelTwo"]["TableLevelThree"]["TableLevelFour"]
                       ["TableLevelFive"]["x"];
    ASSERT_FALSE(value);
}
TEST_F(value_TableTest, GettingSixLevelDepthTableField)
{
    bool value = script["TableLevelOne"]["TableLevelTwo"]["TableLevelThree"]["TableLevelFour"]
                       ["TableLevelFive"]["TableLevelSix"]["x"];
    ASSERT_TRUE(value);
}
TEST_F(value_TableTest, GettingSevenLevelDepthTableField)
{
    bool value = script["TableLevelOne"]["TableLevelTwo"]["TableLevelThree"]["TableLevelFour"]
                       ["TableLevelFive"]["TableLevelSix"]["TableLevelSeven"]["x"];
    ASSERT_FALSE(value);
}
TEST_F(value_TableTest, GettingEightLevelDepthTableField)
{
    bool value =
        script["TableLevelOne"]["TableLevelTwo"]["TableLevelThree"]["TableLevelFour"]
              ["TableLevelFive"]["TableLevelSix"]["TableLevelSeven"]["TableLevelEight"]["x"];
    ASSERT_TRUE(value);
}
TEST_F(value_TableTest, GettingNineLevelDepthTableField)
{
    bool value = script["TableLevelOne"]["TableLevelTwo"]["TableLevelThree"]["TableLevelFour"]
                       ["TableLevelFive"]["TableLevelSix"]["TableLevelSeven"]["TableLevelEight"]
                       ["TableLevelNine"]["x"];
    ASSERT_FALSE(value);
}
TEST_F(value_TableTest, GettingTenLevelDepthTableField)
{
    bool value = script["TableLevelOne"]["TableLevelTwo"]["TableLevelThree"]["TableLevelFour"]
                       ["TableLevelFive"]["TableLevelSix"]["TableLevelSeven"]["TableLevelEight"]
                       ["TableLevelNine"]["TableLevelTen"]["x"];
    ASSERT_TRUE(value);
}
TEST_F(value_TableTest, SettingOneLevelDepthTableField)
{
    script["TableLevelOne"]["x"] = true;
    bool value = script["TableLevelOne"]["x"];
    ASSERT_TRUE(value);
}
TEST_F(value_TableTest, SettingTwoLevelDepthTableField)
{
    script["TableLevelOne"]["TableLevelTwo"]["x"] = false;
    bool value = script["TableLevelOne"]["TableLevelTwo"]["x"];
    ASSERT_FALSE(value);
}
TEST_F(value_TableTest, SettingThreeLevelDepthTableField)
{
    script["TableLevelOne"]["TableLevelTwo"]["TableLevelThree"]["x"] = true;
    bool value = script["TableLevelOne"]["TableLevelTwo"]["TableLevelThree"]["x"];
    ASSERT_TRUE(value);
}

TEST_F(value_TableTest, SettingFourLevelDepthTableField)
{
    script["TableLevelOne"]["TableLevelTwo"]["TableLevelThree"]["TableLevelFour"]["x"] = false;
    bool value = script["TableLevelOne"]["TableLevelTwo"]["TableLevelThree"]["TableLevelFour"]["x"];
    ASSERT_FALSE(value);
}
TEST_F(value_TableTest, SettingFiveLevelDepthTableField)
{
    script["TableLevelOne"]["TableLevelTwo"]["TableLevelThree"]["TableLevelFour"]["TableLevelFive"]
          ["x"] = true;
    bool value = script["TableLevelOne"]["TableLevelTwo"]["TableLevelThree"]["TableLevelFour"]
                       ["TableLevelFive"]["x"];
    ASSERT_TRUE(value);
}
TEST_F(value_TableTest, SettingSixLevelDepthTableField)
{
    script["TableLevelOne"]["TableLevelTwo"]["TableLevelThree"]["TableLevelFour"]["TableLevelFive"]
          ["TableLevelSix"]["x"] = false;
    bool value = script["TableLevelOne"]["TableLevelTwo"]["TableLevelThree"]["TableLevelFour"]
                       ["TableLevelFive"]["TableLevelSix"]["x"];
    ASSERT_FALSE(value);
}
TEST_F(value_TableTest, SettingSevenLevelDepthTableField)
{
    script["TableLevelOne"]["TableLevelTwo"]["TableLevelThree"]["TableLevelFour"]["TableLevelFive"]
          ["TableLevelSix"]["TableLevelSeven"]["x"] = true;
    bool value = script["TableLevelOne"]["TableLevelTwo"]["TableLevelThree"]["TableLevelFour"]
                       ["TableLevelFive"]["TableLevelSix"]["TableLevelSeven"]["x"];
    ASSERT_TRUE(value);
}
TEST_F(value_TableTest, SettingEightLevelDepthTableField)
{
    script["TableLevelOne"]["TableLevelTwo"]["TableLevelThree"]["TableLevelFour"]["TableLevelFive"]
          ["TableLevelSix"]["TableLevelSeven"]["TableLevelEight"]["x"] = false;
    bool value =
        script["TableLevelOne"]["TableLevelTwo"]["TableLevelThree"]["TableLevelFour"]
              ["TableLevelFive"]["TableLevelSix"]["TableLevelSeven"]["TableLevelEight"]["x"];
    ASSERT_FALSE(value);
}
TEST_F(value_TableTest, SettingNineLevelDepthTableField)
{
    script["TableLevelOne"]["TableLevelTwo"]["TableLevelThree"]["TableLevelFour"]["TableLevelFive"]
          ["TableLevelSix"]["TableLevelSeven"]["TableLevelEight"]["TableLevelNine"]["x"] = true;
    bool value = script["TableLevelOne"]["TableLevelTwo"]["TableLevelThree"]["TableLevelFour"]
                       ["TableLevelFive"]["TableLevelSix"]["TableLevelSeven"]["TableLevelEight"]
                       ["TableLevelNine"]["x"];
    ASSERT_TRUE(value);
}
TEST_F(value_TableTest, SettingTenLevelDepthTableField)
{
    script["TableLevelOne"]["TableLevelTwo"]["TableLevelThree"]["TableLevelFour"]["TableLevelFive"]
          ["TableLevelSix"]["TableLevelSeven"]["TableLevelEight"]["TableLevelNine"]["TableLevelTen"]
          ["x"] = false;
    bool value = script["TableLevelOne"]["TableLevelTwo"]["TableLevelThree"]["TableLevelFour"]
                       ["TableLevelFive"]["TableLevelSix"]["TableLevelSeven"]["TableLevelEight"]
                       ["TableLevelNine"]["TableLevelTen"]["x"];
    ASSERT_FALSE(value);
}