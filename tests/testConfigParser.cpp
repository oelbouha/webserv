/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: main.cpp 
 */

#include "tests/test.h"

#include "include/ConfigParserFactory.hpp"



TEST(test_parser_with_a_non_existing_config_file,

	ASSERT_EXCEPTION(
		IConfigParser* parser = factory::makeConfigParser("file");
		(void)parser;
	, std::invalid_argument)

)
 
TEST(test_two_equal_two,
	ASSERT_EQUAL(2, 2);
	// throw std::invalid_argument("invalid");
)

int main( void )
{
	TEST_RUN(test_parser_with_a_non_existing_config_file);
	TEST_RUN(test_two_equal_two);
	return (0);
}
