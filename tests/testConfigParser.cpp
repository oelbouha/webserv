/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / /
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/
 *	/____/	User: Youssef Salmi
 *			File: main.cpp
 */


#include <gtest/gtest.h>
#include "include/ConfigParserFactory.hpp"


TEST(config_parser, testCanParseInlineConfigs)
{
	IConfigParser*	parser = factory::makeConfigParser("tests/simple_inline.yml");
	const Config&	conf = parser->parse();

	ASSERT_NO_THROW({
		conf.getInlineConfig("inline_property");
	});

	ASSERT_EQ("value", conf.getInlineConfig("inline_property"));

}

TEST(config_parser, testCanParseListConfigs)
{
	IConfigParser*	parser = factory::makeConfigParser("tests/simple_list.yml");
	const Config&	conf = parser->parse();

	ASSERT_NO_THROW({
		conf.getListConfig("list_property");
	});

	ASSERT_EQ("value1", conf.getListConfig("list_property")[0]);
	ASSERT_EQ("value2", conf.getListConfig("list_property")[1]);

}

TEST(config_parser, testCanParseBlockConfigs)
{
	IConfigParser*	parser = factory::makeConfigParser("tests/simple_block.yml");
	const Config&	conf = parser->parse();
	Config			expected;
	Config*			block_property = new Config();

	block_property->addInline("prop1", "value1");
	block_property->addList("prop2", {"value2", "value3"});

	expected.addBlock("block_property", block_property);

	ASSERT_NO_THROW({
		conf.getBlockConfig("block_property");
	});

	ASSERT_EQ(expected.getBlockConfig("block_property").size(), 
			conf.getBlockConfig("block_property").size());

}

int main(int c, char *v[])
{
	::testing::InitGoogleTest(&c, v);
	return (RUN_ALL_TESTS());
}