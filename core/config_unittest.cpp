#include "core/config.hpp"

#include "gtest/gtest.h"

namespace {

using namespace husky;

class ConfigTest : public Config {};

class TestConfig : public testing::Test {
   public:
    TestConfig() {}
    ~TestConfig() {}

   protected:
    void SetUp() {}
    void TearDown() {}
};

TEST_F(TestConfig, InitAndDelete) {
    ConfigTest* config = new ConfigTest();
    ASSERT_TRUE(config != NULL);
    delete config;
}

}  // namespace
