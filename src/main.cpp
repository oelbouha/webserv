

#include "ILogger.hpp"

#include "Logger.hpp"

#include <string>

int main()
{
    std::string s("hello");

    INFO( s << " " << 10 << logger::endl );

    return (0);
}