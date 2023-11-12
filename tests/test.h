#ifndef TESTS_TEST_HPP
#define TESTS_TEST_HPP

#include <iostream>
#include <string>

#define TEST_NAME(name) test_name(#name)

#define TEST(name, code) 									\
void	name(){ 											\
	std::cout << TEST_NAME(name) << " ::: ";				\
	try {													\
		code												\
	}catch(...){											\
		std::cout << " Unhandeled Exception." << std::endl; \
	} 														\
}

#define TEST_RUN(name)	name()


#define	ASSERT_EXCEPTION(code, exception)	\
try{ \
	code \
} catch(exception&) { \
	std::cout << "\tpass" << std::endl; \
} catch(...){ \
	std::cout << "\tfail" << std::endl; \
}


#define	ASSERT_EQUAL(one, two) 				\
	if (one == two)							\
		std::cout << "passed" << std::endl;	\
	else									\
		std::cout << "failed" << std::endl



std::string	test_name(std::string s)
{
	std::string::size_type n = 0;
	while ( ( n = s.find( "_", n ) ) != std::string::npos )
	{
		s.replace( n, 1, " " );
		n += 1;
	}
	return (s);
}

#endif