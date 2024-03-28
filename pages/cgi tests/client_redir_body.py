#!/usr/local/bin/python3
import os

body = '''<!DOCTYPE html>
<html>
    <head>
        <title>redir</title>
    </head>
    <body style="margin:0; background-color: black;color:white;">
        <h1>This is a title</h1>
        <ul>
            <li>this is working</li>
            <li>i guess</li>
        </ul>
    </body>
</html>'''

print("location: http://%s/test_dir/\r"%os.getenv("SERVER_NAME"))
print("status: 300 Multiple Choices\r")
print("content-type: text/html\r")
print("\r")
print(body)

