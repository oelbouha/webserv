

#include <iostream>
#include <string>
#include <stdio.h>

int main()
{
    /*
    HTTP/1.1 200 OK
    Connection: keep-alive
    Content-Length: 34803
    Content-Type: text/html
    */

	std::string str("this is the body");

	printf("status: 200 Ok\r\n");
    printf("content-length: %lu\r\n", str.length());
    printf("Content-Type: text/html\r\n");
    printf("\r\n");
    printf("%s\r\n\r\n", str.c_str());
}