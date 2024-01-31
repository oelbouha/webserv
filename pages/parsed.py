#!//usr/local/bin/python3

import os
import sys
test = "x-test: " + os.environ['URI']

print("\n/**********************************************************", file=sys.stderr)
print("URI:" + os.environ['URI'], file=sys.stderr)
print("**********************************************************/\n", file=sys.stderr)

body = '''
<!DOCTYPE html>
<html>
    <head>
        <title>cgi test page</title>
    </head>
    <body style="margin:0; background-color: black;color:white;">
        <h1>This is a title</h1>
        <ul>
            <li>this is working</li>
            <li>i guess</li>
            <img src="/index.jpeg">
        </ul>
    </body>
</html>
'''

# print("Status: 201 Created\r")
print("content-type: text/html\r")
print("set-cookie: sessionid=thisissessionid\r");
print("content-length: ", end="")
print(len(body), end="")
print("\r")

print("\r")
print(body)
