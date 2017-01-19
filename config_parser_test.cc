#include "gtest/gtest.h"
#include "config_parser.h"

TEST(NginxConfigParserTest, SimpleConfig) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("example_config", &out_config);

  EXPECT_TRUE(success);
}

class NginxParsingConfigTest : public ::testing::Test {
protected:
	bool ParseString(const std::string config_string) {
		std::stringstream config_stream(config_string);
		return parser_.Parse(&config_stream, &out_config_);
	}
	NginxConfigParser parser_;
	NginxConfig out_config_;
};

TEST(NginxConfigTest, ToStringTest) {
	NginxConfigStatement statement;
	statement.tokens_.push_back("one");
	statement.tokens_.push_back("two");
	EXPECT_EQ(statement.ToString(1), "  one two;\n");
}

TEST_F(NginxParsingConfigTest, commentsRecognized) {
	EXPECT_TRUE(ParseString("one; #comment here\n two;"));
}

TEST_F(NginxParsingConfigTest, missingSemicolon){
  EXPECT_FALSE(ParseString("one "));
}

TEST_F(NginxParsingConfigTest, emptyToken){
  EXPECT_FALSE(ParseString(";"));
}

TEST_F(NginxParsingConfigTest, missingClosingBracket){
  EXPECT_FALSE(ParseString("one {;"));
}


// BUG DETECTED FROM THE FOLLOWING TEST CASE:
TEST_F(NginxParsingConfigTest, missingOpeningBracket){
  EXPECT_FALSE(ParseString("one ;}"));
}

TEST_F(NginxParsingConfigTest, tokenWithinBlock){
  EXPECT_TRUE(ParseString("one { two;}"));
}

TEST_F(NginxParsingConfigTest, missingSemicolonInBlock){
  EXPECT_FALSE(ParseString("one {two}"));
}

TEST_F(NginxParsingConfigTest, incompatibleBrackets){
  EXPECT_FALSE(ParseString("{two)"));
}

TEST_F(NginxParsingConfigTest, emptyBlock){
  EXPECT_FALSE(ParseString("{}"));
}

TEST_F(NginxParsingConfigTest, outBlockUnclosed){
  EXPECT_FALSE(ParseString("{one { two;}"));
}
