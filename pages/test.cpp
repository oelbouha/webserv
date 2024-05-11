

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

	printf("HTTP/1.1 200 OK\r\n");
    printf("Connection: keep-alive\r\n");
    printf("content-length: %lu\r\n", str.length());
    printf("Content-Type: text/html\r\n");
    printf("\r\n\r\n");
    printf("%s\r\n\r\n", str.c_str());
}