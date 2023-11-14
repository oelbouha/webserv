#ifndef TESTS_TEST_HPP
#define TESTS_TEST_HPP

#include <iostream>
#include <string>

std::string	test_name(std::string s);

#define	SUITE(name) \
std::cout << __FILE__ << " : " << name << std::endl

#define TEST_NAME(name) test_name(#name)

#define TEST(name, code) 									\
void	name(){ 											\
	bool	fail = false;									\
	try {													\
		code												\
	}catch(...){											\
		std::cout<<"\t\e[31m✗ unhandled exception\e[0m - "; \
		std::cout << TEST_NAME(name) << std::endl;			\
		return ;											\
	} 														\
	if (fail)												\
		std::cout << "\t\e[31m✗ fail\e[0m - "; 				\
	else													\
		std::cout << "\t\e[32m✓ pass\e[0m - ";				\
	std::cout << TEST_NAME(name) << std::endl;				\
}

#define TEST_RUN(name)	name()


#define	ASSERT_EXCEPTION(code, exception)	\
try{										\
	code 									\
} catch(exception&) { 						\
} catch(...){ 								\
	fail = true;							\
}

#define	ASSERT_NO_EXCEPTION(code)		\
try{ 									\
	code				 				\
} catch(...){ 							\
	fail = true; 						\
}


#define	ASSERT_EQUAL(one, two) 				\
	if (one != two)							\
		fail = true;

#define	ASSERT_NOT_EQUAL(one, two) 			\
	if (one == two)							\
		fail = true;



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